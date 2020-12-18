#pragma once
#include "GameObject.h"
#define FLOOR_SPEED_Y 0.05f
class Floor : public CGameObject
{
public:
	int model;
	Floor(float X,float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

