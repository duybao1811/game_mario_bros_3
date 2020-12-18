#include "P_Switch.h"
P_Switch::P_Switch(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::P_SWITCH;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_PSWITCH_FROM_TXT));
}
void P_Switch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isUsed)
		return;
	left = x;
	right = x + PSWITCH_BBOX_WIDTH;
	top = y;
	bottom = y + PSWITCH_BBOX_HEIGHT;
}
void P_Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{

}
void P_Switch::Render()
{
	int ani = PSWITCH_ANI;
	if (isUsed)
		ani = PSWITCH_ANI_USED;
	animation_set->at(ani)->Render(x, y);
}
void P_Switch::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PSWITCH_STATE_USED:
		isUsed = true;
		break;
	}
}