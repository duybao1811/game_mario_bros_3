#include "Coin.h"
CCoin::CCoin(float X,float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::COIN;
}
void CCoin::Render()
{
	int ani = COIN_ANI;
	if (isFinish)
		return;
	animation_set->at(ani)->Render(x, y);
}
void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + COIN_WIDTH;
	bottom = y + COIN_HEIGHT;
	if (isFinish)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}