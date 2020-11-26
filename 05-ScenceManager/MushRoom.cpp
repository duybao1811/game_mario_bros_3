#include "MushRoom.h"
#include "Brick.h"
CMushRoom::CMushRoom(int Model, float X,float Y)
{
	model = Model;
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
	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}
void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (y <= minY)
	{
		isWalking =true;
	}
	if (isWalking)
	{
		vx = MUSHROOM_SPEED;
		vy = MUSHROOM_GRAVITY;
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == BRICK)
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0)
				{
					nx *= -1;
					vx *= -1;
				}
				if (brick->GetModel() == BLOCK_COLOR)
				{
					if (e->nx != 0)
						x += dx;
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
	int ani = MUSHROOM_RED_ANI;
	if (model == MUSHROOM_GREEN)
	{
		ani = MUSHROOM_GREEN_ANI;
	}
	animation_set->at(ani)->Render(x, y);
}