#pragma once
#include "GameObject.h"
#include "Item.h"
#define LOAD_FLOWER_FROM_TXT 33
#define FLOWER_BBOX_WIDTH 16
#define FLOWER_BBOX_HEIGHT 16
#define FLOWER_ANI 0
class Flower : public Item
{
public:
	bool isUsed = true;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	Flower(float X, float Y);
};

