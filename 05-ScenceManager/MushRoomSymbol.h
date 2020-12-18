#pragma once
#include "GameObject.h"
#define MushRoomSymbol_ANI 0
#define ID_LOAD_MushRoomSymbol 26
class MushRoomSymbol : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { return; };
	MushRoomSymbol(float X, float Y);
};

