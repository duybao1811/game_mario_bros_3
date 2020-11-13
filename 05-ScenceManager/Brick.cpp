#include "Brick.h"
CBrick::CBrick(float w,float h)
{
	type = Type::BRICK;
	this->width = w;
	this->height = h;
}
void CBrick::Render()
{
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
