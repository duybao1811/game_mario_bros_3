#include "Brick.h"
#include "Mario.h"
CBrick::CBrick(float w,float h, int Btype)
{

	this->BType = Btype;
	this->width = w;
	this->height = h;
	//Btype = 0 : nền
	//Loại khối có màu
	if (Btype == 1)
	{

	}
	if (Btype == 2)
	{
		SetHealth(1);
	}
	// loại question brick ra nấm 
	if (Btype == 3)
	{
		SetHealth(1);
	}
	//loại question brick ra nhiều tiền
	if (Btype == 4)
	{

	}
	type = Type::BRICK;

}
void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	//MARIO ĐỤNG TRÚNG QUESTION BRICK THÌ QUESTION BRICK NẢY LÊN RƠI RA ĐỒNG XU HOẶC ITEM

	if (Health == 1)
	{
		state = QB_STATE_BASE;
	}
	if (Health == 0)
	{
		state = QB_STATE_EMPTY;
	}
	if (vy == 0)
	{
		 starty = y;
	}
	if (starty - y >= QB_UP)
	{
		vy = QB_SPEED_UP;
	}
	if (starty - y < 0)
	{
		y = starty;
		SetState(QB_STATE_EMPTY);
	}
	y += dy;


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
	if (BType == 0 || BType == 1)
	{
		return;
	}
	else {
		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
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
	return BType;
}
void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
