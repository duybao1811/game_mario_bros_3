#include "StarSymbol.h"
StarSymbol::StarSymbol(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_StarSymbol));
}
void StarSymbol::Render()
{
	int ani = StarSymbol_ANI;
	animation_set->at(ani)->Render(x, y);
}
