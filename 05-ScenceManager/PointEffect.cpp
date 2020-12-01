#include "PointEffect.h"
PointEffect::PointEffect(float X, float Y, int Model)
{
	this->x = X;
	this->y = Y;
	this->model = Model;
	minY = Y - POINT_EFFECT_MIN_Y;
	vy = -POINT_EFFECT_SPEED_Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_POINT_EFFECT));
}

void PointEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (y <= minY)
	{
		SetFinish(true);
	}
}
void PointEffect::Render()
{
	int ani = -1;
	switch (model)
	{
	case POINT_EFFECT_TYPE_ONE_HUNDRED:
		ani = POINT_EFFECT_TYPE_ONE_HUNDRED_ANI;
		break;
	case POINT_EFFECT_TYPE_TWO_HUNDRED:
		ani = POINT_EFFECT_TYPE_TWO_HUNDRED_ANI;
		break;
	case POINT_EFFECT_TYPE_FOUR_HUNDRED:
		ani = POINT_EFFECT_TYPE_FOUR_HUNDRED_ANI;
		break;
	case POINT_EFFECT_TYPE_EIGHT_HUNDRED:
		ani = POINT_EFFECT_TYPE_EIGHT_HUNDRED_ANI;
		break;
	case POINT_EFFECT_TYPE_ONE_THOUSAND:
		ani = POINT_EFFECT_TYPE_ONE_THOUSAND_ANI;
		break;
	case POINT_EFFECT_TYPE_TWO_THOUSAND:
		ani = POINT_EFFECT_TYPE_TWO_THOUSAND_ANI;
		break;
	case POINT_EFFECT_TYPE_FOUR_THOUSAND:
		ani = POINT_EFFECT_TYPE_FOUR_THOUSAND_ANI;
		break;
	case POINT_EFFECT_TYPE_EIGHT_THOUSAND:
		ani = POINT_EFFECT_TYPE_EIGHT_THOUSAND_ANI;
		break;
	}
	animation_set->at(ani)->Render(x, y);
}
int PointEffect::GetModel()
{
	return model;
}