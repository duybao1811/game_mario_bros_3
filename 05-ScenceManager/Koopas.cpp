﻿#include "Koopas.h"
#include "Goomba.h"
CKoopas::CKoopas(int Model, int d)
{
	model = Model;
	objType = ObjectType::ENEMY;
	eType = Type::KOOPAS;
	SetState(KOOPAS_STATE_WALKING);
	SetHealth(3);
	direction = d;
	switch (Model)
	{
	case KOOPAS_BASE:
		SetHealth(3);
		break;
	case KOOPAS_RED:
		SetHealth(3);
		break;
	case KOOPAS_FLY:
		SetHealth(4);
		break;
	}
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_BALL || state == KOOPAS_STATE_DEFEND || state==KOOPAS_STATE_ATTACKED || isUpside )
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
	
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += KOOPAS_GRAVITY * dt;
	CGameObject::Update(dt, coObjects);
	// ra khỏi camera thì kết thúc
	if (!(checkObjInCamera(this)))
		SetFinish(true);
	if (model == KOOPAS_FLY && Health == 4 && isOnGround)
	{
		SetState(KOOPAS_STATE_FLY);
	}
	if (Health <= 0)
	{
		SetFinish(true);
	}
	if (Health == 3)   //Koopas
	{
		SetState(KOOPAS_STATE_WALKING);
	}
	if (Health == 2)
	{
		SetState(KOOPAS_STATE_DEFEND);
	}
	if (Health == 1)
	{
		if (state != KOOPAS_STATE_BALL)
			SetState(KOOPAS_STATE_BALL);
	}
	//bị lật ngược thì nằm im
	if (isUpside)
	{
		vx = 0;
	}
	if (isAttacked)
	{
		state = KOOPAS_STATE_ATTACKED;
		vx = direction * 0.1f;
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
			isOnGround = true;
			vy = 0;
		}
		if (ny > 0)
		{
			isOnGround = false;
		}

		if (state == KOOPAS_STATE_ATTACKED)
		{
			if (ny != 0)
			{
				vy = -KOOPAS_BOUNCE_AFTER_LANDFALL;
				isAttacked = false;
				isUpside = true;
			}
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == BLOCK_COLOR)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				if(e->ny <0 && state==KOOPAS_STATE_WALKING && model==KOOPAS_RED)
				{
					if (x <= e->obj->GetX() || x >= e->obj->GetX() + e->obj->width-KOOPAS_BBOX_WIDTH)
					{
						direction *= -1;
						vx *= -1;
					}
				}
			}
			else
			{
				if (e->nx != 0)
				{
					direction *= -1;
					vx *= -1;
				}
			}
			if (state == KOOPAS_STATE_BALL)
			{
				if (e->obj->GetType() == QUESTION_BRICK)
				{
					CQuestionBrick* questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (e->nx != 0)
					{
						if (questionbrick->GetState() != QB_STATE_EMPTY)
						{
							questionbrick->SetState(QB_STATE_UNBOX);
							questionbrick->SubHealth(1);
							direction *= -1;
							vx *= -1;
						}
					}
				}
			}
	
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_BASE_ANI_WALKING_LEFT;
	if (model == KOOPAS_BASE)
	{
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_BASE_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_BASE_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_BASE_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_BASE_ANI_WALKING_LEFT;
		}
		if (state == KOOPAS_STATE_ATTACKED)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
	}
	else if (model == KOOPAS_RED)
	{
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_RED_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_RED_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_RED_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_RED_ANI_WALKING_LEFT;
		}
		if (state == KOOPAS_STATE_ATTACKED)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}

	}
	else if (model == KOOPAS_FLY)
	{
		if (state == KOOPAS_STATE_FLY)
		{
			if (direction > 0)
			{
				ani = KOOPAS_BASE_ANI_FLY_RIGHT;
			}
			else if (direction < 0)
			{
				ani = KOOPAS_BASE_ANI_FLY_LEFT;
			}
		}
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_BASE_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_BASE_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_BASE_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_BASE_ANI_WALKING_LEFT;
		}
		if (state == KOOPAS_STATE_ATTACKED)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = direction*KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DEFEND:
		vx = 0;
		break;
	case KOOPAS_STATE_BALL:
		vx =  direction*KOOPAS_BALL_SPEED;
		break;
	case KOOPAS_STATE_ATTACKED:	
		isAttacked = true;
		vy = -KOOPAS_SPEED_Y_AFTER_ATTACKED;
		break;
	case KOOPAS_STATE_FLY:
		vx = direction * KOOPAS_FLY_SPEED_X;
		isOnGround = false;
		vy = -KOOPAS_FLY_SPEED_Y;
		break;
	}

}
int CKoopas::GetModel()
{
	return model;
}