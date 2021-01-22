#include "Cursor.h"
Cursor::Cursor(float X,float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::CURSOR;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_CURSOR_FROM_TXT));
}
void Cursor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}
void Cursor::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}
