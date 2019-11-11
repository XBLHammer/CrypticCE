#include "Addresses.h"
#include "Structures.h"
#include <cstdio>

struct RGB
{
	int R, G, B;
};
RGB BannerColorRGB = { 181,0,0 };

VOID DrawText(const char *xtext, float x, float y, const char *xfont, float xfontSize, float* color) {
	Addresses::R_AddCmdDrawText(xtext, 0x7FFFFFFF, Addresses::R_RegisterFont(xfont, 0), x, y, xfontSize, xfontSize, 0, White, 0);
}

DWORD _BIG_FONT = 0x82FC3378;

void _DrawText(const char* text, float x, float y, float xScale, const float *color) {
	Addresses::_R_AddCmdDrawText(text, 0x7FFFFFFF, _BIG_FONT, x, y, xScale, xScale, 0, color, 0);
}

void DrawPicture(float x, float y, float width, float height, char *material, const float *color) {
	Addresses::R_AddCmdDrawStretchPic(x, y, width, height, 0, 0, 1, 1, color, Addresses::Material_RegisterHandle(material, 0, false, 0));
}


void drawNumberedShader(int shaderID, float x, float y, float width, float height, float* color) {
	if (vars.scroll == shaderID) { //current scroll
		Addresses::R_AddCmdDrawStretchPic(x, y, width, height, 1, 1, 1, 1, scrollcolor, Addresses::Material_RegisterHandle("white", 0, false, 0));
	}
	else Addresses::R_AddCmdDrawStretchPic(x, y, width, height, 1, 1, 1, 1, color, Addresses::Material_RegisterHandle("white", 0, false, 0));
}
void drawNumberedText(int textID, const char *xtext, float xx, float xy, const char *xfont, float xfontSize, float* color) {
	if (vars.lrscroll == textID) {
		Addresses::R_AddCmdDrawText(xtext, 0x7FFFFFFF, Addresses::R_RegisterFont(xfont, 0), xx, xy, xfontSize, xfontSize, 0, Black, 0);
	}
	else Addresses::R_AddCmdDrawText(xtext, 0x7FFFFFFF, Addresses::R_RegisterFont(xfont, 0), xx, xy, xfontSize, xfontSize, 0, color, 0);
}

/**
** brief addOption information **
*
** add options is a function which allows us to basically create, well.. 
** 'Options'.

** Parameters information:
** param text | what string you want to display for the option
** param textindex | what number the option is
**param secondIndex | whenever the textIndex is above '8' start adding from 0 - 8 on this row. 
*
**/
void addOption(char* text, int textindex, int secondIndex, int additionalIndexIncrease, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textindex) {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);

			//scroller
			Addresses::drawShader(713 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 280 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // bottom

			//scroll bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * secondIndex), 3, 26, flColor);
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textindex), 0.25f, White);

			//scroller
			Addresses::drawShader(713, 260 + (additionalIndexIncrease * textindex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818, 260 + (additionalIndexIncrease * textindex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (additionalIndexIncrease * textindex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 280 + (additionalIndexIncrease * textindex), 104, 0.9f, flColor); // bottom

			//scroller bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * textindex), 3, 26, flColor);
		}
	}
	else {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textindex), 0.25f, White);
		}
	}
}


void changeSubmenu(submenus menu, int optionCount, submenus prevSub) {
	vars.submenu = menu;
	vars.scroll = 0;
	vars.maxScroll = optionCount;
	vars.presub = prevSub;
}

void __setupMenu(submenus menu, tabMenus currentTab, int tabs, int maxTabScroll, int scroller, int maxScroll, submenus preSub) {
	vars.submenu = menu;
	vars.currentTab = currentTab;
	vars.tabs = tabs;
	vars.maxTabScroll = maxTabScroll;
	vars.scroll = scroller;
	vars.maxScroll = maxScroll;
	vars.presub = preSub;
	vars.Menuopen = true;
	vars.RGB = true;
}

void addTab(char* text, int tabindex, float x) {
	if (vars.tabs == tabindex) {
		float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
		Addresses::drawShader(611, 288 + (23 * tabindex), 89, 15, OpusBG);
		_DrawText(text, 627.0f, 305 + (23 * tabindex), 0.4f, flColor);
	}
	else {
		_DrawText(text, 627.0f, 305 + (23 * tabindex), 0.4f, White);
	}
}


