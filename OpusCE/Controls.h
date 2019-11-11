#include "ESP.h"
#include "Aimbot.h"

int rgbStage = 0;

void RGB_FADE() {
	if (rgbStage == 0)
	{
		BannerColorRGB.B++;
		if (BannerColorRGB.B == 255)
			rgbStage = 1;
	}
	else if (rgbStage == 1)
	{
		BannerColorRGB.R--;
		if (BannerColorRGB.R == 0)
			rgbStage = 2;
	}
	else if (rgbStage == 2)
	{
		BannerColorRGB.G++;
		if (BannerColorRGB.G == 255)
			rgbStage = 3;
	}
	else if (rgbStage == 3)
	{
		BannerColorRGB.B--;
		if (BannerColorRGB.B == 0)
			rgbStage = 4;
	}
	else if (rgbStage == 4)
	{
		BannerColorRGB.R++;
		if (BannerColorRGB.R == 255)
			rgbStage = 5;
	}
	else if (rgbStage == 5)
	{
		BannerColorRGB.G--;
		if (BannerColorRGB.G == 0)
			rgbStage = 0;
	}
}

void _HealthBar() {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	float _green[4] = { (float)47 / 255.0f, (float)154 / 255.0f, (float)57 / 255.0f, (float)255 / 255.0f };
	float _red[4] = { (float)192 / 255.0f, (float)15 / 255.0f, (float)15 / 255.0f, (float)130 / 255.0f };
	float _dark[4] = { (float)71 / 255.0f, (float)79 / 255.0f, (float)72 / 255.0f, (float)255 / 255.0f };
	float healthHeight = cgArray_s->Health * 1.75;
	DrawShader(543.0f, 663.0f, 179.0f, 19.0f, _dark, "white");
	DrawShader(545.0f, 665.0f, 175.0f, 15.0f, _red, "white");
	DrawShader((175 - healthHeight) + 545.0f, 665.0f, healthHeight, 15.0f, _green, "white");
	SetTextCenteredWithBackGround("Health bar", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 611.0f + 25, 665.0f, 0.65, 0.65, flColor, OpusBG, false);
}

int ClosetsClientBO2()
{
	__readGame();
	float objectDistance = 0.f;
	float flClosestDistance = (float)INT_MAX;
	int ClosestClient = -1;

	for (int i = 0; i < 12; i++)
	{
		if (i == BO2_cg->cNum)
			continue;

		if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
		{
			if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
			{
				objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

				if (objectDistance < flClosestDistance)
				{
					flClosestDistance = objectDistance;
					ClosestClient = i;
				}
			}
			else
			{
				if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
				{
					objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

					if (objectDistance < flClosestDistance)
					{
						flClosestDistance = objectDistance;
						ClosestClient = i;
					}
				}
			}
		}
	}

	return ClosestClient;
}

