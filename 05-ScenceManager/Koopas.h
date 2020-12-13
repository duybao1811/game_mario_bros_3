#pragma once

#include "GameObject.h"
#include "BlockColor.h"
#include "QuestionBrick.h"
#include "Mario.h"
#define KOOPAS_WALKING_SPEED 0.02f
#define KOOPAS_BALL_SPEED    0.1f
#define KOOPAS_FLY_SPEED_X   0.02f
#define KOOPAS_FLY_SPEED_Y 0.3f
#define KOOPAS_SPEED_Y_AFTER_ATTACKED 0.4f
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DEFEND 300
#define KOOPAS_STATE_BALL  400
#define KOOPAS_STATE_ATTACKED 500
#define KOOPAS_STATE_FLY 600
#define KOOPAS_BASE_ANI_WALKING_LEFT 0
#define KOOPAS_BASE_ANI_WALKING_RIGHT 1
#define KOOPAS_BASE_ANI_DEFEND 2
#define KOOPAS_BASE_ANI_BALL 3
#define KOOPAS_BASE_ANI_ATTACKED 4
#define KOOPAS_RED_ANI_WALKING_LEFT 5
#define KOOPAS_RED_ANI_WALKING_RIGHT 6
#define KOOPAS_RED_ANI_DEFEND 7
#define KOOPAS_RED_ANI_BALL 8
#define KOOPAS_RED_ANI_ATTACKED 9
#define KOOPAS_BASE_ANI_FLY_RIGHT 10 // MODEL 3
#define KOOPAS_BASE_ANI_FLY_LEFT 11
#define KOOPAS_BASE_ANI_COME_BACK 12
#define KOOPAS_RED_ANI_COME_BACK 13
#define KOOPAS_BASE_ANI_COME_BACK_UPSIDE 14
#define KOOPAS_RED_ANI_COME_BACK_UPSIDE 15
#define KOOPAS_GRAVITY 0.001f
#define KOOPAS_BOUNCE_AFTER_LANDFALL 0.1f
#define KOOPAS_BASE 1
#define KOOPAS_RED 2
#define KOOPAS_FLY 3
class CKoopas : public CGameObject
{
	CMario* player;
	vector<LPGAMEOBJECT> ListEffect;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int model;
	int GetModel();
	bool isHeld;
	bool isKicked;
	float KoopasGravity;
	bool isWalking;
	bool isDefend;
	bool isBall;
	bool IsDie;
	bool isAttacked;
	bool isUpside;
	bool isFlying;
	bool isOnGround;
	CKoopas(CMario* mario,int Model,int d);
	void SetPositionFollowMario();
	virtual void SetState(int state);
};