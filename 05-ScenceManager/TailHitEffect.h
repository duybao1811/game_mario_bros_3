#pragma once
#include "Effect.h"
#include "define.h"
class TailHitEffect : public Effect
{
public:
	TailHitEffect(float X, float Y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

