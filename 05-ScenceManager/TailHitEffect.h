#pragma once
#include "Effect.h"
#define ID_LOAD_TAIL_HIT_EFFECT 19
#define ANI_TAIL_HIT_EFFECT 0
class TailHitEffect : public Effect
{
public:
	TailHitEffect(float X, float Y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

