#include "BackGroundIntro.h"
BackGroundIntro::BackGroundIntro(float X, float Y)
{
	this->x = X;
	this->y = Y;
	SetAnimationSet(CAnimationSets::GetInstance()->Get(LOAD_BACKGROUND_INTRO_FROM_TXT));
}
void BackGroundIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}
void BackGroundIntro::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}
