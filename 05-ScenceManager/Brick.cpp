#include "Brick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Game.h"
#include "Item.h"
#include "Leaf.h"
#include "CoinEffect.h"
#include "PointEffect.h"
CBrick::CBrick(float X,float Y,float w,float h, int Btype)
{
	this->x = X;
	this->y = Y;
	this->bType = Btype;
	this->width = w;
	this->height = h;
	starty = Y;
	//Btype = 0 : nền
	//Loại khối có màu
	switch (bType)
	{
	case GROUND:
		break;
	case BLOCK_COLOR:
		break;
	case BRICK_MODEL_COIN:
		SetHealth(1);
		break;
	case BRICK_MODEL_POWER_UP:
		SetHealth(1);
		break;
	}
	eType = Type::BRICK;

}
void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	CoinEffect* coineffect;
	coineffect = new CoinEffect(x, y);
	//MARIO ĐỤNG TRÚNG QUESTION BRICK THÌ QUESTION BRICK NẢY LÊN RƠI RA ĐỒNG XU HOẶC ITEM

	if (bType == 0 || bType == 1)
		return;
	if (Health == 1)
	{
		state = QB_STATE_BASE;
	}
	if (Health == 0)
	{
		state = QB_STATE_EMPTY;
	}
	if (starty - y >= QB_UP)
	{
		vy = QB_SPEED_DOWN;
		switch (bType)
		{
		case BRICK_MODEL_COIN:
			ListEffect.push_back(coineffect);
			break;
		}
	}
	if (starty - y < 0)
	{
		y = starty;
		SetState(QB_STATE_EMPTY);
		LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
		CMario* mario = ((CPlayScene*)scence)->GetPlayer();
		Item* item;

		switch (bType)
		{
		case BRICK_MODEL_POWER_UP:
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				ListItem.push_back(new CMushRoom(MUSHROOM_RED,this->x,this->y));
				break;
			}
			else
			{
				ListItem.push_back(new Leaf(this->x, this->y));
				break;
			}
			
		}
		case BRICK_MODE_1_UP:
			ListItem.push_back(new CMushRoom(MUSHROOM_GREEN, this->x, this->y));
			break;


		}
	}
#pragma region Update Item and effect

	for (int i = 0; i < ListItem.size(); i++)
	{
		ListItem[i]->Update(dt, coObjects);
	}
	for (int i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt, coObjects);
	}
	for (int i = 0; i < ListEffect.size(); i++)
	{
		if (ListEffect[i]->isFinish)
		{
			ListEffect.erase(ListEffect.begin() + i);
			ListPointEffect.push_back(new PointEffect(coineffect->x, coineffect->y, POINT_TYPE_ONE_HUNDRED));
		}
	}
	for (int i = 0; i < ListPointEffect.size(); i++)
	{
		ListPointEffect[i]->Update(dt, coObjects);
	}
	for (int i = 0; i < ListPointEffect.size(); i++)
	{
		if (ListPointEffect[i]->isFinish)
		{
			ListPointEffect.erase(ListPointEffect.begin() + i);
		}
	}
#pragma endregion

}
void CBrick::Render()
{
	int ani = QB_ANI_BASE;
	if (state == QB_STATE_UNBOX)
	{
		ani = QB_ANI_EMPTY;
	}
	if (state == QB_STATE_EMPTY)
	{
		ani = QB_ANI_EMPTY;
	}
	if (bType == 0 || bType == 1)
	{
		return;
	}
	else {
		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	}
	for (int i = 0; i < ListItem.size(); i++)
	{
		ListItem[i]->Render();
	}
	for (int i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
	for (int i = 0; i < ListPointEffect.size(); i++)
	{
		ListPointEffect[i]->Render();
	}
}
void CBrick::SetState(int state)
{
	switch(state)
	{
	case QB_STATE_UNBOX:
		vy = -QB_SPEED_UP;
		break;
	case QB_STATE_EMPTY:
		vy = 0;
		break;
	}
}
int CBrick::GetModel()
{
	return bType;
}
void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
