#include "stdafx.h"
#include "Addresses.h"
#include "Structures.h"
#include <cstdio>
#include "Varibles.h"

DWORD R_AddCmdDrawTextInternal_p = 0, R_RegisterFont_p = 0, R_TextWidth_p = 0, rMaterial_RegisterHandle_p = 0, R_AddCmdDrawStretchPicInternal_p = 0, R_TextHeight_p = 0;
static int(*rMaterial_RegisterHandle)(const char *name, int imageTrack, bool errorIfMissing, int waitTime) = (int(__cdecl*)(const char*, int, bool, int))rMaterial_RegisterHandle_p;
static int(*R_RegisterFont)(const char * name, int imageTrack) = (int(__cdecl*)(const char*, int))R_RegisterFont_p;
static void(__cdecl *R_AddCmdDrawTextInternal)(const char* Text, int MaxCharacters, int Font, float X, float Y, float XScale, float YScale, float Angle, float* Color, int Style) = (void(__cdecl*)(const char*, int, int, float, float, float, float, float, float*, int))R_AddCmdDrawTextInternal_p;
static int(__cdecl*R_TextWidth)(int, const char *text, int maxchars, int font) = (int(__cdecl*)(int, const char*, int, int))R_TextWidth_p;
static void(__cdecl *R_AddCmdDrawStretchPicInternal)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, int) = (void(__cdecl*)(float, float, float, float, float, float, float, float, float*, int))R_AddCmdDrawStretchPicInternal_p;
static int(__cdecl *R_TextHeight)(int font) = (int(__cdecl*)(int))R_TextHeight_p;

void __DrawText(const char *text, float x, float y, const char *fontName, float scale, float* color)
{
	R_AddCmdDrawTextInternal(text, 0x7FFFFFFF, R_RegisterFont(fontName, 1), x, y, scale, scale, 0, color, 0);
}

int TextWidth(const char *text, const char *font)
{
	return R_TextWidth(0, text, 0x7FFFFFFF, R_RegisterFont(font, 0));
}

int TextHeight(const char* font)
{
	return R_TextHeight(R_RegisterFont(font, 0));
}

bool GetStructs()
{
	if (!*(int*)0x82BBC554 || !*(int*)0x82C70F4C || !*(int*)0x82BBAE68) return false;
	Centity_s = *(pCentity_t*)0x82BBC554;
	cgArray_s = *(cgArray_t**)0x82BBAE68;
	ClientActive = *(ClientActive_t**)0x82C70F4C;
	return true;
}

int Material_RegisterHandle(const char *name, int imgTrack) {
	return rMaterial_RegisterHandle(name, imgTrack, false, 0);
}

void DrawShader(float x, float y, float width, float height, float* color, const char *material)
{
	R_AddCmdDrawStretchPicInternal(x, y, width, height, 0, 0, 1, 1, color, Material_RegisterHandle(material, 0));
}

void SetTextCenteredWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, float *color, float *bcolor, bool bDrawShader = true)
{
	if (bDrawShader)
		DrawShader(((x - 2) - (TextWidth(text, font) / 2) * xScale), (y - (TextHeight(font) * xScale)), ((TextWidth(text, font) * xScale) + 4), TextHeight(font) * xScale, bcolor, "white");

	R_AddCmdDrawTextInternal(text, 0x7FFFFFFF, R_RegisterFont(font, 0), (x - (TextWidth(text, font) / 2) * xScale), y, xScale, yScale, 0, color, 0);
}


void DrawLine(float X1, float Y1, float X2, float Y2, float* Colors, int  Shader, float Width)
{
	float X, Y, Angle, L1, L2, H1;
	H1 = Y2 - Y1;
	L1 = X2 - X1;
	L2 = sqrt(L1 * L1 + H1 * H1);
	X = X1 + ((L1 - L2) / 2);
	Y = Y1 + (H1 / 2);
	Angle = (float)atan(H1 / L1) * (180 / 3.14159265358979323846);
	BO2_DrawRotated(0x82CBC168, X, Y, L2, Width, Angle, Colors, Shader);
}

