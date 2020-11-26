#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define QB_STATE_BASE 1
#define QB_STATE_EMPTY 2
#define QB_STATE_UNBOX 3
#define QB_ANI_BASE 1
#define QB_ANI_EMPTY 2
#define QB_UP 10 //Khoảng mà brick tưng lên được khi va chạm với mario
#define QB_SPEED_UP 0.1f
#define QB_SPEED_DOWN 0.1f
#define GROUND 0
#define BLOCK_COLOR 1
#define BRICK_MODEL_COIN 2 // question brick ra tiền
#define BRICK_MODEL_POWER_UP 3 //question brick ra nấm đỏ hoặc lá
#define BRICK_MODE_1_UP 4 //question brick ra nấm xanh 1 up
class CBrick : public CGameObject
{
	float width, height;
	float starty;
	bool isUnbox;
	vector<LPGAMEOBJECT> ListItem;
	vector<LPGAMEOBJECT> ListEffect;
	vector<LPGAMEOBJECT> ListPointEffect;

public:

	int bType = 0;
	CBrick(float X,float Y,float w,float h, int Btype);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	int GetModel();
};