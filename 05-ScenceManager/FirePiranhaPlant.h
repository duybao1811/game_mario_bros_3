#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
class CFirePiranhaPlant: public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	float maxY;
	float minY;
	vector<LPGAMEOBJECT> ListFireEnemy;
	Range marioRange;
	CMario* mario;
	DWORD TimeAttackDelay;
	DWORD TimeHidding;
	DWORD TimeAttack;
	bool isHidding;
	bool isGrowUp;
	int model;
	void CreateFire();
	Range GetMarioRangeCurrent(); //lấy vị trí hiện tại của mario để set đạn bay, set ani của plant
	bool GetSafeZone(); // mario ở vùng an toàn thì không bắn
	CFirePiranhaPlant(float X,float Y,int Model);
	virtual void SetState(int state);
};

