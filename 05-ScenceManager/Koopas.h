#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.02f;
#define KOOPAS_BALL_SPEED    0.1f;
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DEFEND 300
#define KOOPAS_STATE_BALL  400
#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DEFEND 2
#define KOOPAS_ANI_BALL 3
#define KOOPAS_GRAVITY 0.1f
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	bool isWalking;
	bool isDefend;
	bool isBall;
	bool IsDie;
	CKoopas();
	virtual void SetState(int state);
};