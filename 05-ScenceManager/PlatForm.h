#pragma once
#include "GameObject.h"
class CPlatform : public CGameObject
{

	float width, height;
public:
	CPlatform(float w, float h);
	virtual void Render() {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