void __target_details() {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	int ClientValid = ClosetsClientBO2();
	if (vars.enable_radar) {
		if (vars.enable_target_details) {
			char GamerTagBuffer[256];
			sprintf(GamerTagBuffer, "GT: %s", BO2_Clients[BO2_Clients[ClientValid]->mNum]->mName);
			SetTextCenteredWithBackGround("Target Details", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 48, 310 + 7, 0.5, 0.5, flColor, OpusBG, true);
			SetTextCenteredWithBackGround(GamerTagBuffer, "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth(GamerTagBuffer, "fonts/720/normalFont") * 0.5 - 50 + 1 + 27, 330 + 7, 0.5, 0.5, flColor, OpusBG, true);
			vars.enable_target_details = false;
		}
		else {
			SetTextCenteredWithBackGround("Target Details", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 48, 310 + 7, 0.5, 0.5, flColor, OpusBG, true);
			SetTextCenteredWithBackGround("N/A", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("N/A", "fonts/720/normalFont") * 0.5 - 50 + 1 + 27, 330 + 7, 0.5, 0.5, flColor, OpusBG, true);
		}	
	}
	else {
		if (vars.enable_target_details) {
			char GamerTagBuffer[256];
			sprintf(GamerTagBuffer, "GT: %s", BO2_Clients[BO2_Clients[ClientValid]->mNum]->mName);
			SetTextCenteredWithBackGround("Target Details", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 48, 310 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
			SetTextCenteredWithBackGround(GamerTagBuffer, "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth(GamerTagBuffer, "fonts/720/normalFont") * 0.5 - 50 + 1 + 27, 330 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
			vars.enable_target_details = false;
		}
		else {
			SetTextCenteredWithBackGround("Target Details", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 48, 310 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
			SetTextCenteredWithBackGround("N/A", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("N/A", "fonts/720/normalFont") * 0.5 - 50 + 1 + 27, 330 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
		}
	}
}

void _ExternalESP() {
	if (!GetStructs())
		return;

	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };

	R_TextHeight_p = 0x82490390;
	R_TextHeight = (int(__cdecl*)(int))R_TextHeight_p;

	R_AddCmdDrawStretchPicInternal_p = 0x828B86C0;
	R_AddCmdDrawStretchPicInternal = (void(__cdecl*)(float, float, float, float, float, float, float, float, float*, int))R_AddCmdDrawStretchPicInternal_p;

	rMaterial_RegisterHandle_p = 0x828B78F0;
	rMaterial_RegisterHandle = (int(__cdecl*)(const char*, int, bool, int))0x828B78F0;

	R_TextWidth_p = 0x828B6FD8;
	R_TextWidth = (int(__cdecl*)(int, const char*, int, int))R_TextWidth_p;

	R_RegisterFont_p = 0x828B6EC0;
	R_RegisterFont = (int(__cdecl*)(const char*, int))R_RegisterFont_p;

	R_AddCmdDrawTextInternal_p = 0x828B8BA0;
	R_AddCmdDrawTextInternal = (void(__cdecl*)(const char*, int, int, float, float, float, float, float, float*, int))R_AddCmdDrawTextInternal_p;

	int res_1 = 1280;
	int res_2 = 720;
	char __buf[255];
	if (vars.enable_radar) {
		_Radar();
		if (vars.debug_info) {
			SetTextCenteredWithBackGround("CrypticPro", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 49, 250 + 7, 0.5, 0.5, flColor, OpusBG, true);
			sprintf(__buf, "Screen Resolution: %ix%i", res_1, res_2);
			SetTextCenteredWithBackGround(__buf, "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth(__buf, "fonts/720/normalFont") * 0.5 - 50 + 30 + 48, 270 + 7, 0.5, 0.5, flColor, OpusBG, true);
		}
	}
	else {
		if (vars.debug_info) {
			SetTextCenteredWithBackGround("CrypticPro", "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth("Target Details", "fonts/720/normalFont") * 0.5 - 50 + 1 + 49, 250 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
			sprintf(__buf, "Screen Resolution: %ix%i", res_1, res_2);
			SetTextCenteredWithBackGround(__buf, "fonts/720/normalFont", cgArray_s->RefDef.Width - TextWidth(__buf, "fonts/720/normalFont") * 0.5 - 50 + 30 + 48, 270 + 7 - 220, 0.5, 0.5, flColor, OpusBG, true);
		}
	}

	__rgbCol[0] = (float)BannerColorRGB.R / 255.0f;
	__rgbCol[1] = (float)BannerColorRGB.G / 255.0f;
	__rgbCol[2] = (float)BannerColorRGB.B / 255.0f;
	__rgbCol[3] = (float)255 / 255.0f;

	if (vars.enable_esp)
		__ExternalESP();

	if(vars.enable_compass)
		_Compass();

	if(vars.health_bar)
		_HealthBar();

	if(vars.target_details_)
		__target_details();
}

void _aimbot()
{
	printf("aimbot called\n");
	GetStructs();

	Usercmd_t* cur = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber) & 0x7F];
	Usercmd_t* newCmd = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber) & 0x7F];
	Usercmd_t* old = &ClientActive->Usercmd[(ClientActive->CurrentCmdNumber - 1) & 0x7F];

	memcpy(old, cur, sizeof(Usercmd_t));
	old->time -= 1;

	if (vars.nospread)
		_spreadFix(old);

	printf("aimbot done\n");
}

HRESULT CL_RenderScenehk(DWORD a1)
{
	Addresses::R_RenderScene_(a1);
	BO2_RefDefCalc = a1;
	if (Addresses::Dvar_GetBool("cl_ingame")) {
		watermark();
		//_MapOutline();
		RGB_FADE();
		_aimbot();
		_ExternalESP();
		__Aimbot();
     	Nearest();
		fakelag();
	//	crosshairs();
		if (vars.Menuopen) { LoadBO2Menu(); }
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		XInputGetState(0, &state);
		vars.ticktock += 1;
		if (vars.ticktock > 20)
		{
			if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_DPAD_LEFT)) {
				resetVars();
			}
			else if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_DPAD_UP) && vars.Menuopen) {
				vars.scroll--;
				if (vars.scroll < 0) vars.scroll = vars.maxScroll;
				vars.ticktock = 0;
			}
			else if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_DPAD_DOWN) && vars.Menuopen) {
				vars.scroll++;
				if (vars.scroll > vars.maxScroll) vars.scroll = 0;
				vars.ticktock = 0;
			}
			else if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_LEFT_SHOULDER)) {
				vars.tabs--;
				if (vars.tabs < 0) vars.tabs = vars.maxTabScroll;
				switchTabMenu();
				vars.ticktock = 0;
			}
			else if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
				vars.tabs++;
				if (vars.tabs > vars.maxTabScroll) vars.tabs = 0;
				switchTabMenu();
				vars.ticktock = 0;
			}
			else if (state.Gamepad.wButtons == (XINPUT_GAMEPAD_A) && vars.Menuopen) {
				_executeMenu();
				vars.ticktock = 0;
			}
		}
	}
	else {
		// pre game space 
	}

	return S_OK;
}