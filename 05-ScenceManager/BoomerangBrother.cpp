#include "BoomerangBrother.h"
BoomerangBrother::BoomerangBrother(int d,vector<Boomerang*>* listBoomerangEnemy)
{
	this->direction = d;
	eType = Type::BOOM_BROTHER;
	objType = ObjectType::ENEMY;
	this->ListBoomerangEnemy = listBoomerangEnemy;
	SetHealth(1);
	isWalking = false;
	isAttack = false;
	SetState(BOOM_BROTHER_STATE_WALKING);
}
void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	x += dx;
	vy += BOOM_BROTHER_GRAVITY * dt;
	TimeAttackDelay += dt;
	TimeAttack += dt;
	if (TimeAttackDelay > TIME_WALKING)
	{
		TimeAttackDelay = 0;
		SetState(BOOM_BROTHER_STATE_ATTACK);
		if (ListBoomerangEnemy->size() < 1)
		{
			CreateBoomerang();
		}
		TimeAttack += dt;
	}
	if (TimeAttack > TIME_ATTACK)
	{
		TimeAttack = 0;
		SetState(BOOM_BROTHER_STATE_WALKING);
		TimeAttackDelay += dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.4f;
		if (ny < 0)
		{
			vy = 0;
		}
	}
}
void BoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOM_BROTHER_STATE_WALKING:
		vx = direction * BOOM_BROTHER_SPEED_X * dt;
		break;
	case BOOM_BROTHER_STATE_ATTACK:
		vx = direction * BOOM_BROTHER_SPEED_X * dt;
		break;
	}
}
void BoomerangBrother::Render()
{
	if (isFinish)
		return;
	marioRange = GetMarioRangeCurrent();
	int ani = BOOM_BROTHER_ANI_WALKING_LEFT;
	if (state ==BOOM_BROTHER_STATE_WALKING)
	{
		if (marioRange == RIGHT_SIDE)
		{
			ani = BOOM_BROTHER_ANI_WALKING_RIGHT;
		}
		else if (marioRange == LEFT_SIDE)
		{
			ani = BOOM_BROTHER_ANI_WALKING_LEFT;
		}
	}
	if (state == BOOM_BROTHER_STATE_ATTACK)
	{
		if (marioRange == RIGHT_SIDE)
		{
			ani = BOOM_BROTHER_ANI_ATTACK_RIGHT;
		}
		else if (marioRange == LEFT_SIDE)
		{
			ani = BOOM_BROTHER_ANI_ATTACK_LEFT;
		}
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void BoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isFinish)
		left = top = right = bottom = 0; 
	left = x;
	top = y;
	right = x + BOOM_BROTHER_BBOX_WIDTH;
	bottom = y + BOOM_BROTHER_BBOX_HEIGHT;
}
Range BoomerangBrother::GetMarioRangeCurrent()
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->x < this->x)
	{
		direction = -1;
		return LEFT_SIDE;
	}
	if (mario->x > this->x)
	{
		direction = 1;
		return RIGHT_SIDE;
	}
}
void BoomerangBrother::CreateBoomerang()
{
	marioRange = GetMarioRangeCurrent();
	Boomerang* boomerang = new Boomerang(x, y, marioRange);
	ListBoomerangEnemy->push_back(boomerang);
}