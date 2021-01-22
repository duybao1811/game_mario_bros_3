#pragma once
#include "GameObject.h"
#define POSITION_LOGO_IN_INTRO 24
#define LOAD_LOGO_FROM_TXT 52
#define LOGO_WIDTH 128
#define LOGO_SPEED 0.3f
class Logo : public CGameObject
{
public:
	Logo(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

