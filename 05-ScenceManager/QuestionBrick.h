#pragma once
#include "GameObject.h"
#include "define.h"
class CQuestionBrick : public CGameObject
{
	float starty;
	float minY;
public:
	int model;
	bool isFalling;
	bool isUnbox;
	bool isEmpty;
	CQuestionBrick(float X, float Y,int Model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int GetModel();
};
