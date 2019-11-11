#include "stdafx.h"
#include "Utilites.h"

#ifdef DEVKIT
#include <xbdm.h>
#endif

VOID _PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked)
{
	if (Destination & 0x8000)
	{
		Address[0] = 0x3D600000 + (((Destination >> 16) & 0xFFFF) + 1);
	} // lis       r11, Destination@h
	else { Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF); } // lis       r11, Destination@h
	Address[1] = 0x396B0000 + (Destination & 0xFFFF); // addi      r11, r11, Destination@l
	Address[2] = 0x7D6903A6; // mtspr   CTR, r11
	if (Linked == TRUE)
	{
		Address[3] = 0x4E800421;
	} // bctrl
	else { Address[3] = 0x4E800420; } // bctr
}
VOID Utilities::PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked)
{
#ifdef DEVKIT
	DWORD Data[4];
	PatchInJump(Data, Destination, Linked);
	DWORD cbRet = 0;
	DmSetMemory((LPVOID)Address, 16, Data, &cbRet);
#else
	_PatchInJump(Address, Destination, Linked);
#endif
}
VOID __declspec(naked) GLPR(VOID)
{
	__asm
	{
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
			std     r16, -0x88(sp)
			std     r17, -0x80(sp)
			std     r18, -0x78(sp)
			std     r19, -0x70(sp)
			std     r20, -0x68(sp)
			std     r21, -0x60(sp)
			std     r22, -0x58(sp)
			std     r23, -0x50(sp)
			std     r24, -0x48(sp)
			std     r25, -0x40(sp)
			std     r26, -0x38(sp)
			std     r27, -0x30(sp)
			std     r28, -0x28(sp)
			std     r29, -0x20(sp)
			std     r30, -0x18(sp)
			std     r31, -0x10(sp)
			stw     r12, -0x8(sp)
			blr
	}
}
DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress)
{
	DWORD Instruction = 0, Replacing;
	PDWORD Saver = (PDWORD)GLPR;
	if (SFSOffset & 0x2000000)
	{
		SFSOffset = SFSOffset | 0xFC000000;
	}
	Replacing = OriginalAddress[SFSOffset / 4];
	for (int i = 0; i < 20; i++)
	{
		if (Replacing == Saver[i])
		{
			DWORD NewOffset = (DWORD)&Saver[i] - (DWORD)SaveStubAddress;
			Instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
		}
	}
	return Instruction;
}
VOID Utilities::HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination)
{
	if ((SaveStub != NULL) && (Address != NULL)) // Make sure they are not nothing.
	{
		DWORD AddressRelocation = (DWORD)(&Address[4]); // Replacing 4 instructions with a jump, this is the stub return address
		if (AddressRelocation & 0x8000)
		{
			SaveStub[0] = 0x3D600000 + (((AddressRelocation >> 16) & 0xFFFF) + 1); // lis r11, 0 | Load Immediate Shifted
		}
		else
		{
			SaveStub[0] = 0x3D600000 + ((AddressRelocation >> 16) & 0xFFFF); // lis r11, 0 | Load Immediate Shifted
		}
		SaveStub[1] = 0x396B0000 + (AddressRelocation & 0xFFFF); // addi r11, r11, (value of AddressRelocation & 0xFFFF) | Add Immediate
		SaveStub[2] = 0x7D6903A6; // mtspr CTR, r11 | Move to Special-Purpose Register CTR
								  // Instructions [3] through [6] are replaced with the original instructions from the function hook
								  // Copy original instructions over, relink stack frame saves to local ones
		for (int i = 0; i < 4; i++)
		{
			if ((Address[i] & 0x48000003) == 0x48000001)
			{
				SaveStub[i + 3] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
			}
			else
			{
				SaveStub[i + 3] = Address[i];
			}
		}
		SaveStub[7] = 0x4E800420; // Branch unconditionally
		__dcbst(0, SaveStub); // Data Cache Block Store | Allows a program to copy the contents of a modified block to main memory.
		__sync(); // Synchronize | Ensure the dcbst instruction has completed.
		__isync(); // Instruction Synchronize | Refetches any instructions that might have been fetched prior to this instruction.
		PatchInJump(Address, Destination, FALSE); // Redirect Function to ours

		/*
		* So in the end, this will produce:
		*
		* lis r11, ((AddressRelocation >> 16) & 0xFFFF [+ 1])
		* addi r11, r11, (AddressRelocation & 0xFFFF)
		* mtspr CTR, r11
		* branch (?Destination?)
		* dcbst 0, (SaveStub)
		* sync
		*/
	}
}
DWORD Utilities::PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress)
{
	PLDR_DATA_TABLE_ENTRY ModuleHandle = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle(Module);
	return PatchModuleImport(ModuleHandle, ImportedModuleName, Ordinal, PatchAddress);
}
DWORD Utilities::PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress)
{
	// TODO: Clean up names and stuff

	DWORD address = (DWORD)Utilities::ResolveFunction(ImportedModuleName, Ordinal);

	VOID* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(headerBase, 0x000103FF);

	DWORD result = 2;

	CHAR* stringTable = (CHAR*)(importDesc + 1);

	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++)
	{
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++)
		{
			DWORD value = *((DWORD*)importAdd[y]);
			if (value == address)
			{
				memcpy((DWORD*)importAdd[y], &PatchAddress, 4);
				DWORD newCode[4];
				PatchInJump(newCode, PatchAddress, FALSE);
				memcpy((DWORD*)importAdd[y + 1], newCode, 16);

				result = S_OK;
			}
		}

		importTable = (XEX_IMPORT_TABLE_ORG*)(((BYTE*)importTable) + importTable->TableSize);
	}

	return result;
}

