#include "Power.h"
Power::Power(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_POWER));
}
void Power::Render()
{
	int ani = POWER_ANI;
	animation_set->at(ani)->Render(x, y);
}