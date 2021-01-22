#include "Koopas.h"
#include "Goomba.h"
#include "GoldBrick.h"
CKoopas::CKoopas(float X, float Y, CMario* mario,int Model, int d)
{
	player = mario;
	model = Model;
	objType = ObjectType::ENEMY;
	eType = Type::KOOPAS;
	direction = d;
	TimeDefend = 0;
	TimeComeBack = 0;
	this->startX = X;
	this->startY = Y;
	switch (Model)
	{
	case KOOPAS_RED:
	case KOOPAS_BASE:
		SetHealth(3);
		this->fullhealth = 3;
		break;
	case KOOPAS_RED_FLY:
	case KOOPAS_FLY:
		SetHealth(4);
		this->fullhealth = 4;
		break;
	}
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_BALL || state == KOOPAS_STATE_DEFEND || isAttacked || isUpside|| state==KOOPAS_STATE_COMEBACK||state==KOOPAS_STATE_UPSIDE_BALL )
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;

	if (isFinish)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;
	now = GetTickCount64();


	if (Health == 3)   //Koopas
	{
		SetState(KOOPAS_STATE_WALKING);
	}
	if (Health == 2)
	{
		vx = 0;
		SetState(KOOPAS_STATE_DEFEND);
	}
	if (Health == 1)
	{
		SetState(KOOPAS_STATE_BALL);
		if (isReadyUpsideBall)
		{
			SetState(KOOPAS_STATE_UPSIDE_BALL);
		}
	}
	if (isHeld && player->isHoldTurtle == true)
	{
		SetPositionFollowMario();
		vy = 0;
	}
	if (isHeld && player->isHoldTurtle == false)
	{
		player->isKick = true;
		player->TimeKick = GetTickCount64();
		SetDirection(player->nx);
		SetHealth(1);
		isHeld = false;
	}
	if (TimeDefend > TIME_DEFEND)
	{
		SetState(KOOPAS_STATE_COMEBACK);
		TimeComeBack += dt;
	}
	if (TimeComeBack > TIME_COMEBACK)
	{
		TimeDefend = 0;
		TimeComeBack = 0;
		y -= KOOPAS_BBOX_HEIGHT_DIE;
		SetHealth(3);
		isHeld = false;
		isUpside = false;
		isReadyUpsideBall = false;
	}
	if (isUpside)
	{
		vx = 0;
		TimeDefend += dt;
		isReadyUpsideBall = true;
	}
	if (isAttacked)
	{
		TimeDefend = 0;
	}
	if (model == KOOPAS_FLY && Health == 4 && isOnGround)
	{
		vx = direction * KOOPAS_FLY_SPEED_X;
		isOnGround = false;
		vy = -KOOPAS_FLY_SPEED_Y;
	}
	if (isUp)
	{
		vy = -KOOPAS_RED_FLY_Y * dt;
	}
	if (isDown)
	{
		vy = KOOPAS_RED_FLY_Y * dt;
	}
	if (model == KOOPAS_RED_FLY && Health == 4 )
	{
		vx = 0;
		if (y <= startY )
		{
			isUp = false;
			isDown = true;
		}
		if (y >= startY + MAX_Y)
		{
			isUp = true;
			isDown = false;
		}

	}
	else
	{
		isUp = false;
		isDown = false;
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

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.4f;
		if (ny < 0)
		{
			isOnGround = true;
			vy = 0;
		}
		if (ny > 0)
		{
			isOnGround = false;
		}

		if (isAttacked)
		{
			if (ny != 0)
			{
				vy = -KOOPAS_BOUNCE_AFTER_LANDFALL;
				isAttacked = false;
				isUpside = true;
			}
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->obj->GetType() == BLOCK_COLOR)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				if(e->ny <0 && state==KOOPAS_STATE_WALKING && model==KOOPAS_RED)
				{
					if (x <= e->obj->GetX()-14  || x >= e->obj->GetX() + e->obj->width-KOOPAS_BBOX_WIDTH+14)
					{
						direction *= -1;
						vx *= -1;
					}
				}
			}
			if (e->obj->GetType() == GOLD_BRICK)
			{
				if (e->ny < 0 && state == KOOPAS_STATE_WALKING && model == KOOPAS_RED)
				{
					if (x <= e->obj->GetX() -10 || x + KOOPAS_BBOX_WIDTH >= e->obj->GetX() + GB_BBOX_WIDTH + 10 )
					{
						vy = 0;
						direction *= -1;
						vx *= -1;
					}
				}
			}
			if (e->obj->GetType() == EMEDIUM_PIPE || e->obj->GetType() == ESHORT_PIPE || e->obj->GetType()==PLATFORM)
			{
				if (e->nx != 0)
				{
					direction *= -1;
					vx *= -1;
				}
			}
			if (state == KOOPAS_STATE_BALL || state==KOOPAS_STATE_UPSIDE_BALL)
			{
				if (e->obj->GetType() == QUESTION_BRICK)
				{
					CQuestionBrick* questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (e->nx != 0)
					{
						direction *= -1;
						vx *= -1;
						if (questionbrick->GetHealth() != 0)
						{
							questionbrick->vy = -QB_SPEED_UP;
							questionbrick->SubHealth(1);
						}
					}
				}
				if (e->obj->GetObjType() == ENEMY)
				{
					if (e->nx != 0)
					{
						x += dx;
						e->obj->SetState(ENEMY_ATTACKED);
						if (direction > 0)
						{
							ListEffect.push_back(new TailHitEffect(this->x+KOOPAS_BBOX_WIDTH,this->y));
						}
						if (direction < 0)
						{
							ListEffect.push_back(new TailHitEffect(this->x, this->y));
						}
					}
				}
				if (e->obj->GetType() == GOLD_BRICK)
				{
					GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(e->obj);
					if (state==KOOPAS_STATE_BALL || state==KOOPAS_STATE_UPSIDE_BALL)
					{
						if (e->nx != 0)
						{
							direction *= -1;
							vx *= -1;
							switch (e->obj->GetModel())
							{
							case GB_CONTAIN_COIN:
								e->obj->SubHealth(1);
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
							}
						}
					}
				}
			}
	
		}
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt,coObjects);
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		if (ListEffect[i]->GetFinish() == true)
		{
			ListEffect.erase(ListEffect.begin() + i);
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CKoopas::SetPositionFollowMario()
{
	if (player->GetLevel() == MARIO_LEVEL_SMALL)
	{
		if (player->nx > 0)
			SetPosition(player->x + 19, player->y + 2);
		else
			SetPosition(player->x - 3, player->y + 2);
	}
	else if (player->GetLevel() == MARIO_RACCOON)
	{
		if (player->nx > 0)
			SetPosition(player->x + 19, player->y+5);
		else
			SetPosition(player->x - 13, player->y+5);
	}
	else if (player->GetLevel() == MARIO_LEVEL_BIG || player->GetLevel() == MARIO_FIRE)
	{
		if (player->nx > 0)
			SetPosition(player->x + 12, player->y+3);
		else
			SetPosition(player->x-14, player->y+3);
	}
}
void CKoopas::Render()
{
	if (isFinish)
		return;
	int ani = KOOPAS_BASE_ANI_WALKING_LEFT;
	if (model == KOOPAS_BASE)
	{
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_BASE_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_BASE_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_BASE_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_BASE_ANI_WALKING_LEFT;
		}
		if (isAttacked)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (state == KOOPAS_STATE_COMEBACK)
		{
			ani = KOOPAS_BASE_ANI_COME_BACK;
			if (isUpside)
				ani = KOOPAS_BASE_ANI_COME_BACK_UPSIDE;
		}
		if (state == KOOPAS_STATE_UPSIDE_BALL)
		{
			ani = KOOPAS_BASE_ANI_UPSIDE_BALL;
		}
	}
	else if (model == KOOPAS_RED)
	{
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_RED_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_RED_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_RED_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_RED_ANI_WALKING_LEFT;
		}
		if (isAttacked)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}
		if (state == KOOPAS_STATE_COMEBACK)
		{
			ani = KOOPAS_RED_ANI_COME_BACK;
			if (isUpside)
				ani = KOOPAS_RED_ANI_COME_BACK_UPSIDE;
		}
		if (state == KOOPAS_STATE_UPSIDE_BALL)
		{
			ani = KOOPAS_RED_ANI_UPSIDE_BALL;
		}

	}
	if (model == KOOPAS_RED_FLY)
	{
		if (Health == 4)
		{
			if (direction > 0)
			{
				ani = KOOPAS_RED_FLY_ANI_RIGHT;
			}
			else if (direction < 0)
			{
				ani = KOOPAS_RED_FLY_ANI_LEFT;
			}
		}
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_RED_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_RED_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_RED_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_RED_ANI_WALKING_LEFT;
		}
		if (isAttacked)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_RED_ANI_ATTACKED;
		}
		if (state == KOOPAS_STATE_COMEBACK)
		{
			ani = KOOPAS_RED_ANI_COME_BACK;
			if (isUpside)
				ani = KOOPAS_RED_ANI_COME_BACK_UPSIDE;
		}
		if (state == KOOPAS_STATE_UPSIDE_BALL)
		{
			ani = KOOPAS_RED_ANI_UPSIDE_BALL;
		}

	}
	else if (model == KOOPAS_FLY)
	{
		if ( Health == 4)
		{
			if (direction > 0)
			{
				ani = KOOPAS_BASE_ANI_FLY_RIGHT;
			}
			else if (direction < 0)
			{
				ani = KOOPAS_BASE_ANI_FLY_LEFT;
			}
		}
		if (state == KOOPAS_STATE_BALL)
		{
			ani = KOOPAS_BASE_ANI_BALL;
		}
		if (state == KOOPAS_STATE_DEFEND) {
			ani = KOOPAS_BASE_ANI_DEFEND;
		}
		if (state == KOOPAS_STATE_WALKING)
		{
			if (direction > 0)
				ani = KOOPAS_BASE_ANI_WALKING_RIGHT;
			else if (direction < 0)
				ani = KOOPAS_BASE_ANI_WALKING_LEFT;
		}
		if (state == KOOPAS_STATE_ATTACKED)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (isUpside)
		{
			ani = KOOPAS_BASE_ANI_ATTACKED;
		}
		if (state == KOOPAS_STATE_COMEBACK)
		{
			ani = KOOPAS_BASE_ANI_COME_BACK;
		}
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = direction*KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DEFEND:
		vx = 0;
		TimeDefend += dt;
		break;
	case KOOPAS_STATE_BALL:
		vx =  direction*KOOPAS_BALL_SPEED;
		TimeDefend = 0;
		break;
	case KOOPAS_STATE_UPSIDE_BALL:
		isUpside = false;
		vx = direction * KOOPAS_BALL_SPEED;
		TimeDefend = 0;
		break;
	case KOOPAS_STATE_COMEBACK:
		vx = 0;
		break;
	case ENEMY_ATTACKED:
		vx = direction*0.5f;
		vy = -0.3f;
		isAttacked = true;
		break;
	}

}
int CKoopas::GetModel()
{
	return model;
}