bool isEntityPlayer(int i) {

	int cEnt = *(int *)(0x82BBC554 + (0 << 2));
	cEnt += (i * 0x374);

	if ((*(int *)(cEnt + 0x36C) & 0x40000000) == 0)// if the client is dead
		return false;

	if (*(short *)(cEnt + 0x2A8) != 1)
		return false;

	if (*(int *)(cEnt + 0x1D4) & 0x40000)
		return false;

	return (!cgArray_s->ClientInfo[i].Dead && strcmp(cgArray_s->ClientInfo[i].Name, "") && cgArray_s->ClientInfo[i].somethinghax && cgArray_s->Health > 0);
}


Vector2 RadarCenter, Center;

int RadarSize = 175;

Vector3 GetPlayerOrigin(int c) {
	return Centity_s[c].Origin;
}

int CG_GetEntity(int entNum, int mod = 0)
{
	return *(int*)0x82BBC554 + mod + (0x374 * entNum); // centity
}

int GetWeaponShader(int Weapon)
{
	return *(int *)(*(int *)(*(int *)(0x845CA998 + ((Weapon * 4) & 0x3FC)) + 0x08) + 0x660);
}

void _Radar()
{
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };

	Center.x = cgArray_s->RefDef.Width / 2;
	Center.y = cgArray_s->RefDef.Height / 2;
	RadarCenter.x = (Center.x * 2) - RadarSize - 40;
	RadarCenter.y = RadarSize / 2 + 25;

	DrawShader(RadarCenter.x - RadarSize, RadarCenter.y - RadarSize / 2, RadarSize * 2, RadarSize, OpusBG, "white");

	DrawLine(RadarCenter.x - RadarSize / 2, RadarCenter.y - RadarSize / 2, RadarCenter.x, RadarCenter.y, White, Material_RegisterHandle("white", 0), 1);
	DrawLine(RadarCenter.x + RadarSize / 2, RadarCenter.y - RadarSize / 2, RadarCenter.x, RadarCenter.y, White, Material_RegisterHandle("white", 0), 1);

	DrawShader(RadarCenter.x - RadarSize, RadarCenter.y, RadarSize * 2, 1, White, "white"); // horz
	DrawShader(RadarCenter.x, RadarCenter.y - RadarSize / 2, 1, RadarSize, White, "white"); // vert

	float healthHeight = cgArray_s->Health * 1.75;

	DrawShader(RadarCenter.x - RadarSize - 4, RadarCenter.y - RadarSize / 2, 2, 175, White, "white");
	DrawShader(RadarCenter.x - RadarSize - 4, (175 - healthHeight) + RadarCenter.y - RadarSize / 2, 2, healthHeight, flColor, "white");
	for (int i = 0; i < 1024; i++)
	{
		int Type = Centity_s[i].Type;

		if (Type != ET_PLAYER && Type != ET_MISSLE && Type != ET_ITEM)
			continue;

		int Shader;

		float *color = Red;
		if (Type == ET_PLAYER) {
			if (!isEntityPlayer(i))
				continue;
			Shader = Material_RegisterHandle("compassping_player", 0);
		}

		int RadarX, RadarY;
		RadarX = GetPlayerOrigin(cgArray_s->ClientNumber).x - GetPlayerOrigin(i).x;
		RadarY = GetPlayerOrigin(cgArray_s->ClientNumber).y - GetPlayerOrigin(i).y;

		float Angle = cgArray_s->ViewAngle.y / 180 * 3.141;

		float RotateX = RadarY * cosf(Angle) - RadarX * sinf(Angle);
		float RotateY = RadarX * cosf(Angle) + RadarY * sinf(Angle);

		float FinalX = RadarCenter.x + RotateX / _raderDistance;
		float FinalY = RadarCenter.y + RotateY / _raderDistance;

		if (FinalX < RadarCenter.x - RadarSize) FinalX = (RadarCenter.x - RadarSize) + 2;
		if (FinalY < RadarCenter.y - RadarSize / 2) FinalY = (RadarCenter.y - RadarSize / 2) + 2;

		if (FinalX > RadarCenter.x + RadarSize) FinalX = (RadarCenter.x + RadarSize) - 2;
		if (FinalY > RadarCenter.y + RadarSize / 2) FinalY = (RadarCenter.y + RadarSize / 2) - 2;

		if (Type == ET_MISSLE)
			DrawShader(FinalX - 2, FinalY - 2, 4, 4, color, "white");
		else
			BO2_DrawRotated(0x82CBC168, FinalX - 10, FinalY - 10, 20, 20, (i == cgArray_s->ClientNumber || Type == ET_ITEM) ? 0 : (cgArray_s->ViewAngle.y - cgArray_s->ClientInfo[i].viewAngle.y), color, Shader);
	}
}

