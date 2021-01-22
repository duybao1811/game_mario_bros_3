#include "Boomerang.h"
Boomerang::Boomerang(float X, float Y, Range marioRange)
{
	this->x = X;
	this->y = Y;
	eType = Type::BOOMERANG;
	isFinish = false;
	this->MarioRange = marioRange;
	this->startX = X;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_BOOMERANG_FROM_TXT));
	SetDirection(1);
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt,coObjects);
	x += dx;
	y += dy;
	vx = direction*0.008f * dt;
	vy = -0.0005f*dt;
	if (abs(startX - x) >= 160)
	{
		isTurning = true;
		vx *= -1;
		direction *= -1;
		vy = 0.046 * dt;
	}
	if (!(checkObjInCamera(this))) // RA KHỎI CAM THÌ XÓA
	{
		SetFinish(true);
	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->GetType() == BOOM_BROTHER)
		{
			if (checkAABB(e) && isTurning == true)
			{
				SetFinish(true);
			}
		}
	}
}
void Boomerang::SetBoomerangFly(Range marioRange)
{
}
void Boomerang::Render()
{
	int ani = BOOMERANG_ANI;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}