DWORD BytesRead, BytesWritten;
BOOL Utilities::FileExists(PCHAR Path)
{
	if (GetFileAttributes(Path) == -1)
	{
		DWORD LastError = GetLastError();
		if (LastError == ERROR_FILE_NOT_FOUND || LastError == ERROR_PATH_NOT_FOUND)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL Utilities::ReadFileAlt(PCHAR Path, LPVOID Buffer, DWORD dwBytesToRead)
{
	HANDLE hFile = CreateFile(Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile); return FALSE;
	}
	ReadFile(hFile, Buffer, dwBytesToRead, &BytesRead, NULL);
	CloseHandle(hFile);
	return TRUE;
}
int Utilities::getValuePatch(int id, XboxGames g_id) {
	switch (g_id) {
	case XboxGames::BO2:
		{
			switch (id)
			{
			case 1: // hooking
				return 0x824744E0;
				break;
			case 2: // check multiplayer
				return 0x82003580;
				break;
			case 3: // red boxes
				return 0x821F5B7C;
				break;
			case 4: // chams
				return 0x821fc04c;
				break;
			case 5: // no sway (no zoom)
				return 0x826C6E6C;
				break;
			case 6: // zooming
				return 0x826C7A7C;
				break;
			case 7: // advanced uav
				return 0x821B8FD0;
				break;
			case 8: // no recoil
				return 0x82259BC8;
				break;
			case 9: // remove cold blood
				return 0x821F608C;
				break;
			case 10: // laser
				return 0x82255E1C;
				break;
			case 11: // wallhack
				return 0x829052FC;
				break;
			case 12: // fps (null)
				return 0x01cc6e98;
				break;
			case 13: // force game chat  
				return 0x825DCA68;
				break;
			case 14: // BO2 White 
				return 0x83170788;
				break;
			case 15: // isVisble
				return 0x280F803;
				break;
			case 16: // mTag 
				return 0x836C5C92;
				break;
			}
		}
		break;
	}
	return 0;
}
BOOL Utilities::WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite)
{
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile); return FALSE;
	}
	WriteFile(hFile, Buffer, dwBytesToWrite, &BytesRead, NULL);
	CloseHandle(hFile);
	return TRUE;
}

DWORD Utilities::ResolveFunction(PCHAR ModuleName, DWORD Ordinal)
{
	HANDLE hModule; DWORD Address;
	XexGetModuleHandle(ModuleName, &hModule);
	XexGetProcedureAddress(hModule, Ordinal, &Address);
	return Address;
}

BOOL Utilities::IsEmpty(PBYTE Buffer, DWORD Size)
{
	for (DWORD i = 0; i < Size; i++)
	{
		if (Buffer[i] != NULL)
		{
			return FALSE;
		}
	}
	return TRUE;
}


static bool initialized = false;
static HANDLE workerThreadHandle;
static HANDLE workerThreadStartupEventHandle;
static HANDLE workerThreadIOCPHandle;
static DWORD workerThreadId;
HANDLE _xamHandle;
HANDLE _dllHandle;
VOID(__cdecl *XNotifyQueueUI)(DWORD dwType, DWORD dwUserIndex, DWORD dwPriority, LPCWSTR pwszStringParam, ULONGLONG qwParam) = (VOID(__cdecl *)(DWORD, DWORD, DWORD, LPCWSTR, ULONGLONG))Utilities::ResolveFunction("xam.xex", 0x290);
VOID(__cdecl *XNotifyUISetOptions)(BOOL pfShow, BOOL pfShowMovie, BOOL pfPlaySound, BOOL pfShowIPTV) = (VOID(__cdecl *)(BOOL, BOOL, BOOL, BOOL))Utilities::ResolveFunction("xam.xex", 658);
VOID(__cdecl *XNotifyUIGetOptions)(BOOL* pfShow OPTIONAL, BOOL* pfShowMovie OPTIONAL, BOOL* pfPlaySound OPTIONAL, BOOL* pfShowIPTV OPTIONAL) = (VOID(__cdecl *)(BOOL*, BOOL*, BOOL*, BOOL*))Utilities::ResolveFunction("xam.xex", 659);
static VOID toggleNotify(bool on) {
	static BOOL pfShow;
	static BOOL pfShowMovie;
	static BOOL pfPlaySound;
	static BOOL pfShowIPTV;

	if (!on) {
		XNotifyUISetOptions(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
	}
	else {
		XNotifyUIGetOptions(&pfShow, &pfShowMovie, &pfPlaySound, &pfShowIPTV);
		XNotifyUISetOptions(true, true, true, true);
	}
}

static VOID XNotifyDoQueueUI(PWCHAR displayText) {
	toggleNotify(true);
	XNotifyQueueUI(16, XUSER_INDEX_ANY, 2, displayText, NULL);
	toggleNotify(false);
}

HRESULT Utilities::xNotify(PWCHAR displayText) {
	if (KeGetCurrentProcessType() != 1) {
		HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)XNotifyDoQueueUI, (LPVOID)displayText, CREATE_SUSPENDED, NULL);
		if (th == NULL) return E_FAIL;
		ResumeThread(th);
		WaitForSingleObject(th, INFINITE);
	}
	else
		XNotifyDoQueueUI(displayText);

	return S_OK;
}