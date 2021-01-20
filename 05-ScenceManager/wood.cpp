#include "wood.h"
Wood::Wood()
{
	eType = Type::WOOD;
	SetState(WOOD_STATE_FLY);
}
void Wood :: GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + WOOD_BBOX_WIDTH;
	bottom = top + WOOD_BBOX_HEIGHT;

}
void Wood::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	vx = -WOOD_SPEED_X*dt;
	if (isCollisMario)
	{
		vy = WOOD_GRAVITY*dt;
		vx = 0;
	}
	if (checkObjInCamera(this))
	{
		SetFinish(true);
	}
}
void Wood::Render()
{
	int ani = WOOD_ANI;
	animation_set->at(ani)->Render(x, y);
//	RenderBoundingBox();
}
void Wood::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WOOD_STATE_FLY:

		break;
	case WOOD_STATE_FALLING:
		vy = WOOD_GRAVITY;
		vx = 0;
		break;
	}

}

