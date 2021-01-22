#pragma once
#include "GameObject.h"
class GroundIntro : public CGameObject
{
public:
	GroundIntro(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