Vector2 cCenter;

void _Compass()
{
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };

	cCenter.x = cgArray_s->RefDef.Width / 2;
	cCenter.y = cgArray_s->RefDef.Height / 2;

	for (int i = 0; i < 1024; i++)
	{
		int Type = Centity_s[i].Type;

		if (Type != ET_PLAYER && Type != ET_ITEM)
			continue;

		int Shader, dwWidth, dwHeight;

		float *color = compassColor;
		if (Type == ET_PLAYER) {
			if (!isEntityPlayer(i))
				continue;

			if (i == cgArray_s->ClientNumber)
				continue;

			dwWidth = 30; dwHeight = 30;

			Shader = Material_RegisterHandle("compassping_player", 0);
		}

		if (Type == ET_ITEM) {
			if ((Centity_s[i].State & (1 << 6)) == 0)
				continue;

			int WeaponID = (*(int*)CG_GetEntity(i, 0x2B4)) & 0xFF;

			if (WeaponID <= 0)
				continue;

			dwWidth = 40; dwHeight = 20;

			int dwTempShader = GetWeaponShader(WeaponID);
			if (dwTempShader)
				Shader = dwTempShader;

		}

		int RadarX, RadarY;

		RadarX = GetPlayerOrigin(cgArray_s->ClientNumber).x - GetPlayerOrigin(i).x;
		RadarY = GetPlayerOrigin(cgArray_s->ClientNumber).y - GetPlayerOrigin(i).y;

		float Angle = cgArray_s->ViewAngle.y / 180 * 3.141;

		float RotateX = RadarY * cos(Angle) - RadarX * sin(Angle);
		float RotateY = RadarX * cos(Angle) + RadarY * sin(Angle);

		float FinalX = cCenter.x + RotateX / _raderDistance;
		float FinalY = cCenter.y + RotateY / _raderDistance;

		float angle = atan(RotateX / RotateY) * (180 / M_PI);
		float FinalA = FinalY > cCenter.y ? -angle - 180.0f : -angle;

		if (FinalA < 0)
			FinalA += 360.0f;

		double radians = (M_PI / 180)*(FinalA - 90.0f);
		float CircleX = cCenter.x + (cos(radians) * _compassScale);
		float CircleY = cCenter.y + (sin(radians) * _compassScale);

		if (Shader)
			BO2_DrawRotated(0x82CBC168, CircleX - dwWidth / 2, CircleY - dwHeight / 2, dwWidth, dwHeight, FinalA, color, Shader);
	}
}

bool bInit = false;

bool BO2_wS2(float *flLocation, float *flOut)
{

	if (!bInit)
	{
		BO2_RefDef = (BO2_cRefdef*)BO2_RefDefCalc;

		bInit = true;
	}

	Vector vLocal;

	VectorSubtract(flLocation, BO2_RefDef->EyePos, vLocal);
	float flZ = DotProduct(BO2_RefDef->ViewAxis[0], vLocal);
	float flX = DotProduct(BO2_RefDef->ViewAxis[1], vLocal);
	float flY = DotProduct(BO2_RefDef->ViewAxis[2], vLocal);

	if (flZ >= 0.0f)
	{
		float flTmp = 1.0f / flZ;

		flOut[0] = (1.0f - (flX / BO2_RefDef->fov_X * flTmp)) * ((float)1280 / 2);
		flOut[1] = (1.0f - (flY / BO2_RefDef->fov_Y * flTmp)) * ((float)720 / 2);
		return true;
	}
	return false;
}

float GetDistance(D3DXVECTOR3 me, D3DXVECTOR3 enemi)
{
	float dx = me.x - enemi.x;
	float dy = me.y - enemi.y;
	float dz = me.z - enemi.z;
	return (float)sqrt((dx * dx) + (dy * dy) + (dz * dz))  * 0.03048f;
}

int __IsClientClose()
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

