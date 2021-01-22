#include "Box.h"
#include "PlayScence.h"
Box::Box()
{
	eType = Type::BOX;
}
void Box::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOX_BBOX_WIDTH;
	bottom = y + BOX_BBOX_HEIGHT;
	if (isFinish)
		left = top = right = bottom = 0;
}
void Box::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isGameDone)
	{
		x = 2740;
		y = 290;
	}
}
void Box::Render()
{
	if (isFinish)
		return;
	int ani = BOX_ANI_RANDOM;
	if (state == BOX_STATE_MUSHROOM)
		ani = BOX_ANI_MUSHROOM_UP;
	if (state == BOX_STATE_FLOWER)
		ani = BOX_ANI_FLOWER_UP;
	if (state == BOX_STATE_STAR)
		ani = BOX_ANI_STAR_UP;
	if (((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->isGameDone)
	{
		ani -= 3;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}