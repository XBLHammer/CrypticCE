#include "stdafx.h"
#include "Addresses.h"
#include "Structures.h"
#include <cstdio>
#include "Varibles.h"


class BO2_UserCMD
{
public:
	int ServerTime; //0x0000 
	int Button; //0x0004 
	int ViewAngle[3]; //0x0008 
	int WeaponID; //0x0014 
	char _0x0018[4];
	int moveTonpere; //0x001C 
	char _0x0020[38];
};//Size=0x0046

class BO2_clientActive
{
public:
	char _0x0000[152];
	D3DXVECTOR3 vOrigin; //0x0098 
	char _0x00A4[100];
	D3DXVECTOR3 RefAngles; //0x0108 
	char _0x0114[11128];
	D3DXVECTOR3 ViewAngles; //0x2C8C
	char _0x02C98[16];
	BO2_UserCMD cmds[128];//0x2CA8
	int CmdNum; // 0x4AA8

	BO2_UserCMD * GetCmd(int Index)
	{
		return &cmds[(Index & 0x7F)];
	}
};

BO2_clientActive *BO2_getCA()
{
	DWORD ca_ptr = *(DWORD*)0x82C70F4C;

	return (BO2_clientActive*)ca_ptr;
}

#define ENTITIESMAX 12
void ___readGame()
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

struct Vec3
{
	float x, y, z;
};
float angles[3];
Vector vectoangles(Vec3 Angles)
{
	float forward;
	float yaw, pitch;
	float PI = 3.1415926535897931;
	if (Angles.x == 0 && Angles.y == 0)
	{
		yaw = 0;
		if (Angles.z > 0) pitch = 90.00;
		else pitch = 270.00;
	}
	else
	{
		if (Angles.x != -1) yaw = (float)(atan2((double)Angles.y, (double)Angles.x) * 180.00 / PI);
		else if (Angles.y > 0) yaw = 90.00;
		else yaw = 270;
		if (yaw < 0) yaw += 360.00;

		forward = (float)sqrt((double)(Angles.x * Angles.x + Angles.y * Angles.y));
		pitch = (float)(atan2((double)Angles.z, (double)forward) * 180.00 / PI);
		if (pitch < 0) pitch += 360.00;
	}
	angles[0] = -pitch;
	angles[1] = yaw;
	angles[2] = 0;

	return angles;
}

Vec3 Difference;
Vec3 GetVec(D3DXVECTOR3 Attacker, D3DXVECTOR3 Target)
{
	Difference.x = (Target.x - Attacker.x);
	Difference.y = (Target.y - Attacker.y);
	Difference.z = (Target.z - Attacker.z);
	return Difference;
}

float __Get__distance(D3DXVECTOR3 me, D3DXVECTOR3 enemi)
{
	float dx = me.x - enemi.x;
	float dy = me.y - enemi.y;
	float dz = me.z - enemi.z;
	return (float)sqrt((dx * dx) + (dy * dy) + (dz * dz))  * 0.03048f;
}

D3DXVECTOR3 SilentAngles;

