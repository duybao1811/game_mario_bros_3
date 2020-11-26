#pragma once
#include "GameObject.h"
class Effect : public CGameObject
{
protected:
	DWORD startEffect;
	int timeEffect;
public:
	Effect();
	virtual ~Effect() {};
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

