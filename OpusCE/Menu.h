#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <xbox.h>
#include "Structures.h"



typedef HRESULT(*pDmSetMemory)(LPVOID lpbAddr, DWORD cb, LPCVOID lpbBuf, LPDWORD pcbRet);
pDmSetMemory DevSetMemory = NULL;
HRESULT SetMemory(VOID* Destination, VOID* Source, DWORD Length) {

	// Try to resolve our function
	if (DevSetMemory == NULL)
		DevSetMemory = (pDmSetMemory)Utilities::ResolveFunction("xbdm.xex", 40);

	// Now lets try to set our memory
	if (DevSetMemory == NULL) {
		memcpy(Destination, Source, Length);
		return ERROR_SUCCESS;
	}
	else {
		if (DevSetMemory(Destination, Length, Source, NULL) == MAKE_HRESULT(0, 0x2da, 0))
			return ERROR_SUCCESS;
	}

	// We have a problem..
	return E_FAIL;
}

MESSAGEBOX_RESULT g_mb_result;
XOVERLAPPED g_xol;

char editgt() {
	WCHAR tokenInput[512];
	int tokenMaxLen = 15;
	char token;
	char invalid = 0xFF;
	XOVERLAPPED Overlapped;
	ZeroMemory(&Overlapped, sizeof(Overlapped));
	XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"Edit Gamertag", L"Enter A Valid 15 Character Or Less Gamertag To Poke", tokenInput, 20, &Overlapped);
	while (!XHasOverlappedIoCompleted(&Overlapped))
		Sleep(100);
	if (wcslen(tokenInput) != 0) {
		wcstombs((char*)token, tokenInput, tokenMaxLen);
		if (strlen((const char*)token) > 15) {
			return invalid;
		}
		return token;
	}
}


void CustomGT()
{
	XOVERLAPPED Overlapped;
	WCHAR wValue[512];
	char Buffer[0x100];
	ZeroMemory(&Overlapped, sizeof(Overlapped));
	XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"Custom GT Editor!", L"Please Enter your Pre-Game Gamertag!\n(25 Letters)", wValue, 26, &Overlapped);
	while (!XHasOverlappedIoCompleted(&Overlapped))
		Sleep(100);
	wcstombs(Buffer, wValue, 26);
	strcpy((char*)0x841E1B30, Buffer);
}

bool __force_gamechat = true;
bool __enemy_esp_default = true;
bool __friendly_esp_default = true;
bool __force_nameesp = true;
bool __force_distanceesp = true;
bool __force_weaponnames = true;
bool __force_weaponicons = true;
bool __force_debug_info = true;
bool __force_target_info = true;

bool isSet = false;

void resetEspTypes() {
	if (!isSet)
	{
		vars._2DEsp = true;
		vars._3DEsp = false;
		vars._TRIEsp = false;
		vars._SHADEDEsp = false;
		vars._CORNEREsp = false;
	}
}

bool espview_IsSet = false;