void addBool(char* text, int textIndex, int secondIndex, int thirdIndex, bool optionBool, int additionalIndexIncrease, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textIndex) { //current scroll
		if (textIndex > 17) {
			_DrawText(text, 719.0f + 120 + 120, 277.0f + (additionalIndexIncrease * thirdIndex), 0.25f, White);

			/*
			scroller
			*/
			Addresses::drawShader(713 + 120 + 120, 260 + (additionalIndexIncrease * thirdIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818 + 120 + 120, 260 + (additionalIndexIncrease * thirdIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f + 120 + 120, 260 + (additionalIndexIncrease * thirdIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120 + 120, 280 + (additionalIndexIncrease * thirdIndex), 104, 0.9f, flColor); // bottom
			if (!optionBool) {
				// off
				Addresses::drawShader(800 + 120 + 120, 263 + (additionalIndexIncrease * thirdIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800 + 120 + 120, 263 + (additionalIndexIncrease * thirdIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802 + 120 + 120, 265 + (additionalIndexIncrease * thirdIndex), 11, 11, flColor); // on overlay
			}
			//scroller bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * thirdIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * thirdIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * thirdIndex), 3, 26, flColor);
		}
		else if (textIndex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);
			
			/*
			scroller
			*/
			Addresses::drawShader(713 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 280 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // bottom
			if (!optionBool) {
				// off
				Addresses::drawShader(800 + 120, 263 + (additionalIndexIncrease * secondIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800 + 120, 263 + (additionalIndexIncrease * secondIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802 + 120, 265 + (additionalIndexIncrease * secondIndex), 11, 11, flColor); // on overlay
			}
			//scroller bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * secondIndex), 3, 26, flColor);
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textIndex), 0.25f, White);

			/*
			scroller
			*/
			Addresses::drawShader(713, 260 + (additionalIndexIncrease * textIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818, 260 + (additionalIndexIncrease * textIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (additionalIndexIncrease * textIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 280 + (additionalIndexIncrease * textIndex), 104, 0.9f, flColor); // bottom
			if (!optionBool) {
				// off
				Addresses::drawShader(800, 263 + (additionalIndexIncrease * textIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800, 263 + (additionalIndexIncrease * textIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802, 265 + (additionalIndexIncrease * textIndex), 11, 11, flColor); // on overlay
			}

			//scroller bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * textIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * textIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * textIndex), 3, 26, flColor);
		}
	}
	else {
		if (textIndex > 17) {
			_DrawText(text, 719.0f + 120 + 120, 277.0f + (additionalIndexIncrease * thirdIndex), 0.25f, White);
			if (!optionBool) {
				// off
				Addresses::drawShader(800 + 120 + 120, 263 + (additionalIndexIncrease * thirdIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800 + 120 + 120, 263 + (additionalIndexIncrease * thirdIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802 + 120 + 120, 265 + (additionalIndexIncrease * thirdIndex), 11, 11, flColor); // on overlay
			}
		}
		else if (textIndex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);
			if (!optionBool) {
				// off
				Addresses::drawShader(800 + 120, 263 + (additionalIndexIncrease * secondIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800 + 120, 263 + (additionalIndexIncrease * secondIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802 + 120, 265 + (additionalIndexIncrease * secondIndex), 11, 11, flColor); // on overlay
			}
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textIndex), 0.25f, White);
			if (!optionBool) {
				// off
				Addresses::drawShader(800, 263 + (additionalIndexIncrease * textIndex), 15, 15, dark_OpusBG); // bg
			}
			else {
				// on
				Addresses::drawShader(800, 263 + (additionalIndexIncrease * textIndex), 15, 15, dark_OpusBG); // bg
				Addresses::drawShader(802, 265 + (additionalIndexIncrease * textIndex), 11, 11, flColor); // on overlay
			}
		}
	}
}

void addchar(char* text, int textindex, char* optionchar, float x)
{
	if (vars.scroll == textindex) {
		DrawText(text, x, 337 + (26 * textindex), FONT_BOLD, 1, White);
		DrawText(optionchar, x + 115, 337 + (26 * textindex), FONT_BOLD, 1, White);
	}
	else {
		DrawText(text, x, 337 + (26 * textindex), FONT_BOLD, 0.9, White);
		DrawText(optionchar, x + 115, 337 + (26 * textindex), FONT_BOLD, 0.9, White);
	}
}

// beta char option /* not using anymore */
void esp_charOption(char* text, int textindex, int secondIndex, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textindex) {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.3f, White);

			//scroller
			Addresses::drawShader(713 + 120, 260 + (26 * secondIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (26 * secondIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (26 * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 280 + (26 * secondIndex), 104, 0.9f, flColor); // bottom
			
			//scroll bar
			Addresses::drawShader(1069, 251 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * secondIndex), 3, 26, flColor);

			// bar at the bottom
			Addresses::drawShader(706, 541, 359, 11, OpusBG);
			Addresses::drawShader(706, 555, 359, 17, OpusBG);
			Addresses::drawShader(706, 574, 359, 2, flColor);

			_DrawText("Select your ESP Type!", 848.0f, 551.5f, 0.25f, White);

			switch (charCount_esp) {
			case 1:
				// test draws
				_DrawText("Filled", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Triangles", 862.0f, 570.5f, 0.3f, flColor);
				_DrawText("Rectangles", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Triangles;
				break;
			case 2:
				// test draws
				_DrawText("Triangles", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Rectangles", 862.0f, 570.5f, 0.3f, flColor);
				_DrawText("Filled", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Rectangles;
				break;
			case 3:
				// test draws
				_DrawText("Triangles", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Filled", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Rectangles", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Filled;
				break;
			}
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.3f, White);

			//scroller
			Addresses::drawShader(713, 260 + (26 * textindex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818, 260 + (26 * textindex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (26 * textindex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 280 + (26 * textindex), 104, 0.9f, flColor); // bottom

		    //scroller bar
			Addresses::drawShader(1069, 251 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * textindex), 3, 26, flColor);

			// bar at the bottom
			Addresses::drawShader(706, 541, 359, 11, OpusBG);
			Addresses::drawShader(706, 555, 359, 17, OpusBG);
			Addresses::drawShader(706, 574, 359, 2, flColor);

			_DrawText("Select your ESP Type!", 848.0f, 551.5f, 0.25f, White);

			switch (charCount_esp) {
			case 1:
				// test draws
				_DrawText("Filled", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Triangles", 862.0f, 570.5f, 0.3f, flColor);
				_DrawText("Rectangles", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Triangles;
				break;
			case 2:
				// test draws
				_DrawText("Triangles", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Rectangles", 862.0f, 570.5f, 0.3f, flColor);
				_DrawText("Filled", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Rectangles;
				break;
			case 3:
				// test draws
				_DrawText("Triangles", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Filled", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Rectangles", 802.0f, 570.5f, 0.25f, White);
				Current_ESP = Filled;
				break;
			}
		}
	}
	else {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.3f, White);
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.3f, White);
		}
	}
}

// beta snapline char /* not using anymore */
void snapesp_charOption(char* text, int textindex, int secondIndex, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textindex) {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.3f, White);

			//scroller
			Addresses::drawShader(713 + 120, 260 + (26 * secondIndex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (26 * secondIndex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (26 * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 280 + (26 * secondIndex), 104, 0.9f, flColor); // bottom

																							  //scroll bar
			Addresses::drawShader(1069, 251 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * secondIndex), 3, 26, flColor);

			// bar at the bottom
			Addresses::drawShader(706, 541, 359, 11, OpusBG);
			Addresses::drawShader(706, 555, 359, 17, OpusBG);
			Addresses::drawShader(706, 574, 359, 2, flColor);

			_DrawText("Choose your snap pos!", 848.0f, 551.5f, 0.25f, White);

			switch (charCount_snapesp) {
			case 1:
				// test draws
				_DrawText("Middle", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Top", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Bottom", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Top;
				break;
			case 2:
				// test draws
				_DrawText("Top", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Bottom", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Middle", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Bottom;
				break;
			case 3:
				// test draws
				_DrawText("Top", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Middle", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Bottom", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Middle;
				break;
			}
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.3f, White);

			//scroller
			Addresses::drawShader(713, 260 + (26 * textindex), 0.9f, 21.0f, flColor); // left
			Addresses::drawShader(818, 260 + (26 * textindex), 0.9f, 20.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (26 * textindex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 280 + (26 * textindex), 104, 0.9f, flColor); // bottom

			//scroller bar
			Addresses::drawShader(1069, 251 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * textindex), 3, 26, flColor);

			// bar at the bottom
			Addresses::drawShader(706, 541, 359, 11, OpusBG);
			Addresses::drawShader(706, 555, 359, 17, OpusBG);
			Addresses::drawShader(706, 574, 359, 2, flColor);

			_DrawText("Choose your snap pos!", 848.0f, 551.5f, 0.25f, White);

			switch (charCount_snapesp) {
			case 1:
				// test draws
				_DrawText("Middle", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Top", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Bottom", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Top;
				break;
			case 2:
				// test draws
				_DrawText("Top", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Bottom", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Middle", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Bottom;
				break;
			case 3:
				// test draws
				_DrawText("Top", 942.0f, 570.5f, 0.25f, White);
				_DrawText("Middle", 872.0f, 570.5f, 0.3f, flColor);
				_DrawText("Bottom", 802.0f, 570.5f, 0.25f, White);
				Current_Snap = Middle;
				break;
			}
		}
	}
	else {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.3f, White);
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.3f, White);
		}
	}
}

float _raderDistance = 0.3f;
int _compassScale = 200;

void addSliderOption(char* text, int textindex, int secondIndex, int sliderInteger, int sliderAmount, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textindex) {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.25f, White);

			//scroller
			Addresses::drawShader(713 + 120, 260 + (26 * secondIndex), 0.9f, 31.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (26 * secondIndex), 0.9f, 30.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (26 * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 290 + (26 * secondIndex), 104, 0.9f, flColor); // bottom

			//scroll bar
			Addresses::drawShader(1069, 251 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * secondIndex), 3, 26, flColor);

			// slider bg
			Addresses::drawShader(723 + 120, 280 + (26 * secondIndex), 89, 0.9f, flColor); // slider bg
			Addresses::drawShader(723 + 120 + (sliderAmount * sliderInteger), 276 + (26 * secondIndex), 1, 9, White); // slider
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.25f, White);

			//scroller
			Addresses::drawShader(713, 260 + (26 * textindex), 0.9f, 31.0f, flColor); // left
			Addresses::drawShader(818, 260 + (26 * textindex), 0.9f, 30.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (26 * textindex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 290 + (26 * textindex), 104, 0.9f, flColor); // bottom

			//scroller bar
			Addresses::drawShader(1069, 251 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (26 * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (26 * textindex), 3, 26, flColor);

			// slider bg
			Addresses::drawShader(723, 280 + (26 * textindex), 89, 0.9f, flColor); // slider bg
			Addresses::drawShader(723 + (sliderAmount * sliderInteger), 276 + (26 * textindex), 1, 9, White); // slider
		}
	}
	else {
		if (textindex > 8) {
			_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.25f, White);
			//_DrawText(text, 719.0f + 120, 277.0f + (26 * secondIndex), 0.3f, White);
			// slider bg
			Addresses::drawShader(723, 280 + (26 * secondIndex), 89, 0.9f, flColor); // slider bg
			Addresses::drawShader(723 + (sliderAmount * sliderInteger), 276 + (26 * secondIndex), 1, 9, White); // slider
		}
		else {
			_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.25f, White);
			//_DrawText(text, 719.0f, 277.0f + (26 * textindex), 0.3f, White);

			// slider bg
			Addresses::drawShader(723, 280 + (26 * textindex), 89, 0.9f, flColor); // slider bg
			Addresses::drawShader(723 + (sliderAmount * sliderInteger), 276 + (26 * textindex), 1, 9, White); // slider
		}
	}
}

//colors
float __rgbCol[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
float __Green_[4] = { (float)128 / 255.0f,(float)173 / 255.0f,(float)102 / 255.0f,(float)255 / 255.0f };
float __Red_[4] = { (float)194 / 255.0f,(float)66 / 255.0f,(float)66 / 255.0f,(float)255 / 255.0f };
float __White__[4] = { (float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f,(float)255 / 255.0f };
float __blue_[4] = { (float)66 / 255.0f,(float)66 / 255.0f,(float)194 / 255.0f,(float)255 / 255.0f };
float __Yellow[4] = { (float)207 / 255.0f,(float)184 / 255.0f,(float)37 / 255.0f,(float)255 / 255.0f };

int _ESPSelector;
int _ESPMin = 0;
int _ESPMax = 4;
char* _ESPTypes[] = { "2D", "3D", "Tri", "Shaded", "Corners" };

int _CompassColorSelector;
int _CompassColorMin = 0;
int _CompassColorMax = 4;
char* _CompassColors[] = { "RGB", "Green", "White", "Red", "Blue" };
float* compassColor = __rgbCol;

int _ESPViewSelector;
int _ESPViewMin = 0;
int _ESPViewMax = 2;
char* _ESPView[] = {"Both", "Team", "Enemy"};

int _EnemyESPColorSelector;
int _EnemyESPColorMin = 0;
int _EnemyESPColorMax = 4;
char* _EnemyColors[] = { "RGB", "Yellow", "White", "Red", "Blue"};
float* _EnemyColor = __Yellow;

int _EnemyESPSizeSelector;
int _EnemyESPSizeMin = 0;
int _EnemyESPSizeMax = 3;
char* _EnemyESPSizes[] = { "1px", "2px", "3px", "4px" };
int _EnemyESPSize = 1;

int _TeamESPColorSelector;
int _TeamESPColorMin = 0;
int _TeamESPColorMax = 4;
char* _TeamColors[] = { "RGB", "Yellow", "White", "Red", "Blue" };
float* _TeamColor = __Yellow;

int _TeamESPSizeSelector;
int _TeamESPSizeMin = 0;
int _TeamESPSizeMax = 3;
char* _TeamESPSizes[] = { "1px", "2px", "3px", "4px" };
int _TeamESPSize = 1;

void addCharOption(char* text, int textindex, int secondIndex, char **charValues, int*var, int min, int max, int additionalIndexIncrease, float x) {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	if (vars.scroll == textindex) {
		if (textindex > 8) {
			char buf[255];
			sprintf(buf, "%s (%i|%i)", charValues[*var], *var, max);
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);
			_DrawText(buf, 719.0f + 120, 288.0f + (additionalIndexIncrease * secondIndex), 0.24f, flColor);

			//scroller
			Addresses::drawShader(713 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 31.0f, flColor); // left
			Addresses::drawShader(818 + 120, 260 + (additionalIndexIncrease * secondIndex), 0.9f, 30.85f, flColor); // right
			Addresses::drawShader(713.93f + 120, 260 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f + 120, 290 + (additionalIndexIncrease * secondIndex), 104, 0.9f, flColor); // bottom

			//scroll bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * secondIndex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * secondIndex), 3, 26, flColor);

		}
		else {
			char buf[255];
			sprintf(buf, "%s (%i|%i)", charValues[*var], *var, max);
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textindex), 0.25f, White);
			_DrawText(buf, 719.0f, 288.0f + (additionalIndexIncrease * textindex), 0.24f, flColor);

			//scroller
			Addresses::drawShader(713, 260 + (additionalIndexIncrease * textindex), 0.9f, 31.0f, flColor); // left
			Addresses::drawShader(818, 260 + (additionalIndexIncrease * textindex), 0.9f, 30.85f, flColor); // right
			Addresses::drawShader(713.93f, 260 + (additionalIndexIncrease * textindex), 104, 0.9f, flColor); // top
			Addresses::drawShader(713.93f, 290 + (additionalIndexIncrease * textindex), 104, 0.9f, flColor); // bottom

																						//scroller bar
			Addresses::drawShader(1069, 251 + (additionalIndexIncrease * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 280 + (additionalIndexIncrease * textindex), 3, 0.9f, OpusBG);
			Addresses::drawShader(1069, 253 + (additionalIndexIncrease * textindex), 3, 26, flColor);

		}
	}
	else {
		if (textindex > 8) {
			char buf[255];
			sprintf(buf, "%s (%i|%i)", charValues[*var], *var, max);
			_DrawText(text, 719.0f + 120, 277.0f + (additionalIndexIncrease * secondIndex), 0.25f, White);
			_DrawText(buf, 719.0f + 120, 288.0f + (additionalIndexIncrease * secondIndex), 0.24f, flColor);
		}
		else {
			char buf[255];
			sprintf(buf, "%s (%i|%i)", charValues[*var], *var, max);
			_DrawText(text, 719.0f, 277.0f + (additionalIndexIncrease * textindex), 0.25f, White);
			_DrawText(buf, 719.0f, 288.0f + (additionalIndexIncrease * textindex), 0.24f, flColor);
		}
	}
}

void DrawMenu() {
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	Addresses::drawShader(706, 251, 359, 284, OpusBG); // BG num 1.
	Addresses::drawShader(611, 251, 89, 284, OpusBG); // BG num 2.
	Addresses::drawShader(625, 278, 57, 1, flColor);
	// for number 1
	Addresses::drawShader(706, 247, 359, 2, flColor); // Old | Green 
	Addresses::drawShader(706, 537, 359, 2, flColor);

	// for number 2
	Addresses::drawShader(611, 247, 89, 2, flColor);
	Addresses::drawShader(611, 537, 89, 2, flColor);
	_DrawText("CrypticPro", 627.0f, 280.0f, 0.5f, flColor);
}

void watermark()
{
	// information
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	_DrawText("CrypticPro", 1160.0f, 702.0f, 0.3f, flColor);
	_DrawText("- 2017", 1196.0f, 702.0f, 0.3f, White);
	_DrawText(cheatsVer, 1239.0f, 690.0f, 0.3f, flColor);
}

void _MapOutline() {
	// around the map
	float flColor[4] = { (float)BannerColorRGB.R / 255.0f,(float)BannerColorRGB.G / 255.0f,(float)BannerColorRGB.B / 255.0f,(float)255 / 255.0f };
	Addresses::drawShader(106, 58, 3, 181, flColor); //sides
	Addresses::drawShader(259, 58, 3, 181, flColor); //sides
	Addresses::drawShader(109, 58, 150, 3, flColor); //top
	Addresses::drawShader(109, 236, 150, 3, flColor); //bottom
	// _Green
}

#define M_PI 3.14159265359f

#define	SHORT2ANGLE(x)	((x) * (360.0/65536.0f))

Vector3 ViewPos, SpreadAngle, spreadAng, spreadEndVec;

int cg_ss(int Client)
{
	int Value = *(int *)(0x82BBAE68);
	if (Value == 0)
		return 0;
	Value += (Client * 0x87700);

	return Value;
}

int ClientActive_f(int Client)
{
	int Value = *(int *)(0x82C70F4C);
	if (Value == 0)
		return 0;
	return (Value);
}

void __cdecl VectorToAngles_(const float * Vec, float * angles)
{
	float forward, yaw, pitch, PiDiv = (180.0f / XM_PI);

	if (Vec[1] == 0 && Vec[0] == 0)
	{
		yaw = 0;
		if (Vec[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		if (Vec[0])
			yaw = (atan2(Vec[1], Vec[0]) * PiDiv);
		else if (Vec[1] > 0)
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if (yaw < 0.0f)
			yaw += 360.0f;

		forward = sqrt((Vec[0] * Vec[0]) + (Vec[1] * Vec[1]));
		pitch = (atan2(Vec[2], forward) * PiDiv);

		if (pitch < 0.0f)
			pitch += 360;
	}

	angles[0] = 360.0f - pitch;
	angles[1] = yaw;
	angles[2] = 0.0f;
	for (int i = 0; i < 2; i++) {
		if (angles[i] > 360.0f)
			angles[i] -= 360.0f;
		if (angles[i] < 0.0f)
			angles[i] += 360.0f;
	}
}

void __cdecl AngleVectors_(const float * angles, float * forward, float * right, float * up)
{
	float angle, sr, sp, sy, cr, cp, cy, PiDiv = (XM_PI / 180.0f);

	angle = angles[1] * PiDiv;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[0] * PiDiv;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[2] * PiDiv;
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
		right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
		right[2] = -1 * sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy + -sr*-sy);
		up[1] = (cr*sp*sy + -sr*cy);
		up[2] = cr*cp;
	}
}


#define ANGLE2SHORT(x) ((int)((x)*65536/360) & 65535)

typedef int(*Key_IsDown)(int ClientNum, int ButtonEnum);
Key_IsDown BO2_Key_Down = (Key_IsDown)0x8226DB40;

void _spreadFix(Usercmd_t *cmd)
{
	printf("no spread called\n");
	//if (!BO2_Key_Down(0, 0X12))
	//{
		float viewAngles[3],
			viewAxis[3][3],
			minSpread,
			maxSpread,
			spread,
			spreadEndVec[3],
			spreadDirection[3];

		int ClientActive_ts,
			cg_s,
			*cmdNum,
			seed;

		float Range;
		int playerState,
			WeaponNum,
			WeaponDef,
			unkSpread = 0;


		ClientActive_ts = ClientActive_f(0);
		cg_s = cg_ss(0);
		cmdNum = (int *)(ClientActive_ts + 0x44AA8);

		playerState = cg_s + 0x480A8;
		WeaponNum = *(int *)(cg_s + 0x248);
		WeaponDef = ((int(*)(int))0x826BF988)(WeaponNum);
		unkSpread = 0;


		cmd->buttons &= ~0x80000000;

		((void(*)(...))0x826BB4E0)(playerState, WeaponNum, &minSpread, &maxSpread);

		spread = *(float *)(cg_s + 0x80B80) * 0.0039215689f;

		if (*(float *)(playerState + 0x1E0) == 1.0f)
			minSpread = *(float *)(WeaponDef + 0x7E0);

		spread = ((spread * (maxSpread - minSpread)) + minSpread);

		viewAngles[0] = *(float *)(playerState + 0x1F8);
		viewAngles[1] = *(float *)(playerState + 0x1FC);
		viewAngles[2] = 0;
		AngleVectors_(viewAngles, viewAxis[0], viewAxis[1], viewAxis[2]);

		seed = *(int *)playerState;

		((void(*)(...))0x826961B8)(&seed);

		if (*(int *)(WeaponDef + 0x20) != 0x3 && *(int *)(WeaponDef + 0x20) != 0xD)
			unkSpread = 1;
		else
			unkSpread = *(int *)(WeaponDef + 0x3D0);

		if (*(int *)(WeaponDef + 32) == 3 || *(int *)(WeaponDef + 32) == 0xD)
			Range = *(float *)(WeaponDef + 0x410) * ((float(*)(...))0x826C40C8)(WeaponNum);
		else
			Range = 8192.0f;

		((void(*)(...))0x8224D4A8)(&seed, spread, (float*)&cgArray_s->RefDef.viewOrigin, spreadEndVec, spreadDirection, viewAxis[0], viewAxis[1], viewAxis[2], Range,
			WeaponNum, 0, unkSpread);

		VectorToAngles_(spreadDirection, viewAngles);

		//if (*(float *)(cg_s + 0x4DB18) == 0)
		//{
			cmd->viewAngles[0] += ANGLE2SHORT(*(float *)(playerState + 0x1F8) - viewAngles[0]);
			cmd->viewAngles[1] += ANGLE2SHORT(*(float *)(playerState + 0x1FC) - viewAngles[1]);
		//}
		printf("no spead done\n");
	//}
}

bool bTriggerShoot;

/*Vector3 GetPlayerOrigin(int c) {
	return Centity_s[c].Origin;
}*/


typedef int(*BO2_GetTagPos_t)(BO2_cEntity * TheEnt, short tag, float* out);
BO2_GetTagPos_t BO2_GetTagPos = (BO2_GetTagPos_t)0x821A1A40;

typedef int(*BO2_CanSeeFriendlyHead_)(int Unknownn, BO2_cEntity * pEnt, DWORD mask);
BO2_CanSeeFriendlyHead_ BO2_CanSeeFriendlyHead = (BO2_CanSeeFriendlyHead_)0x821C47B8;

typedef void(__cdecl *CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, float x, float y, float width, float height, float angle, const float *color, int Material);
CG_DrawRotatedPicPhysical BO2_DrawRotated = (CG_DrawRotatedPicPhysical)0x821C7F58;

#define BO2_CanSeeFriend 0x821C47B8//BO2 TU18
typedef int(*BO2_CanSeeFriendlyHead2_)(int unknown, BO2_cEntity * pEnt);
BO2_CanSeeFriendlyHead2_ BO2_CanSeeFriendlyHead2 = (BO2_CanSeeFriendlyHead2_)BO2_CanSeeFriend;


BYTE MyTeam;
#define ENTITIESMAX 12
BO2_ClientInfo *BO2_Clients[ENTITIESMAX];
void __readGame()
{
	BO2_RefDef = (BO2_cRefdef*)BO2_RefDefCalc;
	BO2_cg = (BO2_CG_t*)BO2_getCg();
	BO2_cgs = (BO2_CGS*)BO2_getCgs();
	DWORD cent_ptr = *(DWORD*)0x82BBC554;

	for (int i = 0; i < ENTITIESMAX; i++)
		BO2_Entities[i] = (BO2_cEntity*)(cent_ptr + ((int)0x374 * i));

	for (int i = 0; i < ENTITIESMAX; i++)
		BO2_Clients[i] = (BO2_ClientInfo*)BO2_getClientbyIDx(i);

	for (int i = 0; i < ENTITIESMAX; i++)
	{
		if (BO2_cg->cNum == i)
		{
			MyTeam = BO2_Clients[i]->mTeam;
			break;
		}
	}
}