#include "BlockColor.h"
CBlockColor::CBlockColor(float w, float h)
{

	this->width = w;
	this->height = h;
	eType = Type::BLOCK_COLOR;
}
void CBlockColor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
