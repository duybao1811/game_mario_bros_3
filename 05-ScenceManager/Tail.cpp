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
				if (direction > 0)
				{
					ListEffect.push_back(new TailHitEffect(x + TAIL_BBOX_WIDTH, y));
				}
				if (direction < 0)
				{
					ListEffect.push_back(new TailHitEffect(x-TAIL_BBOX_WIDTH, y ));
				}
			}
		}
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->SetDirection(this->direction);
		ListEffect[i]->Update(dt,coObjects);
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		if (ListEffect[i]->GetFinish() == true)
		{
			ListEffect.erase(ListEffect.begin() + i);
		}
	}
}
void Tail::Render()
{
	RenderBoundingBox();
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
}
