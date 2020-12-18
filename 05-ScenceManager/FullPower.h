#pragma once
#include "GameObject.h"
#define FULL_POWER_ANI 0
#define ID_LOAD_FULL_POWER 25
class FullPower:public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; };
	FullPower(float X, float Y);
};

