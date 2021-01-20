#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Boomerang.h"
#define BOOM_BROTHER_STATE_WALKING 100
#define BOOM_BROTHER_STATE_ATTACK 200
#define BOOM_BROTHER_ANI_WALKING_LEFT 0
#define BOOM_BROTHER_ANI_ATTACK_LEFT 1
#define BOOM_BROTHER_ANI_WALKING_RIGHT 2
#define BOOM_BROTHER_ANI_ATTACK_RIGHT 3
#define BOOM_BROTHER_SPEED_X 0.001f
#define BOOM_BROTHER_BBOX_WIDTH 16
#define BOOM_BROTHER_BBOX_HEIGHT 24
#define BOOM_BROTHER_GRAVITY 0.0001f
#define TIME_WALKING 6000
#define TIME_ATTACK 500
class BoomerangBrother : public CGameObject
{
	bool isAttack, isWalking;

public:

	vector<Boomerang*>* ListBoomerangEnemy;
	Range marioRange;
	CMario* mario;
	DWORD now;
	DWORD TimeAttackDelay;
	DWORD TimeHidding;
	DWORD TimeAttack;
	BoomerangBrother(int d, vector<Boomerang*>* listBoomerangEnemy);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void CreateBoomerang();
	Range GetMarioRangeCurrent(); //lấy vị trí hiện tại của mario để set boomerang bay, set ani 
	void SetState(int state);
};

