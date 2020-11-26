#include "Leaf.h"
Leaf::Leaf(float X,float Y)
{
	this->x = X;
	this->y = Y;
	vy = -LEAF_MIN_Y;
	eType = Type::LEAF;
    SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_LEAF_FROM_TXT));
	minY = Y - LEAF_MAX_Y;
	limitLeft = X;
	limitRight = X + LEAF_MAX_RIGHT_X;
}
void Leaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}
void Leaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (y <= minY)
	{
		isFalling = true;
	}
	if (isFalling)
	{
		if (x <= limitLeft)
		{
			vy = LEAFT_GRAVITY;
			vx = LEAF_SPEED_X;
		}
		if (x >= limitRight)
		{
			vy = LEAFT_GRAVITY;
			vx = -LEAF_SPEED_X;
		}
	}
}
void Leaf::Render()
{
	int ani = LEAF_ANI_LEFT;
	if (vx > 0)
		ani = LEAF_ANI_RIGHT;
	animation_set->at(ani)->Render(x, y);
}
void Leaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_APPEAR:
		vy = -LEAF_MIN_Y;
	}
}