#pragma once
#include "GameObject.h"

#define QUESTIONBRICK_BBOX_WIDTH  16
#define QUESTIONBRICK_BBOX_HEIGHT 16
#define QB_STATE_BASE 1
#define QB_STATE_UNBOX 2
#define QB_ANI_BASE 0
#define QB_ANI_AFTER_UNBOX 1
class CQuestionBrick : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	bool isUnbox;
	int QuestionBrickType;
	CQuestionBrick(int objtype);
	virtual void SetState(int state);
};