void resetEspViews() {
	if (!espview_IsSet) {
		vars._TeamView = true;
		vars._EnemyView = true;
	}
}
void LoadBO2Menu() {
	if (Addresses::Dvar_GetBool("cl_ingame")) {
		DrawMenu();
		addTab("Aimbot", 0, 175);
		addTab("ESP", 1, 175);
		addTab("Recovery", 2, 175);
		addTab("Misc.", 3, 175);
		addTab("Controls", 4, 175);
		switch (vars.submenu) {
		case _Aimbot:
			addBool("No spread", 0, 0, 0, vars.nospread, 26, 175);
			addBool("HeadShot Aimbot", 1, 0, 0, vars.aimbot_, 26, 175);
			addBool("Nearest Aimbot", 2, 0, 0, vars.aimbotnearest_, 26, 175);
			addBool("Auto Aim", 3, 0, 0, vars.auto_aim, 26, 175);
			addBool("Auto Shoot", 4, 0, 0, vars.auto_shoot, 26, 175);
			break;
		case _ESP:
			resetEspTypes();
			resetEspViews();
			addBool("Radar", 0, 0, 0, vars.enable_radar, 26, 175);
			addSliderOption("Radar Distance", 1, 0, sliderCount_radar_distance, 6, 175);
			addBool("Compass", 2, 0, 0, vars.enable_compass, 27, 175);
			addSliderOption("Compass Scale", 3, 0, sliderCount_compass_size, 4, 175);
			addCharOption("Compass Color", 4, 0, _CompassColors, &_CompassColorSelector, _CompassColorMin, _CompassColorMax, 27, 175);
			addBool("External ESP", 5, 0, 0, vars.enable_esp, 29, 175);
			addCharOption("ESP Type", 6, 0, _ESPTypes, &_ESPSelector, _ESPMin, _ESPMax, 29, 175);
			addBool("Jordans", 7, 0, 0, vars.jordan_esp, 30, 175);
			addBool("Health bar", 8, 0, 0, vars.health_bar, 30, 175);
			addCharOption("ESP View", 9, 0, _ESPView, &_ESPViewSelector, _ESPViewMin, _ESPViewMax, 26, 175);
			addCharOption("Enemy ESP Color", 10, 1, _EnemyColors, &_EnemyESPColorSelector, _EnemyESPColorMin, _EnemyESPColorMax, 28, 175);
			addCharOption("Enemy ESP Size", 11, 2, _EnemyESPSizes, &_EnemyESPSizeSelector, _EnemyESPSizeMin, _EnemyESPSizeMax, 28, 175);
			addCharOption("Team ESP Color", 12, 3, _TeamColors, &_TeamESPColorSelector, _TeamESPColorMin, _TeamESPColorMax, 28, 175);
			addCharOption("Team ESP Size", 13, 4, _TeamESPSizes, &_TeamESPSizeSelector, _TeamESPSizeMin, _TeamESPSizeMax, 28, 175);
			addBool("Names", 14, 5, 0, vars.name_esp, 29, 175); if (__force_nameesp) { vars.name_esp = true; }
			addBool("Distance", 15, 6, 0, vars.distance_esp, 29, 175); if (__force_distanceesp) { vars.distance_esp = true; }
			addBool("Weapon Names", 16, 7, 0, vars.weapon_names_esp, 29, 175); if (__force_weaponnames) { vars.weapon_names_esp = true; }
			addBool("Weapon Icons", 17, 8, 0, vars.weapon_icon_esp, 29, 175); if (__force_weaponicons) { vars.weapon_icon_esp = true; }
			addBool("Debug Info", 18, 9, 0, vars.debug_info, 26, 175); if (__force_debug_info) { vars.debug_info = true; }
			addBool("Target Details", 19, 10, 1, vars.target_details_, 26, 175); if (__force_target_info) { vars.target_details_ = true; }
			break;
		case HVH:
			addOption("Give Unlock Tokens", 0, 0, 26, 175);
			addOption("Unlock All", 1, 0, 26, 175);
			addOption("10 Classes", 2, 0, 26, 175);
			addOption("Ghost Camo Class 1", 3, 0, 26, 175);
			addOption("Ghost Camo Class 2", 4, 0, 26, 175);
			addOption("Ghost Camo Class 3", 5, 0, 26, 175);
			break;
		case Misc:
			addBool("Red Boxes", 0, 0, 0, vars.redboxes, 26, 175);
			addBool("Chams", 1, 0, 0, vars.chams, 26, 175);
			addBool("No Sway", 2, 0, 0, vars.nosway, 26, 175);
			addBool("Advanced UAV", 3, 0, 0, vars.advanced_uav, 26, 175);
			addBool("No recoil", 4, 0, 0, vars.norecoil, 26, 175);
			addSliderOption("FOV Changer", 5, 0, sliderCount_fov, 5, 175);
			addBool("Remove Blood", 6, 0, 0, vars.removecoldblood, 27, 175);
			addBool("Weapon Laser", 7, 0, 0, vars.laser, 27, 175);
			addBool("Wallhack", 8, 0, 0, vars.wallhack, 27, 175);
			addOption("End game", 9, 0, 26, 175);
			addBool("Force Gamechat", 10, 1, 0, vars.forcegamechat, 26, 175); /* enable default */ if (__force_gamechat) { vars.forcegamechat = true; }
			break;
		case Controls:
			float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
			_DrawText("Controls:", 709.0f, 257.0f + (24 + 0), 0.5f, flColor);
			_DrawText("Switch Between Tabs:", 709.0f, 310.0f, 0.3f, flColor);
			_DrawText("to change tabs use your LEFT & RIGHT bumpers", 709.0f, 322.0f, 0.25f, White);
			_DrawText("Scroll:", 709.0f, 347.0f, 0.3f, flColor);
			_DrawText("to scroll, use the UP & DOWN buttons on your DPAD", 709.0f, 359.0f, 0.25f, White);
			_DrawText("Execute Options:", 709.0f, 382.0f, 0.3f, flColor);
			_DrawText("to execute an option, press A on your controller", 709.0f, 395.0f, 0.25f, White);
			break;

		}
	}
}