float __GetTextPosition(float PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 18;

	else if (PlayerDisctance <= 20)
		return 15;

	else if (PlayerDisctance <= 30)
		return 12;

	else if (PlayerDisctance <= 40)
		return 11;

	else if (PlayerDisctance <= 50)
		return 10;

	else if (PlayerDisctance <= 60)
		return 9;

	else if (PlayerDisctance <= 70)
		return 8;

	else if (PlayerDisctance <= 80)
		return 7;

	else if (PlayerDisctance <= 90)
		return 6;

	else if (PlayerDisctance <= 99.9999)
		return 5;

	else if (PlayerDisctance >= 100)
		return 5;
}

char *__Get_weapon(BYTE iD)
{
	int ids = (int)iD;
	switch (ids)
	{
	case 2:
		return "MP7";
	case 4:
		return "PDW-57";
	case 6:
		return "Vector K10";
	case 8:
		return "MSMC";
	case 10:
		return "Chicom CQB";
	case 12:
		return "Skorpion EVO";
	case 14:
		return "Peacekeeper";
	case 16:
		return "MTAR";
	case 20:
		return "Type 25";
	case 24:
		return "SWAT-556";
	case 28:
		return "FAL OSW";
	case 32:
		return "M27";
	case 36:
		return "SCAR-H";
	case 40:
		return "SMR";
	case 44:
		return "M8A1";
	case 48:
		return "AN-94";
	case 52:
		return "R870 MCS";
	case 53:
		return "S12";
	case 54:
		return "KSG";
	case 55:
		return "M1216";
	case 56:
		return "MK 48";
	case 58:
		return "QBB LSW";
	case 60:
		return "LSAT";
	case 62:
		return "HAMR";
	case 64:
		return "SVU-AS";
	case 65:
		return "DSR 50";
	case 66:
		return "Ballista";
	case 67:
		return "XPR50";
	case 68:
		return "KAP-40";
	case 70:
		return "Tac-45";
	case 72:
		return "Five Seven";
	case 74:
		return "Executor";
	case 76:
		return "B93R";
	case 78:
		return "Five Seven";
	case 79:
		return "Tac-45";
	case 80:
		return "B93R";
	case 81:
		return "Executor";
	case 82:
		return "Kard";
	case 83:
		return "M32";
	case 84:
		return "SMAW";
	case 85:
		return "FHJ-18 AA";
	case 86:
		return "RPG-7";
	case 87:
		return "Knife";
	case 88:
		return "Minigun";
	case 89:
		return "Riot Shield";
	case 90:
		return "Crossbow";
	case 91:
		return "Knife Ballist.";
	default:
		return "Unknown";
	}
}

float __get_text_pos_extra(float PlayerDisctance)
{
	if (PlayerDisctance <= 10)
		return 26;

	else if (PlayerDisctance <= 20)
		return 23;

	else if (PlayerDisctance <= 30)
		return 20;

	else if (PlayerDisctance <= 40)
		return 19;

	else if (PlayerDisctance <= 50)
		return 18;

	else if (PlayerDisctance <= 60)
		return 17;

	else if (PlayerDisctance <= 70)
		return 16;

	else if (PlayerDisctance <= 80)
		return 15;

	else if (PlayerDisctance <= 90)
		return 14;

	else if (PlayerDisctance <= 99.9999)
		return 13;

	else if (PlayerDisctance >= 100)
		return 13;
}

void MakeLines(float center[3], float x1, float y1, float z1, float x2, float y2, float z2, float * color, int Size = 1)
{
	float pointPos1[3] = { center[0] + x1, center[1] + y1, center[2] + z1 };
	float pointPos2[3] = { center[0] + x2, center[1] + y2, center[2] + z2 };
	float xy1[2], xy2[2];

	if (BO2_wS2(pointPos1, xy1) && BO2_wS2(pointPos2, xy2))
		DrawLine(xy1[0], xy1[1], xy2[0], xy2[1], color, Material_RegisterHandle("white", 0), Size);
}


