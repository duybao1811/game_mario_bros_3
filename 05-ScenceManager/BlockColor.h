#pragma once
#include "GameObject.h"
class CBlockColor : public CGameObject
{

public:
	CBlockColor(float w, float h);
	virtual void Render(){}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
