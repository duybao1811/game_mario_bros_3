#include "GroundIntro.h"
GroundIntro::GroundIntro(float X, float Y)
{
	this->x = X;
	this->y = Y;
	eType = Type::GROUND_INTRO;
}
void GroundIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}
void GroundIntro::Render()
{
	ani = 0;
	animation_set->at(ani)->Render(x, y);
}