void DrawBoundingBox(BO2_cEntity *pEntity, float W, float H, float sColor[4])
{
	MakeLines(pEntity->mPos, -W, W, 0, W, W, 0, sColor);
	MakeLines(pEntity->mPos, -W, W, 0, -W, W, H, sColor);
	MakeLines(pEntity->mPos, W, W, 0, W, W, H, sColor);
	MakeLines(pEntity->mPos, -W, W, H, W, W, H, sColor);

	MakeLines(pEntity->mPos, -W, W, 0, -W, -W, 0, sColor);
	MakeLines(pEntity->mPos, W, -W, 0, W, W, 0, sColor);
	MakeLines(pEntity->mPos, W, -W, 0, -W, -W, 0, sColor);
	MakeLines(pEntity->mPos, -W, -W, 0, -W, -W, H, sColor);

	MakeLines(pEntity->mPos, W, -W, 0, W, -W, H, sColor);
	MakeLines(pEntity->mPos, -W, W, H, -W, -W, H, sColor);
	MakeLines(pEntity->mPos, W, -W, H, W, W, H, sColor);
	MakeLines(pEntity->mPos, W, -W, H, -W, -W, H, sColor);
}

void DrawCorners(int x, int y, float fWidth, float fHeight, int ESPSize, float* color) {
	DrawShader((x - (fWidth / 2)), (y - fHeight), (fWidth / 4), ESPSize, color, "white");						// top left line
	DrawShader((x - (fWidth / 2)), y, (fWidth / 4), ESPSize, color, "white");									// bottom left line
	DrawShader((x - (fWidth / 2)), (y - fHeight), ESPSize, (fWidth / 4), color, "white");						// left top line
	DrawShader((x + (fWidth / 2)), (y - fHeight), ESPSize, (fWidth / 4), color, "white");						// right top line

	DrawShader((x + ((fWidth / 2) - (fWidth / 4))), (y - fHeight), (fWidth / 4), ESPSize, color, "white");	    // top right line
	DrawShader((x + ((fWidth / 2) - (fWidth / 4))), y, (fWidth / 4) + 1, ESPSize, color, "white");			    // bottom right line
	DrawShader((x - (fWidth / 2)), (y - (fWidth / 4)), ESPSize, (fWidth / 4), color, "white");				    // left bottom line
	DrawShader((x + (fWidth / 2)), (y - (fWidth / 4)), ESPSize, (fWidth / 4), color, "white");				    // right bottom line
}

void DrawTriBox(BO2_cEntity *pEntity, float W, float H, int ESPSize, float* color)
{
	W *= 1.5;
	H *= 1.25;

	//bottom
	MakeLines(pEntity->mPos, -W, -W, 0, W, -W, 0, color, ESPSize);
	MakeLines(pEntity->mPos, -W, -W, 0, -W, W, 0, color, ESPSize);
	MakeLines(pEntity->mPos, W, W, 0, W, -W, 0, color, ESPSize);
	MakeLines(pEntity->mPos, W, W, 0, -W, W, 0, color, ESPSize);

	//middle
	MakeLines(pEntity->mPos, -W, -W, 0, 0, 0, H, color, ESPSize);
	MakeLines(pEntity->mPos, -W, W, 0, 0, 0, H, color, ESPSize);
	MakeLines(pEntity->mPos, W, -W, 0, 0, 0, H, color, ESPSize);
	MakeLines(pEntity->mPos, W, W, 0, 0, 0, H, color, ESPSize);
}

