#include "Floor.h"
#include "define.h"
Floor::Floor(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::FLOOR;
}
void Floor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
}
void Floor::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}