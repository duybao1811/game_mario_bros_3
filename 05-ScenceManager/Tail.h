#pragma once
#include "GameObject.h"
#include "define.h"
#define TAIL_BBOX_WIDTH 9
#define TAIL_BBOX_HEIGHT 6
class Tail : public CGameObject
{
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Tail();
};

