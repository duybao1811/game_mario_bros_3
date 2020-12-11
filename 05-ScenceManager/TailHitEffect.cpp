#include "TailHitEffect.h"
TailHitEffect::TailHitEffect(float X, float Y)
{
	this->x = X;
	this->y = Y;

	timeEffect = 1000;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_TAIL_HIT_EFFECT));

}
void TailHitEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Effect::Update(dt, coObjects);
}
void TailHitEffect::Render()
{
	animation_set->at(ANI_TAIL_HIT_EFFECT)->Render(x, y);
}