/* yes, it's fusions aimbot, since i never done aimbot, i was gonna do some freaky shit with this lol*/
void __Aimbot() {
	DWORD pd = *(DWORD*)0x82C70F4C;
	BO2_clientActive *cmdInput = (BO2_clientActive*)pd;
	BO2_UserCMD* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
	BO2_UserCMD* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);
	___readGame();
	BO2_cEntity * Target = NULL;
	BO2_clientActive * ca = NULL;
	ca = (BO2_clientActive*)BO2_getCA();
	BO2_clientActive *Ca = BO2_getCA();
	Vector vMyAngles = Ca->ViewAngles;
	Vector vAngles;
	float objectDistance = 0.f;
	float flClosestDistance = (float)INT_MAX;
	D3DXVECTOR3 vHead, vBestHead;
	short mTag = *(short*)0x836C5C94;
	if (vars.aimbot_) {

		for (int i = 0; i < 12; i++)
		{
			if (i == BO2_cg->cNum)
				continue;

			if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
			{
				if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
				{
					if (BO2_CanSeeFriendlyHead2(0, BO2_Entities[i]) == 1)
					{
						objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

						Target = BO2_Entities[i];
						BO2_GetTagPos(Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
				}
				else
				{
					if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
					{
						if (BO2_CanSeeFriendlyHead2(0, BO2_Entities[i]) == 1)
						{
							objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

							Target = BO2_Entities[i];
							BO2_GetTagPos(Target, mTag, vHead);

							if (objectDistance < flClosestDistance)
							{
								vBestHead = vHead;
								flClosestDistance = objectDistance;
							}
						}
					}
				}
			}
		}

		if ((VALID(Target)) && (VALID(ca)))
		{
			CanDrawTargetedESPBO2 = true;
			DrawTargetDetailsBO2 = true;
			if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
			{
				vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
				ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
				ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
			}
		}
	}
}


void Nearest()
{

	DWORD pd = *(DWORD*)0x82C70F4C;
	BO2_clientActive *cmdInput = (BO2_clientActive*)pd;
	BO2_UserCMD* cmd = cmdInput->GetCmd(cmdInput->CmdNum);
	BO2_UserCMD* oldCmd = cmdInput->GetCmd(cmdInput->CmdNum - 1);
	___readGame();
	BO2_cEntity * Target = NULL;
	BO2_clientActive * ca = NULL;
	ca = (BO2_clientActive*)BO2_getCA();
	BO2_clientActive *Ca = BO2_getCA();
	Vector vMyAngles = Ca->ViewAngles;
	Vector vAngles;
	float objectDistance = 0.f;
	float flClosestDistance = (float)INT_MAX;
	D3DXVECTOR3 vHead, vBestHead;
	short mTag = *(short*)0x836C5C94;


	if (vars.aimbotnearest_) {

		for (int i = 0; i < 12; i++)
		{
			if (i == BO2_cg->cNum)
				continue;

			if ((BO2_Entities[i]->Valid) && (BO2_Entities[i]->State == 64))
			{
				if (strcmp(BO2_cgs->gametype, "dm") == 0 || strcmp(BO2_cgs->gametype, "gun") == 0 || strcmp(BO2_cgs->gametype, "oic") == 0 || strcmp(BO2_cgs->gametype, "shrp") == 0 || strcmp(BO2_cgs->gametype, "sas") == 0)
				{
					objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

					Target = BO2_Entities[i];
					BO2_GetTagPos(Target, mTag, vHead);

					if (objectDistance < flClosestDistance)
					{
						vBestHead = vHead;
						flClosestDistance = objectDistance;
					}
				}
				else
				{
					if (BO2_Clients[i]->mTeam != BO2_Clients[BO2_cg->cNum]->mTeam)
					{
						objectDistance = GetDistance(BO2_RefDef->EyePos, BO2_Entities[i]->mPos);

						Target = BO2_Entities[i];
						BO2_GetTagPos(Target, mTag, vHead);

						if (objectDistance < flClosestDistance)
						{
							vBestHead = vHead;
							flClosestDistance = objectDistance;
						}
					}
				}
			}
		}

		if ((VALID(Target)) && (VALID(ca)))
		{
			CanDrawTargetedESPBO2 = true;
			DrawTargetDetailsBO2 = true;
			if (BO2_Key_Down(0, 0x12) || *AutoAimBO2)
			{
				vAngles = vectoangles(GetVec(BO2_RefDef->EyePos, vBestHead));
				ca->ViewAngles.x = vAngles.x - ca->RefAngles.x;
				ca->ViewAngles.y = vAngles.y - ca->RefAngles.y;
			}
		}
	}
}

void fakelag()
{

	if (vars.HVH_Fake_Lag)
	{
		if (GetTickCount() - dwLagTick > fLag)
			dwLagTick = GetTickCount();
		else
			bSendPackett = false;
	}

}












