#include "GoldBrick.h"
#include "Coin.h"
GoldBrick::GoldBrick(float X, float Y,int Model)
{
	this->x = X;
	this->y = Y;
	this->model = Model;
	SetHealth(1);
	eType = Type::GOLD_BRICK;
}
void GoldBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (Health == 0)
	{
		isFinish = true;
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
	int ani = GB_ANI;
	if (state==GB_STATE_EMPTY)
		ani = GB_ANI_UNBOX;
	animation_set->at(ani)->Render(x, y);
	for (UINT i = 0; i < listObj.size(); i++)
	{
		listObj[i]->Render();
	}
}
void GoldBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GB_STATE_EMPTY:
		isUnbox = true;
		break;
	}
}
void GoldBrick::Transform()
{
	listObj.push_back(new CCoin(this->x, this->y));
}
