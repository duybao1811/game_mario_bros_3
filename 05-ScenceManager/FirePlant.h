#pragma once
#include "GameObject.h"
#include "define.h"
#define LOAD_FIRE_FROM_TXT 4
#define FIRE_BBOX_WIDTH 8
#define FIRE_BBOX_HEIGHT 8
#define SHOOT_FIRE_RIGHT 0
#define SHOOT_FIRE_LEFT 1
#define FIRE_ENEMY_SPEED_Y_FAR 0.003f
#define FIRE_ENEMY_SPEED_Y_NEAR 0.0025f
#define FIRE_ENEMY_SPEED_X_NEAR 0.004f
#define FIRE_ENEMY_SPEED_X_FAR  0.005f
class FirePlant : public CGameObject
{
	Range MarioRange;
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetFireFly(Range marioRange);
	FirePlant(float X, float Y, Range marioRange);
	~FirePlant() {}
};

