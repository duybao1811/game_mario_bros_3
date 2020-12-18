#pragma once
#include "GameObject.h"
#include "define.h"
class GoldBrick : public CGameObject
{
	vector<LPGAMEOBJECT> listObj;
public:
	bool isUnbox;
	bool isEmpty;
	GoldBrick(float X, float Y,int Model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void Transform();
};

