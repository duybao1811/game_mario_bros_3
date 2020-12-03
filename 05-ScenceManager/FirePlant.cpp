#include "FirePlant.h"
FirePlant::FirePlant(float X, float Y,Range marioRange)
{
	this->x = X;
	this->y = Y;
	eType = Type::FIRE_ENEMY;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_FIRE_FROM_TXT));
	isFinish = false;
	SetFireFly(marioRange); // SET ĐƯỜNG ĐẠN BAY
}
void FirePlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt,coObjects);
	x += dx;
	y += dy;
	if (!(checkObjInCamera(this))) // RA KHỎI CAM THÌ XÓA
	{
		SetFinish(true);
	}
}
void FirePlant::Render()
{
	int ani = SHOOT_FIRE_LEFT;
	if (direction > 0)
	{
		ani = SHOOT_FIRE_RIGHT;
	}
	animation_set->at(ani)->Render(x, y);
}
void FirePlant::SetFireFly(Range marioArea)
{
	switch (marioArea)
	{
	case LEFT_TOP_SIDE_NEAR:
		vy = -FIRE_ENEMY_SPEED_Y_NEAR;
		vx = -FIRE_ENEMY_SPEED_X_NEAR;
		direction = -1;
		break;
	case LEFT_TOP_SIDE_FAR:
		vy = -FIRE_ENEMY_SPEED_Y_FAR ;
		vx = -FIRE_ENEMY_SPEED_X_FAR;
		direction = -1;
		break;
	case LEFT_BOTTOM_SIDE_NEAR:
		vy = FIRE_ENEMY_SPEED_Y_NEAR;
		vx = -FIRE_ENEMY_SPEED_X_NEAR;
		direction = -1;
		break;
	case LEFT_BOTTOM_SIDE_FAR:
		vy = FIRE_ENEMY_SPEED_Y_FAR;
		vx = -FIRE_ENEMY_SPEED_X_FAR;
		direction = -1;
		break;
	case RIGHT_TOP_SIDE_NEAR:
		vy = -FIRE_ENEMY_SPEED_Y_NEAR;
		vx = FIRE_ENEMY_SPEED_X_NEAR;
		direction = 1;
		break;
	case RIGHT_TOP_SIDE_FAR:
		vy = -FIRE_ENEMY_SPEED_Y_FAR;
		vx = FIRE_ENEMY_SPEED_X_FAR;
		direction = 1;
		break;
	case RIGHT_BOTTOM_SIDE_NEAR:
		vy = FIRE_ENEMY_SPEED_Y_NEAR;
		vx = FIRE_ENEMY_SPEED_X_NEAR;
		direction = 1;
		break;
	case RIGHT_BOTTOM_SIDE_FAR:
		vy = FIRE_ENEMY_SPEED_Y_FAR;
		vx = FIRE_ENEMY_SPEED_X_FAR;
		direction = 1;
		break;

	}
}
void FirePlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIRE_BBOX_WIDTH;
	bottom = top + FIRE_BBOX_HEIGHT;
}