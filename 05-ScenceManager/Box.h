#pragma once
#include "GameObject.h"
#include "define.h"
class Box : public CGameObject
{
public:
	int ani;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	Box();
	~Box() {}
};

