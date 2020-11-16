#include "BlockColor.h"
CBlockColor::CBlockColor(float w, float h)
{
	type = Type::BLOCK_COLOR;
	this->width = w;
	this->height = h;
}
void CBlockColor::Render()
{
	RenderBoundingBox();
}

void CBlockColor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
