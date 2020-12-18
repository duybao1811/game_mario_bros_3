#pragma once
#include "GameObject.h"
#include "define.h"
#include "TailHitEffect.h"
#include "BrokenBrickEffect.h"
#include "EffectDisappear.h"
class Tail : public CGameObject
{
	vector<LPGAMEOBJECT> ListEffect;
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Tail();
};

