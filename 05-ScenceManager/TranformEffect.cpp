#include "TranformEffect.h"
TranformEffect::TranformEffect(float X, float Y,int d)
{
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_EFFECT_TRANFORM_FROM_TXT));
	this->x = X;
	this->y = Y;
	this->direction = d;
	startEffect = 0;
	timeEffect = TIME_EFFECT;
}
void TranformEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Effect::Update(dt, coObjects);
}
void TranformEffect::Render()
{
	if (isFinish)
		return;
	int ani =ANI_TRANFORM_EFFECT_RIGHT;
	if (direction < 0)
	{
		int ani = ANI_TRANFORM_EFFECT_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
}
void TranformEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	Effect::GetBoundingBox(left, top, right, bottom);
}