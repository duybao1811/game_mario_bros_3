#include "CoinEffect.h"
#include "PointEffect.h"
CoinEffect::CoinEffect(float X, float Y)
{
	this->x = X + SET_EFFECT_IN_BRICK_X;
	this->y = Y - SET_EFFECT_IN_BRICK_Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(ID_LOAD_COIN_EFFECT));
	eType = Type::COIN_EFFECT;
	minY = Y - COIN_EFFECT_MIN_Y;
	maxY = Y - COIN_EFFECT_MAX_Y;
	vy = -COIN_EFFECT_SPEED_Y;
	timeEffect = 1000;
	isFalling = 0;
}
void CoinEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
//	Effect::Update(dt, coObjects);
	y += dy;
	if (y <= minY)
	{
		vy = COIN_EFFECT_SPEED_Y;
		isFalling = true;
	}
	if (isFalling)
	{
		if (y >= maxY)
		{
			SetFinish(true);
		}
	}

}
void CoinEffect::Render()
{
	int ani = COIN_EFFECT_ANI;
	animation_set->at(ani)->Render(x, y);
}