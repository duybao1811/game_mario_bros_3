#pragma once
#include "GameObject.h"
#include "define.h"
class CGoomba : public CGameObject
{
public: 	
	float lastvx;
	int model;
	int JumpCount;
	bool isOnAir;
	DWORD TimeWalk;
	DWORD TimeDisappear;
	bool isDie;
	DWORD start_die;
	bool isOnGround;
	bool isAttacked;
	CGoomba(float X,float Y,int Model, int d);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};