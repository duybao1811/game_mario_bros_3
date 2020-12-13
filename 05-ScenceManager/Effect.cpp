#include "Effect.h"
Effect::Effect()
{
	startEffect = 0;
}

void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
	if (startEffect < timeEffect)
	{
		startEffect += dt;
	}
	else
	{
		startEffect = 0;
		SetFinish(true);
	}
}
void Effect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = right = bottom = 0;
}
void Effect::Render()
{
}
