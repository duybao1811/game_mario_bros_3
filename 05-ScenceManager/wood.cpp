#include "wood.h"
Wood::Wood()
{
	
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
}
void Wood::Render()
{
	int ani = WOOD_ANI;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void Wood::SetState(int state)
{
	CGameObject::SetState(state);
}

