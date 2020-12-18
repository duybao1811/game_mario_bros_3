#include "Floor.h"
Floor::Floor(float X, float Y)
{
	this->x = X;
	this->y = Y;
	vy = -FLOOR_SPEED_Y;
}
void Floor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (checkObjInCamera(this) == false)
		SetFinish(true);
}
void Floor::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}