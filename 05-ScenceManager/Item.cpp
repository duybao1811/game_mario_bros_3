#include "Item.h"
Item::Item()
{
	isFinish = 0;
}
bool Item::GetFinish()
{
	return isFinish;
}
void Item::SetFinish(bool b)
{
	isFinish = b;
}
void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}