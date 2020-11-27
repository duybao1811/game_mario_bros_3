#include "Fire.h"
#include "define.h"
#include "Camera.h"

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
		vy += FIRE_GRAVITY * dt;
		vx = direction*FIRE_SPEED*dt;

	// ra khỏi camera thì kết thúc
	if (!(checkObjInCamera(this)))
		SetFinish(true);
	CGameObject::Update(dt, coObjects);
	//nảy lên khi va chạm đất
	// chạm vào object thì kết thúc
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
			this->vy = -FIRE_BOUNCE_SPEED_Y;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (e->nx != 0)
				{
					goomba->SetState(GOOMBA_STATE_ATTACKED);
				}
			}
			else 
				if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Goomba 
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

					if (e->nx != 0)
					{
						if (koopas->GetState() == KOOPAS_STATE_WALKING) {
							//koopas->SetState(GOOMBA_STATE_ATTACKED);
							koopas->SubHealth(1);
						}
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
					if (e->nx != 0)
					{
						SetFinish(true);
					}
				}
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
	animation_set->at(ani)->Render(x, y);
}
void Fire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}