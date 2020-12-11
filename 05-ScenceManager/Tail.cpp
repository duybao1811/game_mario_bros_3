#include "Tail.h"
Tail::Tail()
{

}
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isFinish)
	{
		left = x;
		top = y;
		right = x + TAIL_BBOX_WIDTH;
		bottom = y + TAIL_BBOX_HEIGHT;
	}

}
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt); 
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->GetType() == QUESTION_BRICK)
		{
			if (checkAABB(e))
			{
				if (e->GetState() != QB_STATE_EMPTY)
				{
					e->SetState(QB_STATE_UNBOX);
					e->SubHealth(1);
				}
			}
		}
		if (e->GetType() == GOOMBA )
		{
			if (checkAABB(e))
			{
				e->SetState(GOOMBA_STATE_ATTACKED);
			}
		}
	}
}
void Tail::Render()
{
	RenderBoundingBox();
}