#pragma once
#include "Enemy.h"
#include "Mario.h"
#include "define.h"
#include "PlayScence.h"
#define PLANT_BBOX_WIDTH 16
#define PLANT_BBOX_HEIGHT 24
#define	PLANT_ANI 0
#define PLANT_STATE_GROW_UP 100
#define PLANT_STATE_HIDDING 200
#define PLANT_SPEED_GROW_UP 0.002f
#define PLANT_SPEED_HIDDING 0.002f
#define TIME_DELAY_GROW_UP 1500
#define TIME_DELAY_HIDE 1500
class CPiranhaPlant : public Enemy
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	float maxY;
	float minY;
	CMario* mario;
	DWORD TimeHidding;
	DWORD TimeGrowUp;
	int model;
	CPiranhaPlant(float X, float Y, int Model);
	virtual void SetState(int state);
	int GetModel();
};


