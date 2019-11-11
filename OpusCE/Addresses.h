#pragma once
#define FONT_SMALL_DEV		"fonts/smallDevFont"
#define FONT_BIG_DEV		"fonts/bigDevFont"
#define FONT_CONSOLE		"fonts/consoleFont"
#define FONT_BIG			"fonts/bigFont"
#define FONT_SMALL			"fonts/smallFont"
#define FONT_BOLD			"fonts/boldFont"
#define FONT_NORMAL			"fonts/normalFont"
#define FONT_EXTRA_BIG		"fonts/extraBigFont"
#define FONT_OBJECTIVE		"fonts/objectiveFont"

typedef void(_cdecl *R_AddCmdDrawStretchPic_t)(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, const float *color, void *material);
typedef void(__cdecl *UI_FillRectPhysical_t)(float x, float y, float width, float height, const float *color);
typedef void(__cdecl *R_AddCmdDrawText_t)(const char *text, int maxChars, void *font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
typedef void(__cdecl *_R_AddCmdDrawText_t)(const char *text, int maxChars, DWORD font, float x, float y, float xScale, float yScale, float rotation, const float *color, int style);
typedef void *(*Material_RegisterHandle_t)(const char *name, int imageTrack, bool, int);
typedef void(__cdecl *Cbuf_AddText_t)(int localClientNum, const char *text);
typedef bool(*Dvar_GetBool_t)(const char *dvarName);
typedef void(__cdecl *Cbuf_AddTextBO2_t)(int localClientNum, const char *text); 
typedef void(__cdecl *R_RenderScene_t)(DWORD refdef);
/*int(*CG_StrWidth)(int a1,char *szString,int iMaxLen,void *pFont)=(int(__cdecl*)(int,char*,int,void*))0x714AD0;*/
class Addresses {
public:
	static R_AddCmdDrawStretchPic_t R_AddCmdDrawStretchPic;
	static UI_FillRectPhysical_t drawShader;
	static R_AddCmdDrawText_t R_AddCmdDrawText;
	static _R_AddCmdDrawText_t _R_AddCmdDrawText;
	static void*(*R_RegisterFont)(const char * font, int imageTrack);
	static Material_RegisterHandle_t Material_RegisterHandle;
	static Cbuf_AddText_t Cbuf_AddText;
	static Dvar_GetBool_t Dvar_GetBooll;
	static BOOL(*Dvar_GetBool)(CONST PCHAR Dvar);
	static R_RenderScene_t R_RenderScene_;
	//static BO2_CanSeeFriendlyHead_ BO2_CanSeeFriendlyHead;
	//static BO2_GetTagPos_t BO2_GetTagPos;
};

R_AddCmdDrawStretchPic_t Addresses::R_AddCmdDrawStretchPic = (R_AddCmdDrawStretchPic_t)0x828B86C0; // TU18
UI_FillRectPhysical_t Addresses::drawShader = (UI_FillRectPhysical_t)0x82444160; // TU18	
R_AddCmdDrawText_t Addresses::R_AddCmdDrawText = (R_AddCmdDrawText_t)0x828B8BA0;
_R_AddCmdDrawText_t Addresses::_R_AddCmdDrawText = (_R_AddCmdDrawText_t)0x828B8BA0;
void*(*Addresses::R_RegisterFont)(const char * font, int imageTrack) = (void*(*)(const char *, int))0x82275F78; // TU18 //0x82275F78
Material_RegisterHandle_t Addresses::Material_RegisterHandle = (Material_RegisterHandle_t)0x828B78F0; // TU18
Cbuf_AddText_t Addresses::Cbuf_AddText = (Cbuf_AddText_t)0x824015E0; // TU18
Dvar_GetBool_t Addresses::Dvar_GetBooll = (Dvar_GetBool_t)0x8231C1D8; // MW3 TU23 0x8232E200
BOOL(*Addresses::Dvar_GetBool)(CONST PCHAR Dvar) = (BOOL(*)(CONST PCHAR))0x82461610;
R_RenderScene_t Addresses::R_RenderScene_ = (R_RenderScene_t)0x828AF5A0;
DWORD BO2_RefDefCalc = NULL;


