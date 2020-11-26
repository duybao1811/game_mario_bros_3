#pragma once
#include "GameObject.h"
#include "Item.h"
#define MUSHROOM_RED 1
#define MUSHROOM_GREEN 2
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_RED_ANI 0
#define MUSHROOM_GREEN_ANI 1
#define MUSHROOM_GRAVITY 0.2f
#define MUSHROOM_SPEED 0.1f
#define MUSHROOM_SPEED_APPEAR_Y 0.03
#define MUSHROOM_STATE_APPEAR 100
#define MUSHROOM_STATE_WALKING 200
#define LOAD_MUSHROOM_FROM_TXT 8
class CMushRoom : public Item
{
public:
	float startY;
	float MushRoom_Gravity;
	int model;
	bool isWalking;
	float minY;
	bool isFinish;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CMushRoom(int Model, float X, float Y);
	void SetState(int state);
	int GetModel() { return model; }
};

