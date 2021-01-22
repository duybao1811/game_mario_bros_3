#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Effect.h"
#include "EffectDisappear.h"
#define FIRE_SPEED 0.006f
#define FIRE_GRAVITY 0.0006f
#define FIRE_BOUNCE_SPEED_Y  0.15f
#define FIRE_BBOX_WIDTH 8
#define FIRE_BBOX_HEIGHT 8
#define SHOOT_FIRE_RIGHT 0
#define SHOOT_FIRE_LEFT 1
#define LOAD_FIRE_FROM_TXT 4
class Fire : public CGameObject
{
	vector<LPGAMEOBJECT> ListEffect;
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Fire(float X,float Y);
	~Fire() {}
};

