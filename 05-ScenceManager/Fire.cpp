#include "Fire.h"
#include "define.h"
#include "Camera.h"
#include "Brick.h"
#include "Goomba.h"
Fire::Fire()
{
	// load animation từ txt
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	SetAnimationSet(ani_set);
	SetHealth(1);
}
void Fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//set chuyển động của fire 
		vy += FIRE_GRAVITY * dt;
		vx = direction*FIRE_SPEED;

	if (Health == 0)
	{
		isFinish = true;
	}
	CGameObject::Update(dt);
	//nảy lên khi va chạm đất
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Fire*>(e->obj))
			{
				Fire* fire = dynamic_cast<Fire*>(e->obj);
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (nx != 0)
				{
					SubHealth(1);
				}
				if (ny != 0)
				{
					this->vy = -FIRE_BOUNCE;
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (e->nx != 0)
				{
					goomba->SetState(GOOMBA_STATE_ATTACKED);
					SubHealth(1);
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Fire::Render()
{
	int ani = SHOOT_FIRE_LEFT;
	if (isFinish)
		return;
	if (direction > 0)
	{
		ani = SHOOT_FIRE_RIGHT;
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