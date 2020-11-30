#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.025f
#define GOOMBA_RED_WALKING_SPEED 0.02f
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_RED_BBOX_WIDTH 20
#define GOOMBA_RED_BBOX_HEIGHT 21 
#define GOOMBA_BBOX_HEIGHT_DIE 9
#define GOOMBA_JUMP_SPEED_Y 0.2f
#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_ATTACKED 300
#define GOOMBA_STATE_JUMP 400
#define GOOMBA_RED_PARA_STATE_WALKING 500
#define GOOMBA_RED_PARA_STATE_FALLING 600
#define GOOMBA_RED_STATE_WALKING 700
#define GOOMBA_RED_PARA_STATE_DIE 800
#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_ATTACKED 2
#define GOOMBA_RED_ANI_WALKING 3
#define GOOMBA_RED_PARA_ANI_DIE 4
#define GOOMBA_RED_PARA_ANI_WALKING 5
#define GOOMBA_RED_PARA_ANI_JUMP 6
#define GOOMBA_RED_PARA_ANI_FALL 7
#define GOOMBA_ATTACKED_SPEED_X 0.1f
#define GOOMBA_ATTACKED_SPEED_Y 0.35f
#define GOOMBA_BASE 1
#define GOOMBA_RED_PARA 2
class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	float lastvx;
	int model;
	int JumpCount;
	DWORD TimeWalk;
	DWORD TimeDisappear;
	bool isDie;
	DWORD start_die;
	bool isOnGround;
	bool isFinish;
	bool isAttacked;
	CGoomba(int Model, int d);
	virtual void SetState(int state);
};