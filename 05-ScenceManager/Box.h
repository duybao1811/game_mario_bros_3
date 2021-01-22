#pragma once
#include "GameObject.h"
#include "define.h"
#define BOX_BBOX_WIDTH 26
#define BOX_BBOX_HEIGHT 26
#define BOX_STATE_MUSHROOM 100
#define BOX_STATE_FLOWER 200
#define BOX_STATE_STAR 300
#define BOX_ANI_MUSHROOM 1
#define BOX_ANI_FLOWER 2
#define BOX_ANI_STAR 3
#define BOX_ANI_MUSHROOM_UP 4
#define BOX_ANI_FLOWER_UP 5
#define BOX_ANI_STAR_UP 6
#define BOX_SPEED_UP 0.065f
#define NUMBER_OF_ITEM_IN_BOX 3
class Box : public CGameObject
{
public:
	
	int ani;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	Box();
	~Box() {}
};

