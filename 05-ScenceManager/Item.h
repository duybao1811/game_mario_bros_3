#pragma once
#include "GameObject.h"
class Item : public CGameObject
{
	bool isFinish;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)=0;
	virtual void Render()=0;
	bool GetFinish();
	void SetFinish(bool b);
	Item();
};

