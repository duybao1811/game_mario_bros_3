#include "EffectDisappear.h"
EffectDisappear::EffectDisappear(float X, float Y)
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_EFFECT_DISAPPEAR_FROM_TXT));
	this->x = X;
	this->y = Y;
	startEffect = 0;
	timeEffect = TIME_EFFECT;
}
void EffectDisappear::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Effect::Update(dt, coObjects);
}
void EffectDisappear::Render()
{
	int ani = EFFECT_DISAPPEAR_ANI;
	animation_set->at(ani)->Render(x, y);
}
void EffectDisappear::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	Effect::GetBoundingBox( left,  top, right,  bottom);
}