void Draw3D(BO2_cEntity *pEntity, float W, float H, int ESPSize, float* sColor) {

	//bottom
	MakeLines(pEntity->mPos, -W, -W, 0, -W + (W / 2), -W, 0, sColor, ESPSize);
	MakeLines(pEntity->mPos, W / 2, -W, 0, W, -W, 0, sColor, ESPSize);

	MakeLines(pEntity->mPos, -W, -W, 0, -W, -W + (W / 2), 0, sColor, ESPSize);
	MakeLines(pEntity->mPos, -W, W - (W / 2), 0, -W, W, 0, sColor, ESPSize);

	MakeLines(pEntity->mPos, W, -W + (W / 2), 0, W, -W, 0, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, W - (W / 2), 0, W, W, 0, sColor, ESPSize);

	MakeLines(pEntity->mPos, -W + (W / 2), W, 0, -W, W, 0, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, W, 0, W - (W / 2), W, 0, sColor, ESPSize);

	//middle
	MakeLines(pEntity->mPos, -W, -W, 0, -W, -W, H / 6, sColor, ESPSize);
	MakeLines(pEntity->mPos, -W, -W, H, -W, -W, H - (H / 6), sColor, ESPSize);

	MakeLines(pEntity->mPos, -W, W, 0, -W, W, H / 6, sColor, ESPSize);
	MakeLines(pEntity->mPos, -W, W, H, -W, W, H - (H / 6), sColor, ESPSize);

	MakeLines(pEntity->mPos, W, -W, 0, W, -W, H / 6, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, -W, H, W, -W, H - (H / 6), sColor, ESPSize);

	MakeLines(pEntity->mPos, W, W, 0, W, W, H / 6, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, W, H, W, W, H - (H / 6), sColor, ESPSize);

	//top
	MakeLines(pEntity->mPos, -W, -W, H, -W + (W / 2), -W, H, sColor, ESPSize);
	MakeLines(pEntity->mPos, W / 2, -W, H, W, -W, H, sColor, ESPSize);

	MakeLines(pEntity->mPos, -W, -W, H, -W, -W + (W / 2), H, sColor, ESPSize);
	MakeLines(pEntity->mPos, -W, W - (W / 2), H, -W, W, H, sColor, ESPSize);

	MakeLines(pEntity->mPos, W, -W + (W / 2), H, W, -W, H, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, W - (W / 2), H, W, W, H, sColor, ESPSize);

	MakeLines(pEntity->mPos, -W + (W / 2), W, H, -W, W, H, sColor, ESPSize);
	MakeLines(pEntity->mPos, W, W, H, W - (W / 2), W, H, sColor, ESPSize);
}

void DrawFaded(int x, int y, int size, int fWidth, int fHeight, float* border_color, float* fadeColor) {
	DrawShader((x - (fWidth / 2)), (y - fHeight), fWidth, fHeight, fadeColor, "white"); //top

	DrawShader((x - (fWidth / 2)), (y - fHeight), fWidth, size, border_color, "white"); // top line
	DrawShader((x - (fWidth / 2)), y, fWidth, size, border_color, "white"); // bottom line
	DrawShader((x - (fWidth / 2)), (y - fHeight), size, fHeight, border_color, "white"); // left line
	DrawShader((x + (fWidth / 2)), (y - fHeight), size, fHeight + size, border_color, "white"); // right line
}

void Draw2D(int x, int y, int size, int fWidth, int fHeight, float* sColor) {
	DrawShader((x - (fWidth / 2)), (y - fHeight), fWidth, size, sColor, "white"); // top line
	DrawShader((x - (fWidth / 2)), y, fWidth, size, sColor, "white"); // bottom line
	DrawShader((x - (fWidth / 2)), (y - fHeight), size, fHeight, sColor, "white"); // left line
	DrawShader((x + (fWidth / 2)), (y - fHeight), size, fHeight + size, sColor, "white"); // right line
}

typedef int(*BO2_RTGBN) (char* szName);
BO2_RTGBN BO2_GetTagByName = (BO2_RTGBN)0x823599E0;

bool BO2_GetTagPosReg(BO2_cEntity* pEnt, char* szTag, float* flOut) {
	WORD wTag = BO2_GetTagByName(szTag);
	if (!wTag)
		return false;

	BO2_GetTagPos(pEnt, wTag, flOut);

	return true;
}

void __drawLines_extra(int x1, int y1, int x2, int y2, const float* color, int shader) {
	DWORD ptr = 0x82CBC168;

	float x, y, angle, l1, l2, h1;
	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1)* (180 / 3.14159265358979323846);
	BO2_DrawRotated(ptr, x, y, l2, 1, angle, color, shader);
}

