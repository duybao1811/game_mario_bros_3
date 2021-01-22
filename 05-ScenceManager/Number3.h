#pragma once
#include "GameObject.h"
#define LOAD_NUMBER_3_FROM_TXT 54
class Number3 : public CGameObject
{
public:
	Number3(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

