#include "Goomba.h"
#include "Brick.h"
#include "Fire.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	SetHealth(1);
	time = 0;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
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
	if (Health == 1)
	{
		vx = -GOOMBA_WALKING_SPEED;
	}
	if (Health <= 0)
	{
		state = GOOMBA_STATE_DIE;
		vx = 0;
		vy = 0;
	}
	if (isDie)
	{
		vx = 0;
		state = GOOMBA_STATE_DIE;
		if (time <= 30)
		{
			time++;
		}
		else
		{
			isFinish = true;
		}
	}
	if (isAttacked)
	{
		state = GOOMBA_STATE_ATTACKED;
	}


	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
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

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->obj->GetState()==GOOMBA_STATE_ATTACKED && ny<0)
				{
					y += 10;
				}
			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	if (isFinish)
	{
		return;
	}
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	if (state == GOOMBA_STATE_ATTACKED)
	{
		ani = GOOMBA_ANI_ATTACKED;
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
			break;
		case GOOMBA_STATE_ATTACKED:
			//vx = 0.1f;
			vy = -0.35f;
			isAttacked = true;
			break;
	}
}
