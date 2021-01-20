#include "GoldBrick.h"
#include "Coin.h"
GoldBrick::GoldBrick(float X, float Y,int Model)
{
	this->x = X;
	this->y = Y;
	this->model = Model;
	SetHealth(1);
	starty = Y;
	startX = X;
	minY = Y - QB_MINY;
	eType = Type::GOLD_BRICK;
	if (model == GB_MODEL_MANY_COIN)
	{
		SetHealth(8);
		this->fullhealth = 8;
	}
}
void GoldBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (Health == 0)
	{
		if (model == GB_CONTAIN_COIN)
		{
			isFinish = true;
		}
		if( model == GB_MODEL_MANY_COIN || model == GB_CONTAIN_MUSHROOM_1_UP || model == GB_CONTAIN_POWER_UP)
		{
			SetState(GB_STATE_EMPTY);
		}
	}
	if (y <= minY)
	{
		vy = QB_SPEED_UP;
		isUnbox = true;
	}
	if (y >= starty)
	{
		y = starty;
		vy = 0;
	}
}
void GoldBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + GB_BBOX_WIDTH;
	b = t + GB_BBOX_HEIGHT;
	if (isFinish)
		l = t = r = b = 0;
}
void GoldBrick::Render()
{
	if (isFinish)
		return;
	if (eType == Type::GOLD_BRICK)
	{
		ani = GB_ANI;
		if (state == GB_STATE_EMPTY)
			ani = GB_ANI_UNBOX;
	}
	else if (eType == Type::COIN)
	{
		ani = IDLE_COIN_ANI;
	}
	animation_set->at(ani)->Render(x, y);
}
void GoldBrick::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == GB_STATE_TRANFORM)
	{
		if (eType == Type::GOLD_BRICK)
		{
			eType = Type::COIN;
		}
		else if (eType == Type::COIN)
		{
			eType = Type::GOLD_BRICK;
		}
	}
}