#pragma once
#include "Effect.h"
#define TIME_EFFECT 300
#define ID_LOAD_EFFECT_DISAPPEAR_FROM_TXT 10
#define EFFECT_DISAPPEAR_ANI 0
class EffectDisappear : public Effect
{
public:
	EffectDisappear(float X=0, float Y=0);
	~EffectDisappear() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

