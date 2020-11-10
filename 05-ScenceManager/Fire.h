#pragma once
#include "GameObject.h"
#include "Camera.h"
#define FIRE_SPEED 0.2f
class Fire : public CGameObject
{
private: 
	Camera* camera;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Fire(Camera* camera);
	virtual ~Fire();
};

