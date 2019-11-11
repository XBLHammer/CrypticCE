#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "stdafx.h"

#define GetPointer(X) *(PDWORD)(X)
#define CSleep(X) Sleep(X * 1000)

enum XboxGames {
	BO2 = 0x415608C3
};

class Utilities {
public:
	// Hooking
	static VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked);
	static VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination);
	static DWORD PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);

	// File Stuff
	static BOOL FileExists(PCHAR Path);
	static BOOL ReadFileAlt(PCHAR Path, LPVOID Buffer, DWORD dwBytesToRead);
	static BOOL WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite);

	// Misc
	static DWORD ResolveFunction(PCHAR ModuleName, DWORD Ordinal);
	static HRESULT xNotify(PWCHAR displayText);
	static BOOL IsEmpty(PBYTE Buffer, DWORD Size);
	static int getValuePatch(int id, XboxGames g_id);
};
#endif // _UTILITIES_H