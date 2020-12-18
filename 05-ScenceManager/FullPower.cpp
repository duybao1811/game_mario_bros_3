#include "FullPower.h"
FullPower::FullPower(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_FULL_POWER));
}
void FullPower::Render()
{
	int ani = FULL_POWER_ANI;
	animation_set->at(ani)->Render(x, y);
}