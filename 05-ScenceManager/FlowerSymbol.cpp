#include "FlowerSymbol.h"
FlowerSymbol::FlowerSymbol(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_FlowerSymbol));
}
void FlowerSymbol::Render()
{
	int ani = FlowerSymbol_ANI;
	animation_set->at(ani)->Render(x, y);
}