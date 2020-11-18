#include "FirePiranhaPlant.h"
CFirePiranhaPlant::CFirePiranhaPlant()
{
	SetHealth(1);
}
void CFirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
void CFirePiranhaPlant::Render()
{
	if (isFinish)
		return;
	int ani = FPLANT_ANI_LEFT_TOP;
	if (isLeftBottom)
	{
		ani = FPLANT_ANI_LEFT_BOTTOM;
	}
	if (isAttackLeftTop)
	{
		ani = FPLANT_ANI_ATTACK_LEFT_TOP;
	}
	if (isAttackLeftBottom)
	{
		ani = FPLANT_ANI_ATTACK_LEFT_BOTTOM;
	}
	if (isRightTop)
	{
		ani = FPLANT_ANI_RIGHT_TOP;
	}
	if (isRightBottom)
	{
		ani = FPLANT_ANI_RIGHT_BOTTOM;
	}
	if (isAttackRightTop)
	{
		ani = FPLANT_ANI_ATTACK_RIGHT_TOP;
	}
	if (isAttackRightBottom)
	{
		ani = FPLANT_ANI_ATTACK_RIGHT_BOTTOM;
	}
	animation_set->at(ani)->Render(x, y);
}
void CFirePiranhaPlant::SetState(int state)
{
	switch (state)
	{
	}
}
void CFirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FPLANTWIDTH;
	bottom = y + FPLANTHEIGHT;
	if (isFinish)
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}
}