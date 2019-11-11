#include "stdafx.h"
#include "Varibles.h"
#include "Huds.h"
#include "Menu.h"
#include "Controls.h"
#include <time.h>
#include <stdio.h>

bool bInitialized;

__declspec(naked) int SaveStubPacket(int a1)
{
	__asm
	{
		nop
		nop
			nop
			nop
			nop
			nop
			nop
	}
}

DWORD(__cdecl *XamGetCurrentTitleID)() = (DWORD(__cdecl *)())Utilities::ResolveFunction("xam.xex", 0x1CF);

int __declspec(naked) H_CL_WritePacketStub(int r3)
{
	__asm
	{
		li r3, 8
		nop
			nop
			nop
			nop
			nop
			nop
			blr
	}
}

bool bSendPacket;

// some tampered no spread, i was curious about getting to working *actually certain it works* / /
int H_CL_WritePacket(int r3)
{
	if (Addresses::Dvar_GetBool("cl_ingame")) {
		GetStructs();

		bSendPacket = true;

		Usercmd_t* cur = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber) & 0x7F];
		Usercmd_t* newCmd = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber) & 0x7F];
		Usercmd_t* old = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber - 1) & 0x7F];

		memcpy(old, cur, sizeof(Usercmd_t));
		old->time -= 1;

		if (vars.nospread)
			_spreadFix(old);

		if (bSendPacket)
			return H_CL_WritePacketStub(r3);
	}
	else {
		return H_CL_WritePacketStub(r3);
	}	
}

DWORD __declspec(naked) XamInputGetStateHook_Stub(DWORD dwUserIndex, DWORD r4, PXINPUT_STATE pState)
{
	__asm
	{
		li r3, 1
		nop
			li r4, 1
			nop
			nop
			nop
			nop
			blr
	}
}


DWORD XamInputGetStateHookBO2(DWORD dwUserIndex, DWORD dwFlags, PXINPUT_STATE pState) {
	DWORD r = XInputGetStateEx(dwUserIndex, dwFlags, pState);

	if (vars.auto_shoot) {
		if (vars.can_shoot) {
			pState->Gamepad.bRightTrigger = 100;
			vars.can_shoot = false;
		}
	}

	if (vars.auto_aim && vars.can_aim) {
		pState->Gamepad.bLeftTrigger = 100;
		vars.can_aim = false;
	}

	return r;
}


VOID __InitOpusCE()
{
	for (;;)
	{
		if (XamGetCurrentTitleID() == XboxGames::BO2 && !strcmp((CONST PCHAR)Utilities::getValuePatch(2, XboxGames::BO2), "multiplayer") == TRUE)
		{
			if (bInitialized == FALSE)
			{
				Sleep(2000);
				Utilities::xNotify(L"CrypticPro - Initializing Cheats...");
				Utilities::xNotify(L"CrypticPro - Success");
				Utilities::PatchInJump((DWORD*)0x82262978, (DWORD)CL_RenderScenehk, false);
				Utilities::PatchInJump((DWORD*)0x8293D884, (DWORD)XamInputGetStateHookBO2, false);
				bInitialized = TRUE;
			}
		}
		else { bInitialized = FALSE; }
		Sleep(120);
	}
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:

		HANDLE hThread; DWORD hThreadID;
		ExCreateThread(&hThread, 0, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)__InitOpusCE, NULL, 0x2);
		XSetThreadProcessor(hThread, 4);
		ResumeThread(hThread);
		CloseHandle(hThread);

		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

