#include "Flower.h"
Flower::Flower(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::FLOWER;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FLOWER_FROM_TXT));
}
void Flower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	Item::GetBoundingBox(left, top, right, bottom);
}
void Flower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (checkObjInCamera(this) == false)
	{
		SetFinish(true);
	}
}
void Flower::Render()
{
	int ani = FLOWER_ANI;
	animation_set->at(ani)->Render(x, y);
}
