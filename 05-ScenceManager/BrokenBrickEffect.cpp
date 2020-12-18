#include "BrokenBrickEffect.h"
BrokenBrickEffect::BrokenBrickEffect(float X, float Y,int d,float defY)
{
	this->x = X;
	this->y = Y;
	this->direction = d;
	this->deflectY = defY;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_EFFECT_BROKEN_BRICK));
	vx = direction * EFFECT_BROKEN_BRICK_SPEED_X;
	vy = -deflectY * EFFECT_BROKEN_BRICK_SPEED_Y;
}
void BrokenBrickEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += EFFECT_BROKEN_BRICK_GRAVITY*dt;
	x += dx;
	y += dy;
	if (checkObjInCamera(this)==false)
	{
		isFinish = true;
	}
}
void BrokenBrickEffect::Render()
{
	if (isFinish)
		return;
	int ani = EFFECT_BROKEN_BRICK_ANI;
	animation_set->at(ani)->Render(x, y);
}