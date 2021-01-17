#include "Box.h"
Box::Box()
{
	eType = Type::BOX;
}
void Box::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 26;
	bottom = y + 26;
	if (isFinish)
		left = top = right = bottom = 0;
}
void Box::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

}
void Box::Render()
{
	if (isFinish)
		return;
	ani = BOX_ANI_RANDOM;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}
void Box::SetState(int state)
{

}