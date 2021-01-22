#pragma once
#include "GameObject.h"
#define LOAD_BACKGROUND_INTRO_FROM_TXT 53
class BackGroundIntro : public CGameObject
{
public:
	BackGroundIntro(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

