#include "MushRoom.h"
CMushRoom::CMushRoom(int Model, float X,float Y)
{
	this->model = Model;
	this->x = X;
	this->y = Y;
	direction = 1; 
	minY = Y - MUSHROOM_BBOX_HEIGHT;
	SetState(MUSHROOM_STATE_APPEAR);
	switch(model)
	{
	case MUSHROOM_RED:
		eType = Type::MUSHROOM_POWER;
		break;
	case MUSHROOM_GREEN:
		eType = Type::MUSHROOM_1_UP;
		break;
	}
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_MUSHROOM_FROM_TXT));
}
void CMushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	Item::GetBoundingBox(left, top, right, bottom);
}
void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	CGameObject::Update(dt);
	if (y <= minY)
	{
		isWalking =true;
	}
	if (isWalking)
	{
		vx = direction*MUSHROOM_SPEED;
		vy = MUSHROOM_GRAVITY;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(listObject, coEvents);

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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == BLOCK_COLOR)
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
					direction *= -1;
					vx *= -1;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_APPEAR:
		vy = -MUSHROOM_SPEED_APPEAR_Y;
		break;
	}
}

void CMushRoom::Render()
{
	int ani = -1;
	if(model==MUSHROOM_RED)
		ani = MUSHROOM_RED_ANI;
	if (model == MUSHROOM_GREEN)
	{
		ani = MUSHROOM_GREEN_ANI;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}