void __ExternalESP() {
	__readGame();
	char cNum[200];
	char count[250];
	int CenterX = 1280 / 2;
	int CenterY = 720 / 2;
	bool isFriend = false;
	Vector vScreen;
	Vector vScreenClient;
	int ClientValid = __IsClientClose();
	BO2_cEntity *pEntityValid = BO2_Entities[ClientValid];
	int playercount = 0;
	float power = 1.0;
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	float _espGreen[4] = { (float)128 / 255.0f,(float)173 / 255.0f,(float)102 / 255.0f,(float)255 / 255.0f };
	float _espRed[4] = { (float)194 / 255.0f,(float)66 / 255.0f,(float)66 / 255.0f,(float)255 / 255.0f };
	float _espYellowTrans[4] = { (float)207 / 255.0f,(float)184 / 255.0f,(float)37 / 255.0f,(float)255 / 255.0f };
	float _espYellowTransparent[4] = { (float)207 / 255.0f,(float)184 / 255.0f,(float)37 / 255.0f,(float)120 / 255.0f };
	for (int i = 0; i < 12; i++)
	{
		if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
		{
			playercount++;
			BO2_cEntity *pEntity = BO2_Entities[i];

			float DistancePlayer = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

			if (BO2_wS2(pEntity->mPos, vScreen))
			{
				if (DistancePlayer > 3)
				{
					int x = vScreen[0];
					int y = vScreen[1];

					float fWidth = (60.0f * 15.0f) / DistancePlayer;
					float fHeight = (100.0f * 15.0f) / DistancePlayer;
					bool bVisible = (BO2_CanSeeFriendlyHead(0, BO2_Entities[i], 0x280F803));
					int newX = x + (fWidth / 2) + 1;
					int newY = y - (fHeight);

					D3DXVECTOR3 vHead;
					short mTag = *(short*)0x836C5C92;
					BO2_GetTagPos(BO2_Entities[i], mTag, vHead);
					D3DXVECTOR3 BoxPos;
					BO2_wS2(vHead, BoxPos);
					int HeadX = BoxPos.x;
					int HeadY = BoxPos.y;
					if (vars.enable_esp) {
						if ((BO2_Clients[i]->mTeam == BO2_Clients[BO2_cg->cNum]->mTeam) && (strcmp(BO2_cgs->gametype, "dm") != 0) && (strcmp(BO2_cgs->gametype, "gun") != 0) && (strcmp(BO2_cgs->gametype, "oic") != 0) && (strcmp(BO2_cgs->gametype, "shrp") != 0) && (strcmp(BO2_cgs->gametype, "sas") != 0))
						{
							if (vars._TeamView) {
								if (vars.name_esp) {
									char clientdetails[256];
									sprintf(clientdetails, "%s [TEAM]", BO2_Clients[BO2_Clients[i]->mNum]->mName);
									SetTextCenteredWithBackGround(clientdetails, "fonts/720/normalFont", HeadX - strlen(clientdetails) * 2.5 + 6 + 35, HeadY - __GetTextPosition(DistancePlayer), 0.4, 0.4, _espGreen, OpusBG, true);
								}
								
								char clientweapon[256];
								if (vars.weapon_names_esp) {
									sprintf(clientweapon, "%s", __Get_weapon(BO2_Entities[i]->WeaponID));
									SetTextCenteredWithBackGround(clientweapon, "fonts/720/normalFont", x - strlen(clientweapon) * 2.5 + 6 + 17, y + __get_text_pos_extra(DistancePlayer), 0.4, 0.4, _espGreen, OpusBG, true);
								}
								
								if (vars.weapon_icon_esp) {
									int Shader = GetWeaponShader(Centity_s[i].WeaponID);
									if (Shader)
										BO2_DrawRotated(0x82CBC168, x - strlen(clientweapon) * 2.5 + 6, y + __get_text_pos_extra(DistancePlayer), 60, 30, 0, White, Shader);
								}

								if (vars.distance_esp) {
									char __distanceBuf[256];
									sprintf(__distanceBuf, "%.fM", GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									SetTextCenteredWithBackGround(__distanceBuf, "fonts/720/normalFont", HeadX - strlen(__distanceBuf) * 2.5 - 32 + 40, HeadY - __GetTextPosition(DistancePlayer) - 20.0f, 0.4, 0.4, _espGreen, OpusBG, true);
								}

								if (vars.jordan_esp) {
									DrawBoundingBox(BO2_Entities[i], 8, 13, _espYellowTrans); // jordans

									if (Centity_s[i].Type == ET_MISSLE)
									{
										if (!(Centity_s[i].State & (1 << 6)))
											continue;

										DrawBoundingBox(BO2_Entities[i], 8, 13, _espYellowTrans); // jordans on a weapon (dont ask)
									}
								}

								DrawLine(x, y, cgArray_s->RefDef.Width / 2, cgArray_s->RefDef.Height - 30, color, Material_RegisterHandle("white", 0), 1); // tracers

								if (vars._3DEsp) {
									Draw3D(BO2_Entities[i], 20, 60, _TeamESPSize, _TeamColor); // 3D Box
								}
								else if (vars._2DEsp) {
									Draw2D(x, y, _TeamESPSize, fWidth, fHeight, _espGreen); // 2D Box
								}
								else if (vars._SHADEDEsp) {
									DrawFaded(x, y, _TeamESPSize, fWidth, fHeight, _espGreen, _espYellowTransparent); // shaded
								}
								else if (vars._TRIEsp) {
									DrawTriBox(BO2_Entities[i], 20, 60, _TeamESPSize, _TeamColor); // tri
								}
								else if (vars._CORNEREsp) {
									DrawCorners(x, y, fWidth, fHeight, _TeamESPSize, _TeamColor); // corners
								}
							}
						}
						else
						{
							if (vars._EnemyView) {
								if (vars.name_esp) {
									char clientdetails[256];
									sprintf(clientdetails, "%s [ENEMY]", BO2_Clients[BO2_Clients[i]->mNum]->mName);
									SetTextCenteredWithBackGround(clientdetails, "fonts/720/normalFont", HeadX - strlen(clientdetails) * 2.5 + 6 + 35, HeadY - __GetTextPosition(DistancePlayer), 0.4, 0.4, _espRed, OpusBG, true);
								}

								char clientweapon[256];
								if (vars.weapon_names_esp) {
									sprintf(clientweapon, "%s", __Get_weapon(BO2_Entities[i]->WeaponID));
									SetTextCenteredWithBackGround(clientweapon, "fonts/720/normalFont", x - strlen(clientweapon) * 2.5 + 6 + 17, y + __get_text_pos_extra(DistancePlayer), 0.4, 0.4, _espRed, OpusBG, true);
								}

								if (vars.weapon_icon_esp) {
									int Shader = GetWeaponShader(Centity_s[i].WeaponID);
									if (Shader)
										BO2_DrawRotated(0x82CBC168, x - strlen(clientweapon) * 2.5 + 6, y + __get_text_pos_extra(DistancePlayer), 60, 30, 0, White, Shader);
								}

								if (vars.distance_esp) {
									char __distanceBuf[256];
									sprintf(__distanceBuf, "%.fM", GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos));
									SetTextCenteredWithBackGround(__distanceBuf, "fonts/720/normalFont", HeadX - strlen(__distanceBuf) * 2.5 - 32 + 40, HeadY - __GetTextPosition(DistancePlayer) - 20.0f, 0.4, 0.4, _espRed, OpusBG, true);
								}

								if (vars.jordan_esp) {
									DrawBoundingBox(BO2_Entities[i], 8, 13, _EnemyColor);

									if (Centity_s[i].Type == ET_MISSLE)
									{
										if (!(Centity_s[i].State & (1 << 6)))
											continue;

										DrawBoundingBox(BO2_Entities[i], 8, 13, _EnemyColor);
									}
								}

								DrawLine(x, y, cgArray_s->RefDef.Width / 2, cgArray_s->RefDef.Height - 30, color, Material_RegisterHandle("white", 0), 1); // tracers

								if (vars._3DEsp) {
									Draw3D(BO2_Entities[i], 20, 60,  _EnemyESPSize, _EnemyColor); // 3D Box
								}
								else if (vars._2DEsp) {
									Draw2D(x, y, _EnemyESPSize, fWidth, fHeight, _espRed); // 2D Box
								}
								else if (vars._SHADEDEsp) {
									DrawFaded(x, y, _EnemyESPSize, fWidth, fHeight, _espRed, _espYellowTransparent); // shaded
								}
								else if (vars._TRIEsp) {
									DrawTriBox(BO2_Entities[i], 20, 60, _EnemyESPSize, _EnemyColor); // tri
								}
								else if (vars._CORNEREsp) {
									DrawCorners(x, y, fWidth, fHeight, _EnemyESPSize, _EnemyColor); // corners
								}
							}
						}
					}
				}
			}
		}
	}
}