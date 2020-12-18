#pragma once
#include "GameObject.h"
#include "define.h"
#define PSWITCH_BBOX_HEIGHT 16
#define PSWITCH_BBOX_WIDTH 16
#define PSWITCH_ANI 0
#define PSWITCH_ANI_USED 1
#define LOAD_PSWITCH_FROM_TXT 22
#define PSWITCH_STATE_USED 100
#define PSWITCH_SMALLER 10
class P_Switch : public CGameObject
{
public:
	bool isUsed;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject);
	P_Switch(float X, float Y);
	virtual void SetState(int state);
};

