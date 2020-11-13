#include "Fire.h"
#include "define.h"
#include "Camera.h"
#include "Brick.h"
Fire::Fire()
{
	// load animation từ txt
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(4);
	SetAnimationSet(ani_set);
}
void Fire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//set chuyển động của fire 
	vx = nx * FIRE_SPEED;
	CGameObject::Update(dt);

	vy += FIRE_GRAVITY * dt;
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
					x += dx;
					return;
				}
				if (ny != 0)
				{
					this->vy = -FIRE_BOUNCE;
				}
			}

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Fire::Render()
{
	int ani = SHOOT_FIRE_LEFT;

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