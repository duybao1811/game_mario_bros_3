#include "Number3.h"
Number3::Number3(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_NUMBER_3_FROM_TXT));
}
void Number3::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}
void Number3::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}
