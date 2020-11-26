#pragma once
#include "GameObject.h"
#define SHORT_PIPE 1
#define MEDIUM_PIPE 2
#define ANI_SHORT_PIPE 0
#define ANI_MEDIUM_PIPE 1
#define PIPE_BBOX_WIDTH 32
#define PIPE_SHORT_BBOX_HEIGHT 32 
#define PIPE_MEDIUM_BBOX_HEIGHT 48
class Pipe : public CGameObject
{
public:
	int model;
	Pipe(int Model);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

