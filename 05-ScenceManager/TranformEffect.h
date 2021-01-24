#pragma once
#include "Effect.h"
#define TIME_EFFECT 300
#define ID_LOAD_EFFECT_TRANFORM_FROM_TXT 34
#define ANI_TRANFORM_EFFECT_RIGHT 0
#define ANI_TRANFORM_EFFECT_LEFT 1
class TranformEffect : public Effect
{
public:
	int direction;
	TranformEffect(float X , float Y ,int d);
	~TranformEffect() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

