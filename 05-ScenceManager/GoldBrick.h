#pragma once
#include "GameObject.h"
#include "define.h"
class GoldBrick : public CGameObject
{
	vector<LPGAMEOBJECT> listObj;
	DWORD TimeTranform;
public:
	float starty,startX, minY;
	bool isTranform;
	bool isUnbox;
	bool isEmpty;
	vector<LPGAMEOBJECT> ListEffect;
	GoldBrick(float X, float Y,int Model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};

