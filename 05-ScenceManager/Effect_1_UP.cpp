#include "Effect_1_UP.h"
Effect_1_UP::Effect_1_UP(float X, float Y)
{
	this->x = X;
	this->y = Y;
	minY = Y - EEFECT_1_UP_MIN_Y;
	vy = -EEFECT_1_UP_SPEED_Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_EFFECT_1_UP));
}
void Effect_1_UP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (y <= minY)
	{
		SetFinish(true);
	}
}
void Effect_1_UP::Render()
{
	int ani = EFFECT_1_UP_ANI;
	animation_set->at(ani)->Render(x, y);
}