#pragma once
#include "GameObject.h"
#define FlowerSymbol_ANI 0
#define ID_LOAD_FlowerSymbol 27
class FlowerSymbol : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; };
	FlowerSymbol(float X, float Y);
};

