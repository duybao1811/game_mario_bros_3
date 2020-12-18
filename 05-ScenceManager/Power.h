#pragma once
#include "GameObject.h"
#define POWER_ANI 0
#define ID_LOAD_POWER 24
class Power : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; };
	Power(float X, float Y);
};

