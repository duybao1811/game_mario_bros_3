#include "Goomba.h"
#include "Fire.h"
CGoomba::CGoomba(float X,float Y,int Model, int d)
{
	this->x = X;
	this->y = Y;
	model = Model;
	objType = ObjectType::ENEMY;
	eType = Type::GOOMBA;
	isKilled = false;
	direction = d;
	this->startX = X;
	this->startY = Y;
	TimeWalk = 0;
	TimeDisappear = 0;
	JumpCount = 0;
	SetState(GOOMBA_STATE_WALKING);
	switch (Model)
	{
	case GOOMBA_BASE:
		SetHealth(1);
		this->fullhealth = 1;
		break;
	case GOOMBA_RED_PARA:
		SetHealth(2);
		this->fullhealth = 2;
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
		if (isFinish)
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
		if (isAttacked)
		{
			left = top = right = bottom = 0;
		}
}
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += GOOMBA_GRAVITY * dt;
	if(model == GOOMBA_RED_PARA && Health == 2 && isOnGround && TimeWalk==0&&JumpCount!=3)
	{
		TimeWalk += dt;
		SetState(GOOMBA_RED_PARA_STATE_JUMP_SLOW);
	}
	
	if (isOnAir && isOnGround)
	{
		JumpCount++;
		isOnAir = false;
	}
	if (JumpCount == 3)
	{
		SetState(GOOMBA_STATE_JUMP);
		JumpCount = 0;
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
		isKilled = true;
	}
	if (isKilled)
	{
		vx = 0;
		state = GOOMBA_STATE_DIE;
		if (TimeDisappear <= 100)
		{
			TimeDisappear += dt;
		}
		else
		{
			isFinish = true;
		}
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
				TimeWalk = 0;
				vy = 0;
			}
			if (ny > 0)
			{
				isOnGround = false;
			}

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->obj->GetType() == BLOCK_COLOR|| e->obj->GetObjType() == ENEMY) // if e->obj is Goomba 
				{
					if (e->nx != 0)
					{
						x += dx;
					}
				}
				else
				{
					if (e->nx != 0)
					{
						vx *= -1;
						direction *= -1;
					}
				}
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
	else
	{
		if (model == GOOMBA_BASE) {
			if (state == GOOMBA_STATE_DIE) {
				ani = GOOMBA_ANI_DIE;
			}
			if (isAttacked)
			{
				ani = GOOMBA_ANI_ATTACKED;
			}
		}
		if (model == GOOMBA_RED_PARA)
		{
			if (state == GOOMBA_RED_PARA_STATE_JUMP_SLOW)
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
	{	y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		isDie = true;
		break;
	}
	case GOOMBA_STATE_WALKING:
	{	vx = direction * GOOMBA_WALKING_SPEED;
		break;
	}
	case GOOMBA_STATE_JUMP:
	{	isOnGround = false;
		vy = -GOOMBA_JUMP_SPEED_Y;
		break;
	}
	case GOOMBA_RED_PARA_STATE_JUMP_SLOW:
		    vy = -0.2f;
			vx = direction*GOOMBA_RED_WALKING_SPEED;
			isOnAir = true;
			break;
	case GOOMBA_RED_STATE_WALKING:
	{
		vx = direction * GOOMBA_RED_WALKING_SPEED;
		isFinish = false;
		break;
	}
	case ENEMY_ATTACKED:
	{
		vx = -0.3f;
		vy = -0.3f;
		isAttacked = true;
		break;
	}
	}
}
