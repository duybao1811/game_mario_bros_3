#pragma once
#include "GameObject.h"
#define StarSymbol_ANI 0
#define ID_LOAD_StarSymbol 28
class StarSymbol : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; };
	StarSymbol(float X, float Y);
};
