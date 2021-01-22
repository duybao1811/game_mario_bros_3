#pragma once
#include "GameObject.h"
#define LOAD_CURSOR_FROM_TXT 56
class Cursor : public CGameObject
{
public:
	Cursor(float X,float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

