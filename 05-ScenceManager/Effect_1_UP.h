#pragma once
#include "Effect.h"
#define EFFECT_1_UP_ANI 0
#define ID_LOAD_EFFECT_1_UP 23
#define EEFECT_1_UP_MIN_Y 30
#define EEFECT_1_UP_SPEED_Y 0.05f
class Effect_1_UP : public Effect
{
private:
	float minY;
	float maxY;
	bool isFalling;
public:
	Effect_1_UP(float X, float Y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

