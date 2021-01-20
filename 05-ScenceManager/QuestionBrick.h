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
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetModel();
};
