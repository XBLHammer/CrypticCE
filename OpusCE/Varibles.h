#pragma once
#ifndef __VARIBLES
#define __VARIBLES
char* cheatsVer = "V1.1"; /* make sure to update every BIG update */
float Red[4] = { 1, 0, 0, 1 };
float LightRed[4] = { 1, 0, 0, 0.6f };
float LightGreen[4] = { 0, 1, 0, 0.2f };
float Green[4] = { 0, 1, 0, 1 };
float _Green[4] = { 0.0f, 1.0f, 0.0f, 0.4f };
float Blue[4] = { 0, 0, 1, 0.4 };
float LightBlue[4] = { 0, 0, 1, 0.2f };
float Yellow[4] = { 1, 1, 0, 1 };
float LightYellow[4] = { 1, 1, 0, 0.2f };
int dwLagTick = 0;
bool bSendPackett;
int fLag = 200;
float Purple[4] = { 0.4, 0, 0.7, 1 };
float LightPurple[4] = { 0.4, 0, 0.7, 0.2f };
float Pink[4] = { 0.9, 0, 0.9, 1 };
bool CanDrawTargetedESPBO2 = false;
bool DrawTargetDetailsBO2 = false;
bool LastFrameAimKey = false;
bool AimboptionsBO2[6] = { false };
bool *AutoAimBO2 = &AimboptionsBO2[2];
float LightPink[4] = { 0.9, 0, 0.9, 0.2f };
float Orange[4] = { 1, 0.4, 0, 1 };
float LightOrange[4] = { 1, 0.4, 0, 0.6f };
float _Orange[4] = { 1, 0.4, 0, 0.4f };
float Cyan[4] = { 0, 1, 0.9, 1 };
float White[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float Black[4] = { 0, 0, 0, 1.0f };
float LightBlack[4] = { 0, 0, 0, 0.6f };
float Mint[4] = { 0.04, 0.82, 0.43, 1 };
float Grey[4] = { 0.32, 0.33, 0.34, 1 };
float mainBlue[4] = { 0, 0.67f, 0.54f, 0.7f };
float mainBlueAlpha[4] = { 0, 0.67f, 0.54f, 1 };
float bluee[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
float OpusGreen[4] = { 0.07f, 0.98f, 0.24f, 1.0f };
float OpusBG[4] = { 0.16f, 0.17f, 0.16f, 0.7f };
float dark_OpusBG[4] = { 0.16f, 0.17f, 0.16f, 1.0f };

float* titlebackground = Black;
float* title = White;
float* backgroundcolor = Blue;
float* optionsbackground = Black;
float* scrollcolor = Blue;
float* _backgroundColor = Grey;
float* _customOpus = OpusGreen;
float* _customOpusBackground = OpusBG;

enum submenus {
	null,
	main,
	_Aimbot,
	_ESP,
	Misc,
	Controls,
	HVH,
};

enum tabMenus {
	Aimbot,
	ESP
};

int fluxswitch = 0;
float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float* backgroundColor = color;


// ESP Type cnt
int charCount_esp = 1;

// Snap ESP count
int charCount_snapesp = 3;

// Starter ESP
enum ESP_Types {
	Triangles = 1,
	Rectangles = 2,
	Filled = 3
};
int Current_ESP = Triangles;

// snaplines
enum Snap_Types {
	Top = 1,
	Bottom = 2, 
	Middle = 3
};
int Current_Snap = Middle;


int sliderCount_fov = 1;
int sliderCount_radar_distance = 1;
int sliderCount_compass_size = 1;
#endif

BYTE UnlockAll[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff
};












typedef void(__cdecl *SV_GameSendServerCommand_t)(int clientindex, const char *svcommand);
SV_GameSendServerCommand_t SV_GameSendServerCommand = (SV_GameSendServerCommand_t)0x8242FB70;

static void UnlockAchievments3() {
	SV_GameSendServerCommand(-1, "< \"^6Unlocing Achievments ^5Part 3 Started\"");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_YEMEN");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_PANAMA");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_KARMA");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_****STAN");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_NICARAGUA");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_AFGHANISTAN");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_MONSOON");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_ANGOLA");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_FSIRT_AGAINST_THE_WALL");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_AWAKEN_THE_GAZEBO");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_MAZED_AND_CONFUSED");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_BURIED_SIDEQUEST");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_IM_YOUR_HUCKLEBERRY");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_ECTOPLASMIC_RESIDUE");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_DEATH_FROM_BELOW");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_CANDYGRAM");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_REVISIONIST_HISTORIAN");
	SV_GameSendServerCommand(-1, "# \"ZM_DLC4_TOMB_SIDEQUEST\"");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_ALL_YOUR_BASE");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_PLAYING_WITH_POWER");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_OVERACHIEVER");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_NOT_A_GOLD_DIGGER");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_KUNG_FU_GRIP");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_IM_ON_A_TANK");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_SAVING_THE_DAY_ALL_DAY");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_MASTER_OF_DISGUISE");
	SV_GameSendServerCommand(-1, "# ZM_DLC4_MASTER_WIZARD");
	SV_GameSendServerCommand(-1, "; \"^5100% Unlocked\"");
	SV_GameSendServerCommand(-1, "# ZM_PRISON_PERK_CHERRY");
	SV_GameSendServerCommand(-1, "< \"^6Project XDK Has ^5Finished Unlocking ^2Your Achievments\"");
}
void UnlockAllAchievments() {
	SV_GameSendServerCommand(-1, "< \"^6Project XDK ^5Is Unlocking ^2Achievments ^1<3\"");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_TRAPPED_IN_TIME");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_POP_GOES_THE_WEASEL");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_FULL_LOCKDOWN");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_GG_BRIDGE");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_PARANORMAL_PROGRESS");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_A_BURST_OF_FLAVOR");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_FEED_THE_BEAST");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_ACID_DRIP");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_MAKING_THE_ROUNDS");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_MONKEY_SEE_MONKEY_DOOM");
	SV_GameSendServerCommand(-1, "# ZM_DLC2_PRISON_SIDEQUEST");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_I_SEE_LIVE_PEOPLE");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_FACING_THE_DRAGON");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_POLYARMORY");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_IM_MY_OWN_BEST_FRIEND");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_MAD_WITHOUT_POWER");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_SLIPPERY_WHEN_UNDEAD");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_SHAFTED");
	SV_GameSendServerCommand(-1, "; \"^525% Unlocked\"");
	SV_GameSendServerCommand(-1, "# ZM_HAPPY_HOUR");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_VERTIGONER");
	SV_GameSendServerCommand(-1, "# ZM_DLC1_HIGHRISE_SIDEQUEST");
	SV_GameSendServerCommand(-1, "# ZM_YOU_HAVE_NO_POWER_OVER_ME");
	SV_GameSendServerCommand(-1, "# ZM_FUEL_EFFICIENT");
	SV_GameSendServerCommand(-1, "# ZM_I_DONT_THINK_THEY_EXIST");
	SV_GameSendServerCommand(-1, "# ZM_UNDEAD_MANS_PARTY_BUS");
	SV_GameSendServerCommand(-1, "# ZM_STANDARD_EQUIPMENT_MAY_VARY");
	SV_GameSendServerCommand(-1, "# ZM_DANCE_ON_MY_GRAVE");
	SV_GameSendServerCommand(-1, "# ZM_TRANSIT_SIDEQUEST");
	SV_GameSendServerCommand(-1, "# ZM_THE_LIGHTS_OF_THEIR_EYES");
	SV_GameSendServerCommand(-1, "# ZM_DONT_FIRE_UNTIL_YOU_SEE");
	SV_GameSendServerCommand(-1, "# MP_MISC_3");
	SV_GameSendServerCommand(-1, "# MP_MISC_5");
	SV_GameSendServerCommand(-1, "# MP_MISC_4");
	SV_GameSendServerCommand(-1, "# SP_MISC_10K_SCORE_ALL");
	SV_GameSendServerCommand(-1, "# MP_MISC_2");
	SV_GameSendServerCommand(-1, "# MP_MISC_1");
	SV_GameSendServerCommand(-1, "# SP_BACK_TO_FUTURE");
	SV_GameSendServerCommand(-1, "# SP_MISC_WEAPONS");
	SV_GameSendServerCommand(-1, "# SP_STORY_99PERCENT");
	SV_GameSendServerCommand(-1, "# SP_STORY_CHLOE_LIVES");
	SV_GameSendServerCommand(-1, "# SP_MISC_ALL_INTEL");
	SV_GameSendServerCommand(-1, "# SP_STORY_MENENDEZ_CAPTURED");
	SV_GameSendServerCommand(-1, "; \"^250% Unlocked\"");
	SV_GameSendServerCommand(-1, "< \"^6Part 1 ^5Finished\"");
} 
 void UnlockAllAchievments2() {
	SV_GameSendServerCommand(-1, "< \"^6Unlocing Achievments ^5Part 2 Started\"");
	SV_GameSendServerCommand(-1, "# SP_STORY_HARPER_LIVES");
	SV_GameSendServerCommand(-1, "# SP_STORY_LINK_CIA");
	SV_GameSendServerCommand(-1, "# SP_STORY_OBAMA_SURVIVES");
	SV_GameSendServerCommand(-1, "# ZM_DLC3_WHEN_THE_REVOLUTION_COMES");
	SV_GameSendServerCommand(-1, "# SP_STORY_FARID_DUEL");
	SV_GameSendServerCommand(-1, "# SP_STORY_HARPER_FACE");
	SV_GameSendServerCommand(-1, "# SP_STORY_MASON_LIVES");
	SV_GameSendServerCommand(-1, "# SP_RTS_SOCOTRA");
	SV_GameSendServerCommand(-1, "# SP_RTS_****STAN");
	SV_GameSendServerCommand(-1, "# SP_RTS_CARRIER");
	SV_GameSendServerCommand(-1, "# SP_RTS_DRONE");
	SV_GameSendServerCommand(-1, "; \"^669% Unlocked ^1<3\"");
	SV_GameSendServerCommand(-1, "# SP_RTS_AFGHANISTAN");
	SV_GameSendServerCommand(-1, "# SP_RTS_DOCKSIDE");
	SV_GameSendServerCommand(-1, "# SP_ONE_CHALLENGE");
	SV_GameSendServerCommand(-1, "# SP_ALL_CHALLENGES_IN_LEVEL");
	SV_GameSendServerCommand(-1, "# SP_ALL_CHALLENGES_IN_GAME");
	SV_GameSendServerCommand(-1, "# SP_VETERAN_FUTURE");
	SV_GameSendServerCommand(-1, "# SP_VETERAN_PAST");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_HAITI");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_LA");
	SV_GameSendServerCommand(-1, "# SP_COMPLETE_BLACKOUT");
	SV_GameSendServerCommand(-1, "; \"^575% Unlocked\"");
	SV_GameSendServerCommand(-1, "< \"^6Part 2 ^5Finished\"");
}
 