#pragma once
#include "GameObject.h"
#define FLOOR_SPEED_Y 0.1f
#define FLOOR_HEIGHT 183
class Floor : public CGameObject
{
public:
	Floor(float X,float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

