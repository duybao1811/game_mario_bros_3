#pragma once
#include "GameObject.h"
#define WORLD_MAP_ANI 0
class WorldMap : public CGameObject
{
public:
	WorldMap(float X, float Y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) { l = t = r = b = 0; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
};

