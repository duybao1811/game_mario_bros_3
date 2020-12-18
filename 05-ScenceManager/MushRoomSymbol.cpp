#include "MushRoomSymbol.h"
MushRoomSymbol::MushRoomSymbol(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_MushRoomSymbol));
}
void MushRoomSymbol::Render()
{
	int ani = MushRoomSymbol_ANI;
	animation_set->at(ani)->Render(x, y);
}