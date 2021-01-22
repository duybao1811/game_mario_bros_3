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
}
void Box::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	CGameObject::Update(dt,coObjects);
	y += dy;
	if (((CPlayScene*)game->GetCurrentScene())->section == 1)
	{
		vy = -BOX_SPEED_UP;
	}
	if (((CPlayScene*)game->GetCurrentScene())->section == 2)
	{
		vy = 0;
		x = 2740;
		y = 290;
	}

}
void Box::Render()
{
	CGame* game = CGame::GetInstance();
	int ani = BOX_ANI_RANDOM;
	if (state == BOX_STATE_MUSHROOM)
		ani = BOX_ANI_MUSHROOM_UP;
	if (state == BOX_STATE_FLOWER)
		ani = BOX_ANI_FLOWER_UP;
	if (state == BOX_STATE_STAR)
		ani = BOX_ANI_STAR_UP;
	if (((CPlayScene*)game->GetCurrentScene())->section == 2)
	{
		ani -= NUMBER_OF_ITEM_IN_BOX;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}