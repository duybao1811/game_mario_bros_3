#pragma once
#include "GameObject.h"
class Item : public CGameObject
{
	bool isFinish;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)=0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)=0;
	virtual void Render()=0;
	bool GetFinish();
	void SetFinish(bool b);
};

