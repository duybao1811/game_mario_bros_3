#include "Goomba.h"
#include "Fire.h"
CGoomba::CGoomba(int Model, int d)
{
	model = Model;
	eType = Type::GOOMBA;
	SetState(GOOMBA_STATE_WALKING);
	direction = d;
	TimeWalk = 0;
	TimeDisappear = 0;
	JumpCount = 0;
	switch (Model)
	{
	case GOOMBA_BASE:
		SetHealth(1);
		break;
	case GOOMBA_RED_PARA:
		SetHealth(2);
		break;
	}
}
void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
	if (isFinish || isAttacked)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

}
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += 0.001f * dt;
	if(model == GOOMBA_RED_PARA && Health == 2 && isOnGround)
	{


	}
	if(isOnGround==false)
	{
		TimeWalk = 0;
	}
	if (Health == 1)
	{
		SetState(GOOMBA_STATE_WALKING);
	}
	if (Health == 0)
	{
		state=GOOMBA_STATE_DIE;
		vx = 0;
		vy = 0;
	}
	if (isDie)
	{
		vx = 0;
		state = GOOMBA_STATE_DIE;
		if (TimeDisappear <= 30)
		{
			TimeDisappear+=dt;
		}
		else
		{
			isFinish = true;
		}
	}
	vector<LPGAMEOBJECT> ListBrick;
	ListBrick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == Type::BRICK)
		{
			ListBrick.push_back(coObjects->at(i));
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	CalcPotentialCollisions(&ListBrick, coEvents);

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
		if (nx != 0)
		{
			vx *= -1;
			direction *= -1;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		/*	if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (brick->GetModel() == 1)
				{
					if (e->nx != 0)
					{
						x += dx;
					}
				}
			}*/
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (isFinish)
	{
		return;
	}
	if (model == GOOMBA_BASE) {
		if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_DIE;
		}
		if (state == GOOMBA_STATE_ATTACKED)
		{
			ani = GOOMBA_ANI_ATTACKED;
		}
	}
	if (model == GOOMBA_RED_PARA)
	{
		if (state == GOOMBA_RED_PARA_STATE_WALKING)
		{
			ani = GOOMBA_RED_PARA_ANI_WALKING;
		}
		if (state == GOOMBA_STATE_JUMP)
		{
			ani = GOOMBA_RED_PARA_ANI_JUMP;
		}
		if (state == GOOMBA_RED_PARA_STATE_FALLING)
		{
			ani = GOOMBA_RED_PARA_ANI_FALL;
		}
		if (state == GOOMBA_STATE_WALKING)
		{
			ani = GOOMBA_RED_ANI_WALKING;
		}
		if (state == GOOMBA_STATE_DIE)
		{
			ani = GOOMBA_RED_PARA_ANI_DIE;
		}
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			isDie = true;
			break;
		case GOOMBA_STATE_WALKING:
			vx = direction * GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_ATTACKED:
			vx = direction*GOOMBA_ATTACKED_SPEED_X;
			vy = -GOOMBA_ATTACKED_SPEED_Y;
			isAttacked = true;
			break;
		case GOOMBA_STATE_JUMP:
			isOnGround = false;
			vy = -GOOMBA_JUMP_SPEED_Y;
			break;
		case GOOMBA_RED_PARA_STATE_WALKING:
			vx = direction*GOOMBA_RED_WALKING_SPEED;
			break;
		case GOOMBA_RED_PARA_STATE_FALLING:
			break;
		case GOOMBA_RED_STATE_WALKING:
			vx = direction * GOOMBA_RED_WALKING_SPEED;
			break;
	}
}
