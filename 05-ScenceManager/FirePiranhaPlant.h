#pragma once
#include "GameObject.h"
#define FPLANTWIDTH 16
#define FPLANTHEIGHT 23
#define	FPLANT_ANI_LEFT_TOP 0
#define FPLANT_ANI_LEFT_BOTTOM 1
#define FPLANT_ANI_RIGHT_TOP 2
#define FPLANT_ANI_RIGHT_BOTTOM 3
#define FPLANT_ANI_ATTACK_LEFT_TOP 4
#define FPLANT_ANI_ATTACK_LEFT_BOTTOM 5
#define FPLANT_ANI_ATTACK_RIGHT_TOP 6
#define FPLANT_ANI_ATTACK_RIGHT_BOTTOM 7
class CFirePiranhaPlant: public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	bool isFinish;
	bool isAttackLeftTop;
	bool isAttackRightTop;
	bool isAttackLeftBottom;
	bool isAttackRightBottom;
	bool isLeftTop;
	bool isRightTop;
	bool isLeftBottom;
	bool isRightBottom;
	CFirePiranhaPlant();
	virtual void SetState(int state);
};

