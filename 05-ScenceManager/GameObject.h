#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "define.h"
#include "Animations.h"
#include "Game.h"
using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
protected:
	Type eType;
	ObjectType objType;
public:
	int Health;
	int fullhealth;
	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;
	int direction; // -1: hướng trái, 1: hướng phải
	int nx;	 
	int state;
	bool isFinish;
	float width;
	float height;
	bool isInCam;
	bool isKilled;
	bool isAttacked;
	bool isFireAttack;
	float startX, startY;
	DWORD dt; 
	int model;
	int ani;
	LPANIMATION_SET animation_set;

public: 
	CGame* cam;
	bool GetFinish() { return isFinish; }
	void SetFinish(bool b) {
		isFinish = b;
	}
	int GetModel() {
		return model;
	}
	void SetFullHealth(int fh);
	void SetVx(float Vx) { this->vx = Vx; }
	float GetVx() { return vx; }
	void SetX(float X) { x = X; }
	float GetX() { return x; }
	void SetY(float Y) { y = Y; }
	float GetY() { return y; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetHealth(int h);
	int GetHealth();
	void SubHealth(int subh);
	void SetDirection(int d);
	int GetDirection();
	int GetState() { return this->state; }
	float GetStartX() { return startX; }
	float GetStartY() { return startY; }
	bool GetInCam() { return isInCam; }
	void SetInCam(bool inCam) { isInCam = inCam; }
	Type GetType();
	void RenderBoundingBox();
	ObjectType GetObjType();
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	
	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();
	bool isCollisionObjectWithObject(CGameObject* obj);
	bool checkAABB(CGameObject* obj);
	bool checkObjInCamera(CGameObject* obj);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	~CGameObject();
};

