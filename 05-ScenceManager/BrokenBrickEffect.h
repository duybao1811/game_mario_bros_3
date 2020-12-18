#pragma once
#include "Effect.h"
#define LOAD_EFFECT_BROKEN_BRICK 21
#define EFFECT_BROKEN_BRICK_ANI 0
#define EFFECT_BROKEN_BRICK_SPEED_X 0.08f
#define EFFECT_BROKEN_BRICK_SPEED_Y 0.25f
#define EFFECT_BROKEN_BRICK_GRAVITY 0.0009f
class BrokenBrickEffect : public Effect
{
public:
	float deflectY;
	BrokenBrickEffect(float X, float Y,int d,float defY);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

