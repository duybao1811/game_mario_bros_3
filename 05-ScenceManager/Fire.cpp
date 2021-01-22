#include "Fire.h"
#include "define.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Game.h"
#include "PlatForm.h"
#include "BlockColor.h"
Fire::Fire(float X,float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::FIRE;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FIRE_FROM_TXT));
	isFinish = false;
}
void Fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//set chuyển động của fire 
	if (isFinish)
		return;
		vy += FIRE_GRAVITY * dt;
		vx = direction*FIRE_SPEED*dt;

	// ra khỏi camera thì kết thúc
	if (!checkObjInCamera(this))
	{
		SetFinish(true);
	}
	CGameObject::Update(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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
		y += min_ty * dy + ny * 0.1f;
		if (ny != 0)
		{
			//nảy lên khi va chạm đất
			this->vy = -FIRE_BOUNCE_SPEED_Y;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->nx != 0 || e->ny != 0)
			{
				if (e->obj->GetObjType() == ENEMY)
				{

					e->obj->SetState(ENEMY_STATE_FIRE_ATTACK);
					ListEffect.push_back(new EffectDisappear(e->obj->GetX(), e->obj->GetY() + FPLANT_RED_BBOX_HEIGHT / 2));
					SetFinish(true);
				}
			}
				if (e->obj->GetType()==BLOCK_COLOR)
				{
					if (e->nx != 0)
					{
						x += dx;
					}
				}
				else
				{
					// chạm vào object thì kết thúc
					if (e->nx != 0)
					{
						SetFinish(true);
					}
				}

		}
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->SetDirection(this->direction);
		ListEffect[i]->Update(dt, coObjects);
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		if (ListEffect[i]->GetFinish() == true)
		{
			ListEffect.erase(ListEffect.begin() + i);
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Fire::Render()
{
	int ani = SHOOT_FIRE_LEFT;
	if (direction > 0)
	{
		ani = SHOOT_FIRE_RIGHT;
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}
void Fire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}