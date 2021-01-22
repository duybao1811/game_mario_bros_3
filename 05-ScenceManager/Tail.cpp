#include "Tail.h"
#include "CoinEffect.h"
#include "GoldBrick.h"
Tail::Tail()
{
}
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x;
		top = y;
		right = x + TAIL_BBOX_WIDTH;
		bottom = y + TAIL_BBOX_HEIGHT;
		if (isFinish)
			left = top = right = bottom = 0;
}
void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isFinish)
	{
		ListEffect.clear();
		return;
	}
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (e->GetType() == QUESTION_BRICK)
		{
			if (this->checkAABB(e))
			{
				if (e->GetHealth() != 0)
				{
					e->vy = -QB_SPEED_UP;
					e->SubHealth(1);
				}
			}
		}
		if (e->GetObjType() == ENEMY)
		{
			if (this->checkAABB(e))
			{
				if (e->GetType() == FIRE_PIRANHA || e->GetType() == PIRANHA_GREEN)
				{
					e->SetState(ENEMY_ATTACKED);
					ListEffect.push_back(new EffectDisappear(e->GetX(), e->GetY() + FPLANT_RED_BBOX_HEIGHT / 2));
					ListEffect.push_back(new TailHitEffect(x , y - PULL_UP_EFFECT_TAIL_HIT));
				}
				else
				{
					e->SetDirection(direction);
					e->SetState(ENEMY_ATTACKED);
				}
			}
		}
		if (e->GetType() == GOLD_BRICK)
		{
			GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(e);
			if (checkAABB(e))
			{
				switch (e->GetModel())
				{
				case GB_CONTAIN_COIN:
					e->SubHealth(1);
					ListEffect.push_back(new BrokenBrickEffect(x, y, 1, 1));
					ListEffect.push_back(new BrokenBrickEffect(x, y, 1, 1.5));
					ListEffect.push_back(new BrokenBrickEffect(x, y, -1, 1));
					ListEffect.push_back(new BrokenBrickEffect(x, y, -1, 1.5));
					break;
				case GB_CONTAIN_PSWITCH:
				{
					if (goldbrick->GetState() != GB_STATE_EMPTY)
					{
						goldbrick->SetState(GB_STATE_EMPTY);
						goldbrick->isUnbox = true;
					}
				}
				break;
				case GB_CONTAIN_MUSHROOM_1_UP:
				{
					if (goldbrick->GetHealth() != 0)
					{
						goldbrick->vy = -QB_SPEED_UP;
						goldbrick->SubHealth(1);
					}
				}
				break;
				case GB_MODEL_MANY_COIN:
				{
					if (goldbrick->GetHealth() != 0)
					{
						goldbrick->vy = -QB_SPEED_UP;
						goldbrick->SubHealth(1);
						ListEffect.push_back(new CoinEffect(goldbrick->x, goldbrick->y));
					}
				}
				break;
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
	//RenderBoundingBox();
	if(isFinish)
		return;
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
}
