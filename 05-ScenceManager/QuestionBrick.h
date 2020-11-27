#pragma once
#include "GameObject.h"
#define QB_BBOX_WIDTH 16
#define QB_BBOX_HEIGHT 16
#define QB_ANI 0
#define QB_ANI_EMPTY 1
#define	QB_MODEL_1UP_ANI 2
#define BRICK_MODEL_COIN 1
#define BRICK_MODEL_POWER_UP 2
#define QB_MINY 10
#define QB_SPEED_DOWN 0.1f
#define QB_SPEED_UP 0.1f
#define QB_STATE_UNBOX 100
#define QB_STATE_EMPTY 200
class CQuestionBrick : public CGameObject
{
	int model;
	float starty;
	float minY;
public:
	CQuestionBrick(float X, float Y,int Model);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};
