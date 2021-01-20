#pragma once
#include "GameObject.h"
#define BOOMERANG_ANI 0
#define BOOMERANG_BBOX_WIDTH 15
#define BOOMERANG_BBOX_HEIGHT 16
#define LOAD_BOOMERANG_FROM_TXT 31
class Boomerang : public CGameObject
{
public:
	float startX, startY;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetBoomerangFly(Range marioRange);
	Boomerang(float X, float Y, Range marioRange);
	Range MarioRange;
	~Boomerang() {}
};

