#pragma once
#include "GameObject.h"
#include "Item.h"
#define LOAD_LEAF_FROM_TXT 9
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16
#define LEAF_GRAVITY 0.01f
#define LEAF_ANI_LEFT 0
#define LEAF_ANI_RIGHT 1
#define LEAF_MAX_Y 32
#define LEAF_MAX_RIGHT_X 32
#define LEAF_MIN_Y 0.1f;
#define LEAF_STATE_APPEAR 100
#define LEAF_SPEED_X 0.05f
#define LEAFT_GRAVITY 0.05f
class Leaf : public Item
{
public:
	bool isFalling;
	float minY;
	float limitRight;
	float limitLeft;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	Leaf(float X,float Y);
	void SetState(int state);
};

