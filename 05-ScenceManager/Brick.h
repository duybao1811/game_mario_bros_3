#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define QB_STATE_BASE 1
#define QB_STATE_EMPTY 2
#define QB_STATE_UNBOX 3
#define QB_ANI_BASE 1
#define QB_ANI_EMPTY 2
#define QB_UP 5 //Khoảng mà brick tưng lên được khi va chạm với mario
#define QB_SPEED_UP 0.02
class CBrick : public CGameObject
{
	
	float width, height;
public:
	float starty;
	bool isUnbox;
	int BType = 0;
	CBrick(float w,float h, int Btype);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	int GetModel();
};