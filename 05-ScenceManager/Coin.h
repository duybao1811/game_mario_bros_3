#pragma once
#include "GameObject.h"
#define COIN_ANI 0
#define COIN_WIDTH 16
#define COIN_HEIGHT 16

class CCoin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();

public:
	CCoin(float X,float Y);
};
