#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.035f;

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_ATTACKED 300
#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_ATTACKED 2
#define GOOMBA_ATTACKED_SPEED_X 0.1f
#define GOOMBA_ATTACKED_SPEED_Y 0.35f
class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public: 	
	int time;
	bool isDie;
	DWORD start_die;
	bool isFinish;
	bool isAttacked;
	CGoomba();
	virtual void SetState(int state);
};