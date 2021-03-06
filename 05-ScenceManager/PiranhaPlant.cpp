﻿#include "PiranhaPlant.h"
CPiranhaPlant::CPiranhaPlant(float X, float Y, int Model)
{
	this->x = X;
	this->y = Y;
	this->model = Model;
	TimeHidding = 0;
	TimeGrowUp = 0;
	minY = Y - PLANT_BBOX_HEIGHT;
	maxY = Y;
	SetState(PLANT_STATE_GROW_UP);
	switch (model)
	{
	case 1:                // piranha plant green
		eType = Type::PIRANHA_GREEN;
		objType = ObjectType::ENEMY;
		SetHealth(1);
		break;
	}
}
void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isFinish)
		return;
	y += dy;
	if (y <= minY)
	{
		y = minY;
		vy = 0;
		TimeGrowUp += dt;
	}
	else if (y >= maxY)
	{
		y = maxY;
		vy = 0;
		TimeHidding += dt;
	}
	if (GetSafeZone() == false && TimeHidding > TIME_DELAY_GROW_UP)
	{
		SetState(PLANT_STATE_GROW_UP);
		TimeHidding = 0;
	}
	if (TimeGrowUp > TIME_DELAY_HIDE)
	{
		SetState(PLANT_STATE_HIDDING);
		TimeGrowUp = 0;
	}
	if (Health <= 0)
	{
		isFinish = true;
	}


}
void CPiranhaPlant::Render()
{
	if (isFinish)
		return;
	int ani = FPLANT_ANI_LEFT_TOP;
	animation_set->at(ani)->Render(x, y);
}
void CPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANT_STATE_GROW_UP:
	{
		vy = -PLANT_SPEED_GROW_UP * dt;
		break;
	}
	case PLANT_STATE_HIDDING:
	{
		vy = PLANT_SPEED_HIDDING * dt;
		break;
	}
	case ENEMY_STATE_FIRE_ATTACK:
	{
		isFinish = true;
		break;
	}
	case ENEMY_ATTACKED:
	{
		isFinish = true;
		break;
	}
	}
}
void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + PLANT_BBOX_WIDTH;
	bottom = y + PLANT_BBOX_HEIGHT;
	if (isFinish)
		left = top = right = bottom = 0;
}
bool CPiranhaPlant::GetSafeZone()
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->x < this->x)
	{
		if (this->x - mario->x <= DISTANCE_SAFE_ZONE)   // nếu khoảng cách từ mario đến plant trong vùng an toàn thì plant không grow up
		{
			return true;
		}
	}
	if (mario->x > this->x)
	{
		if (mario->x - this->x <= DISTANCE_SAFE_ZONE)
		{
			return true;
		}
	}
	return false;
}
int CPiranhaPlant::GetModel()
{
	return model;
}