void resetVars() {
	if (!vars.Menuopen) {
		__setupMenu(_Aimbot, Aimbot, 0, 3, 0, 9, null);
	}
	else {
		vars.Menuopen = false;
		vars.RGB = false;
	}
	vars.ticktock = 0;
}

void switchTabMenu() {
	switch (vars.tabs) {
	case 0: // aimbot
		changeSubmenu(_Aimbot, 4, null);
		break;
	case 1: // ESP
		changeSubmenu(_ESP, 19, _Aimbot);
		break;
	case 2: //HVH 
		changeSubmenu(HVH, 5, _ESP);
		break;
	case 3: // Misc. 
		changeSubmenu(Misc, 10, HVH);
		break;
	case 4: // Controls
		changeSubmenu(Controls, 3, Misc);
		break;
	}
}



void _executeMenu() {	
	switch (vars.submenu)
	{
	case _Aimbot:
		switch (vars.scroll)
		{
		case 0: 
			if (!vars.nospread) {
				*(int*)0x82254810 = 0x60000000; // for visuals -- 
				*(int*)0x821C7234 = 0x60000000; // for visuals --
				vars.nospread = true;
			}
			else {
				*(int*)0x82254810 = 0x48466CD1; // for visuals -- 
				*(int*)0x821C7234 = 0x484F42AD; // for visuals -- 
				vars.nospread = false;
			}
			break;
		case 1: 
			if (!vars.aimbot_) {
				vars.aimbot_ = true;
			}
			else{
				vars.aimbot_ = false;
			}
			break;
		case 2:
			if (!vars.aimbotnearest_) {
				vars.aimbotnearest_ = true;
			}
			else {
				vars.aimbotnearest_ = false;
			}
			break;
		case 3:
			if (!vars.auto_aim) {
				vars.auto_aim = true;
			}
			else {
				vars.auto_aim = false;
			}
			break;
		case 4:
			if (!vars.auto_shoot) {
				vars.auto_shoot = true;
			}
			else {
				vars.auto_shoot = false;
			}
			break;
		}
		break;
	case HVH:
		switch (vars.scroll)
		{
		case 0:
		{
			byte unlocktokens[] = { 0xc0, 0x3f };
			SetMemory((PVOID)0x84352ac8, unlocktokens, 4);
			}
			break;
		case 1:
		{
			byte unlocked[1] = { 0xff };
			SetMemory((PVOID)0x8435429f, UnlockAll, 4);
			}
			break;
		case 2:
		{
			byte unlocked[1] = { 0xff };
			SetMemory((PVOID)0x843543ac, unlocked, 4);
			SetMemory((PVOID)0x843543ac, unlocked, 4);
		}
		break;
		case 3:
		{
			byte classes[8] = { 0x44, 0x80, 8, 0x10, 1, 0x22, 0x40, 4 };
			SetMemory((PVOID)0x843546b2, classes, 4);

		}
		break;
		case 4:
		{
			byte GhostCammo1[2] = { 0x51, 0xA1 };
			SetMemory((PVOID)0x84353A50, GhostCammo1, 4);
			byte GhostCammo2[2] = { 00, 0xA8 };
			SetMemory((PVOID)0x84353A5D, GhostCammo2, 4);

		}
		break;
		case 5:
		{
			byte GhostCammo3[3] = { 00, 00, 15 };
			SetMemory((PVOID)0x84353A83, GhostCammo3, 4);

			byte GhostCammo4[2] = { 00, 0xA8 };
			SetMemory((PVOID)0x84353A5D, GhostCammo4, 4);

		}
		break;
		case 6:
		{
			byte GhostCammo5[2] = { 50, 01 };
			SetMemory((PVOID)0x84353A83, GhostCammo5, 4);

			byte GhostCammo6[2] = { 00, 0xA8 };
			SetMemory((PVOID)0x84353AC6, GhostCammo6, 4);

		}
		break;
		}
		break;


	case _ESP:
		switch (vars.scroll)
		{
		case 0:
			if (!vars.enable_radar) {
				vars.enable_radar = true;
			}
			else {
				vars.enable_radar = false;
			}
			break;
		case 1: 
			sliderCount_radar_distance++;
			if (sliderCount_radar_distance == 13) {
				sliderCount_radar_distance = 1;
				_raderDistance = 0.3f;
			}
			else {
				switch (sliderCount_radar_distance) {
				case 1: 
					_raderDistance = _raderDistance + 0.3f; 
					break;
				case 2: _raderDistance = _raderDistance + 0.3f; break;
				case 3: _raderDistance = _raderDistance + 0.3f; break;
				case 4: _raderDistance = _raderDistance + 0.3f; break;
				case 5: _raderDistance = _raderDistance + 0.3f; break;
				case 6: _raderDistance = _raderDistance + 0.3f; break;
				case 7: _raderDistance = _raderDistance + 0.3f; break;
				case 8: _raderDistance = _raderDistance + 0.3f; break;
				case 9: _raderDistance = _raderDistance + 0.3f; break;
				case 10: _raderDistance = _raderDistance + 0.3f; break;
				case 11: _raderDistance = _raderDistance + 0.3f; break;
				case 12: _raderDistance = _raderDistance + 0.3f; break;
				case 13: _raderDistance = _raderDistance + 0.3f; break;
				case 14: _raderDistance = _raderDistance + 0.3f; break;
				}
			}
			break;
		case 2: 
			if (!vars.enable_compass) {
				vars.enable_compass = true;
			}
			else {
				vars.enable_compass = false;
			}
			break;
		case 3: 
			sliderCount_compass_size++;
			if (sliderCount_compass_size == 6) {
				sliderCount_compass_size = 1;
				_compassScale = 200;
			}
			else {
				switch (sliderCount_compass_size) {
				case 1:
					_compassScale = _compassScale + 20;
					break;
				case 2: _compassScale = _compassScale + 20; break;
				case 3: _compassScale = _compassScale + 20; break;
				case 4: _compassScale = _compassScale + 20; break;
				case 5: _compassScale = _compassScale + 20; break;
				case 6: _compassScale = _compassScale + 20; break;
				case 7: _compassScale = _compassScale + 20; break;
				}
			}
			break;
		case 4: 
			if (*&_CompassColorSelector >= _CompassColorMax) {
				*&_CompassColorSelector = _CompassColorMin;
			}
			else {
				*&_CompassColorSelector = *&_CompassColorSelector + 1;
			}
			switch (_CompassColorSelector) {
			case 0: // RGB 
				compassColor = __rgbCol;
				break;
			case 1: // Green 
				compassColor = __Green_;
				break;
			case 2: // White
				compassColor = __White__;
				break;
			case 3: // Red
				compassColor = __Red_;
				break;
			case 4: // Blue
				compassColor = __blue_;
				break;
			}
			break;
		case 5: 
			if (!vars.enable_esp) {
				vars.enable_esp = true;
			}
			else {
				vars.enable_esp = false;
			}
			break;
		case 6: 
			isSet = true;
			if (*&_ESPSelector >= _ESPMax) {
				*&_ESPSelector = _ESPMin;
			}
			else {
				*&_ESPSelector = *&_ESPSelector + 1;
			}
			switch (_ESPSelector) {
			case 0: // 2D
				vars._2DEsp = true;
				vars._3DEsp = false;
				vars._TRIEsp = false;
				vars._SHADEDEsp = false;
				vars._CORNEREsp = false;
				break;
			case 1: // 3D
				vars._2DEsp = false;
				vars._3DEsp = true;
				vars._TRIEsp = false;
				vars._SHADEDEsp = false;
				vars._CORNEREsp = false;
				break;
			case 2: // Triangles
				vars._2DEsp = false;
				vars._3DEsp = false;
				vars._TRIEsp = true;
				vars._SHADEDEsp = false;
				vars._CORNEREsp = false;
				break;
			case 3: // Shaded
				vars._2DEsp = false;
				vars._3DEsp = false;
				vars._TRIEsp = false;
				vars._SHADEDEsp = true;
				vars._CORNEREsp = false;
				break;
			case 4: // Corners
				vars._2DEsp = false;
				vars._3DEsp = false;
				vars._TRIEsp = false;
				vars._SHADEDEsp = false;
				vars._CORNEREsp = true;
				break;
			}
			break;
		case 7: 
			if (!vars.jordan_esp) {
				vars.jordan_esp = true;
			}
			else {
				vars.jordan_esp = false;
			}
			break;
		case 8: 
			if (!vars.health_bar) {
				vars.health_bar = true;
			}
			else {
				vars.health_bar = false;
			}
			break;
		case 9: 
			espview_IsSet = true;
			if (*&_ESPViewSelector >= _ESPViewMax) {
				*&_ESPViewSelector = _ESPViewMin;
			}
			else {
				*&_ESPViewSelector = *&_ESPViewSelector + 1;
			}
			switch (_ESPViewSelector) {
			case 0: 
				vars._TeamView = true;
				vars._EnemyView = true;
				break;
			case 1:
				vars._TeamView = true;
				vars._EnemyView = false;
				break;
			case 2: 
				vars._TeamView = false;
				vars._EnemyView = true;
				break;
			}
			break;
		case 10: 
			/* "RGB", "Yellow", "White", "Red", "Blue" */
			if (*&_EnemyESPColorSelector >= _EnemyESPColorMax) {
				*&_EnemyESPColorSelector = _EnemyESPColorMin;
			}
			else {
				*&_EnemyESPColorSelector = *&_EnemyESPColorSelector + 1;
			}
			switch (_EnemyESPColorSelector) {
			case 0: // RGB
				_EnemyColor = __rgbCol;
				break;
			case 1: 
				_EnemyColor = __Yellow;
				break;
			case 2:
				_EnemyColor = __White__;
				break;
			case 3:
				_EnemyColor = __Red_;
				break;
			case 4:
				_EnemyColor = __blue_;
				break;
			}
			break;
		case 11: 
			if (*&_EnemyESPSizeSelector >= _EnemyESPSizeMax) {
				*&_EnemyESPSizeSelector = _EnemyESPSizeMin;
			}
			else {
				*&_EnemyESPSizeSelector = *&_EnemyESPSizeSelector + 1;
			}
			switch (_EnemyESPSizeSelector) {
			case 0:
				_EnemyESPSize = 1;
				break;
			case 1:
				_EnemyESPSize = 2;
				break;
			case 2:
				_EnemyESPSize = 3;
				break;
			case 3: 
				_EnemyESPSize = 4;
				break;
			}
			break;
		case 12:
			/* "RGB", "Yellow", "White", "Red", "Blue" */
			if (*&_TeamESPColorSelector >= _TeamESPColorMax) {
				*&_TeamESPColorSelector = _TeamESPColorMin;
			}
			else {
				*&_TeamESPColorSelector = *&_TeamESPColorSelector + 1;
			}
			switch (_TeamESPColorSelector) {
			case 0: // RGB
				_TeamColor = __rgbCol;
				break;
			case 1:
				_TeamColor = __Yellow;
				break;
			case 2:
				_TeamColor = __White__;
				break;
			case 3:
				_TeamColor = __Red_;
				break;
			case 4:
				_TeamColor = __blue_;
				break;
			}
			break;
		case 13:
			if (*&_TeamESPSizeSelector >= _TeamESPSizeMax) {
				*&_TeamESPSizeSelector = _TeamESPSizeMin;
			}
			else {
				*&_TeamESPSizeSelector = *&_TeamESPSizeSelector + 1;
			}
			switch (_TeamESPSizeSelector) {
			case 0:
				_TeamESPSize = 1;
				break;
			case 1:
				_TeamESPSize = 2;
				break;
			case 2:
				_TeamESPSize = 3;
				break;
			case 3:
				_TeamESPSize = 4;
				break;
			}
			break;
		case 14: 
			__force_nameesp = false;
			if (!vars.name_esp) {
				vars.name_esp = true;
			}
			else {
				vars.name_esp = false;
			}
			break;
		case 15: 
			__force_distanceesp = false;
			if (!vars.distance_esp) {
				vars.distance_esp = true;
			}
			else {
				vars.distance_esp = false;
			}
			break;
		case 16:
			__force_weaponnames = false;
			if (!vars.weapon_names_esp) {
				vars.weapon_names_esp = true;
			}
			else {
				vars.weapon_names_esp = false;
			}
			break;
		case 17:
			__force_weaponicons = false;
			if (!vars.weapon_icon_esp) {
				vars.weapon_icon_esp = true;
			}
			else {
				vars.weapon_icon_esp = false;
			}
			break;
		case 18: 
			__force_debug_info = false;
			if (!vars.debug_info) {
				vars.debug_info = true;
			}
			else {
				vars.debug_info = false;
			}
			break;
		case 19: 
			__force_target_info = false;
			if (!vars.target_details_) {
				vars.target_details_ = true;
			}
			else {
				vars.target_details_ = false;
			}
			break;
		}
		break;
	case Misc:
		switch (vars.scroll)
		{
		case 0:
			if (!vars.redboxes) {
				*(int*)Utilities::getValuePatch(3, XboxGames::BO2) = 0x38600001;
				vars.redboxes = true;
			}
			else if (vars.redboxes)
			{
				*(int*)Utilities::getValuePatch(3, XboxGames::BO2) = 0x38600000;
				vars.redboxes = false;
			}
			break;
		case 1: 
			if (!vars.chams) 
			{
				*(int*)Utilities::getValuePatch(4, XboxGames::BO2) = 0x38c0ffff;
				vars.chams = true;
			}
			else 
			{
				*(int*)Utilities::getValuePatch(4, XboxGames::BO2) = 0x7fa6eb120;
				vars.chams = false;
			}
			break;
		case 2:
			if (!vars.nosway) {
				*(int*)Utilities::getValuePatch(5, XboxGames::BO2) = 0x60000000;
				*(int*)Utilities::getValuePatch(6, XboxGames::BO2) = 0x60000000;
				vars.nosway = true;
			}
			else {
				*(int*)Utilities::getValuePatch(5, XboxGames::BO2) = 0x4BFFE975;
				*(int*)Utilities::getValuePatch(6, XboxGames::BO2) = 0x4BFFFA85;
				vars.nosway = false;
			}
			break;
		case 3:	
			if (!vars.advanced_uav) {
				*(int*)Utilities::getValuePatch(7, XboxGames::BO2) = 0x2B0B0001;
				vars.advanced_uav = true;
			}
			else {
				*(int*)Utilities::getValuePatch(7, XboxGames::BO2) = 0x2B0B0000;
				vars.advanced_uav = false;
			}
			break;
		case 4: 
			if (!vars.norecoil) {
				*(int*)Utilities::getValuePatch(8, XboxGames::BO2) = 0x60000000;
				vars.norecoil = true;
			}
			else {
				*(int*)Utilities::getValuePatch(8, XboxGames::BO2) = 0x48461341;
				vars.norecoil = false;
			}
			break;
		case 5:
			sliderCount_fov++;
			if (sliderCount_fov == 0) {
				sliderCount_fov = 1;
			}
			else if (sliderCount_fov > 16) {
				sliderCount_fov = 1;
			}
			else {
				switch (sliderCount_fov) {
				case 1: Addresses::Cbuf_AddText(0, "cg_fov 65"); break;
				case 2: Addresses::Cbuf_AddText(0, "cg_fov 70"); break;
				case 3: Addresses::Cbuf_AddText(0, "cg_fov 75"); break;
				case 4: Addresses::Cbuf_AddText(0, "cg_fov 80"); break;
				case 5: Addresses::Cbuf_AddText(0, "cg_fov 85"); break;
				case 6: Addresses::Cbuf_AddText(0, "cg_fov 90"); break;
				case 7: Addresses::Cbuf_AddText(0, "cg_fov 95"); break;
				case 8: Addresses::Cbuf_AddText(0, "cg_fov 100"); break;
				case 9: Addresses::Cbuf_AddText(0, "cg_fov 105"); break;
				case 10: Addresses::Cbuf_AddText(0, "cg_fov 110"); break;
				case 11: Addresses::Cbuf_AddText(0, "cg_fov 115"); break;
				case 12: Addresses::Cbuf_AddText(0, "cg_fov 120"); break;
				case 13: Addresses::Cbuf_AddText(0, "cg_fov 125"); break;
				case 14: Addresses::Cbuf_AddText(0, "cg_fov 130"); break;
				case 15: Addresses::Cbuf_AddText(0, "cg_fov 135"); break;
				case 16: Addresses::Cbuf_AddText(0, "cg_fov 140"); break;
				}
			}
			break;
		case 6: 
			if (!vars.removecoldblood) {
				*(int*)Utilities::getValuePatch(9, XboxGames::BO2) = 0x60000000;
				vars.removecoldblood = true;
			}
			else {
				*(int*)Utilities::getValuePatch(9, XboxGames::BO2) = 0x419900C8;
				vars.removecoldblood = false;
			}
			break;
		case 7: 
			if (!vars.laser) {
				*(int*)Utilities::getValuePatch(10, XboxGames::BO2) = 0x2b110000;		
				vars.laser = true;
			}
			else{
				*(int*)Utilities::getValuePatch(10, XboxGames::BO2) = 0x2b110001;
				vars.laser = false;
			}
			break;
		case 8:
			if (!vars.wallhack) {
				*(int*)Utilities::getValuePatch(11, XboxGames::BO2) = 0x38600001;
				vars.wallhack = true;
			}
			else {
				*(int*)Utilities::getValuePatch(11, XboxGames::BO2) = 0x38600000;
				vars.wallhack = false;
			}
			break;
		case 9:
			char buffer[200];
			sprintf(buffer, "cmd mr %i -1 endround", *(int*)0x82C15758);
			Addresses::Cbuf_AddText(0, buffer);
			break;
		case 10: 
			if (!vars.forcegamechat) {
				*(int*)Utilities::getValuePatch(13, XboxGames::BO2) = 0x38600000;
				vars.forcegamechat = true;
			}
			else {
				*(int*)Utilities::getValuePatch(13, XboxGames::BO2) = 0x38600001;
				vars.forcegamechat = false;
				__force_gamechat = false;
			}
			break;
		}
		break;
	case Controls:
		// NULL
		break;
	}
}