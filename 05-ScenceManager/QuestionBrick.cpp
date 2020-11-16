#include "QuestionBrick.h"
#include "Mario.h"
CQuestionBrick::CQuestionBrick(int objtype)
{
	type = Type::QUESTION_BRICK;
	SetHealth(1);
	this->QuestionBrickType = objtype;
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (Health == 1)
	{
		state = QB_STATE_BASE;
	}
	if (Health <= 0)
	{
		state = QB_STATE_UNBOX;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario *mario=dynamic_cast<CMario*>(e->obj);
				if (state = QB_STATE_BASE  && Health ==1)
				{
					if (ny < 0)
					{
						isUnbox = true;
						SubHealth(1);
					}
				}
			}

		}
	}

}
void CQuestionBrick::Render()
{
	int ani = QB_ANI_BASE;
	if (state == QB_STATE_UNBOX)
	{
		ani = QB_ANI_AFTER_UNBOX;
	}
	animation_set->at(ani)->Render(x, y);
//	RenderBoundingBox();
}
void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QB_STATE_BASE:
		break;
	case QB_STATE_UNBOX:
		break;
	}
}
void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QUESTIONBRICK_BBOX_WIDTH;
	b = y + QUESTIONBRICK_BBOX_HEIGHT;
}
