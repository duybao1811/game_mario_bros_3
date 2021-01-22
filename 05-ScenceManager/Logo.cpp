#include "Logo.h"
#include "define.h"
Logo::Logo(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::LOGO;
}
void Logo::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
}
void Logo::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}