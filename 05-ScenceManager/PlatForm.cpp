#include "PlatForm.h"
CPlatform::CPlatform(float w, float h)
{
	this->width = w;
	this->height = h;
}
void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b) 
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}