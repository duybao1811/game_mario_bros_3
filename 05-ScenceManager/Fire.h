#pragma once
#include "GameObject.h"
#include "Camera.h"
#define FIRE_SPEED 0.15f
#define FIRE_GRAVITY 0.0015f
#define FIRE_BOUNCE  0.2f
#define FIRE_BBOX_WIDTH 8
#define FIRE_BBOX_HEIGHT 8
#define SHOOT_FIRE_RIGHT 0
#define SHOOT_FIRE_LEFT 1
#define EFFECT_FIRE_DESTOY 2
class Fire : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	bool isFinish;
	int destroy;
	bool isDestroy;
	Fire();
	~Fire() {}
};

