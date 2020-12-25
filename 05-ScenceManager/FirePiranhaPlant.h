#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "FirePlant.h"
class CFirePiranhaPlant: public CGameObject
{
public:
	float maxY;
	float minY;
	vector<FirePlant*>* ListFireEnemy;
	Range marioRange;
	CMario* mario;
	DWORD TimeAttackDelay;
	DWORD TimeHidding;
	DWORD TimeAttack;
	bool isHidding;
	bool isGrowUp;
	bool isSafe;
	int model;
	CFirePiranhaPlant(float X, float Y, int Model, vector<FirePlant*>* listFireEnemy);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CreateFire();
	Range GetMarioRangeCurrent(); //lấy vị trí hiện tại của mario để set đạn bay, set ani của plant
	bool GetSafeZone(); // mario ở vùng an toàn thì không bắn
	virtual void SetState(int state);

};

