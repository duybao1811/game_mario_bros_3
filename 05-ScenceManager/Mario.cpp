#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Koopas.h"
#include "Fire.h"
#include "BlockColor.h"
#include "Coin.h"
#include "Item.h"
#include "EffectDisappear.h"
#include "MushRoom.h"
#include "QuestionBrick.h"
#include "PointEffect.h"
#include "d3dx9.h"
#include "GoldBrick.h"
#include "P_Switch.h"
#include "Box.h"
#include "wood.h"
#include "CoinEffect.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_RACCOON;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	SetHealth(1);
	tail = new Tail();
	isKick = false;
	this->camera = camera;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x < CGame::GetInstance()->GetCamX())
	{
		x = CGame::GetInstance()->GetCamX();
	}
	if (x + MARIO_BIG_BBOX_WIDTH > CGame::GetInstance()->GetCamX() - 16+SCREEN_WIDTH)
	{
		x = (float)(CGame::GetInstance()->GetCamX() - 16 + SCREEN_WIDTH - MARIO_BIG_BBOX_WIDTH);
	}
	if (y < CGame::GetInstance()->GetCamY())
	{
		y = CGame::GetInstance()->GetCamY();
	}
	// không cho ra khỏi camera
	now = GetTickCount64();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (CGame::GetInstance()->GetScene() != 1)
	{
		vy += MARIO_GRAVITY * dt;
	}

	if (isJumping)
	{
		state = MARIO_STATE_JUMP;
		if (vy > 0)
		{
			if (!isFalling) {
				isFalling = true;
			}
			state = MARIO_STATE_FALLING;
		}
	}
	if (isFalling && isSitting)
	{
		state = MARIO_STATE_SIT;
	}
	if (isKick)
	{
		state = MARIO_STATE_KICK;
	}
	if (isFalling && level != MARIO_LEVEL_RACCOON)
	{
		state = MARIO_STATE_FALLING;
	}
	if (isFlying)
	{
		state = MARIO_STATE_FLY_UP;
		if (isFalling && level == MARIO_LEVEL_RACCOON)
		{
			state = MARIO_STATE_FALL_FLY;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
#pragma region Set thời gian
	if (level == MARIO_LEVEL_RACCOON && GetTickCount64() - TimeAttack > 500 && isAttack)
	{
		//tail->SetFinish(true);
		isAttack = false;
		state = MARIO_STATE_IDLE;
		TimeAttack = now;
	}
	if (level == MARIO_LEVEL_FIRE && GetTickCount64() - TimeAttack > 300 && isAttack)
	{
		isAttack = false;
		state = MARIO_STATE_IDLE;
		TimeAttack = now;
	}
	if (GetTickCount64() - TimeKick > 230 && isKick)
	{
		isKick = false;
		TimeKick = now;
	}
	if (level == MARIO_LEVEL_RACCOON && TimeFly > 400 && isFlying)
	{
		isFlying = false;
		TimeFly = 0;
	}

#pragma endregion
#pragma region Set attack
	if (isAttack && level == MARIO_LEVEL_FIRE)
	{
		if (ListFire.size() < 2)
		{
			ShootFire();
			isAttack = false;
		}
	}
	if (isAttack && level == MARIO_LEVEL_RACCOON)
	{
		TailAttack();
	}

#pragma endregion
	if (vy)
	{
		last_vy = vy;
	}
	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (ny != 0)
		{
			if (ny < 0)
			{
				isOnGround = true;
				isJumping = false;
				isFlying = false;
				isFallFly = false;
				isFallSlow = false;
				isFlyup = false;
				isFalling = false;
				if (isFalling)
				{
					isFalling = false;
				}
			}
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == GOOMBA)
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->ny < 0)
				{
					vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					goomba->SubHealth(1);
					ListEffect.push_back(new PointEffect(goomba->GetX(), goomba->GetY(), POINT_EFFECT_TYPE_ONE_HUNDRED));
					score += 100;
				}
			}
			if (e->obj->GetType() == KOOPAS)
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0)
				{
					ListEffect.push_back(new PointEffect(koopas->GetX(), koopas->GetY(), POINT_EFFECT_TYPE_ONE_HUNDRED));
					score += 100;
					if (koopas->GetHealth() ==4)
					{
						koopas->SubHealth(1);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						koopas->SubHealth(1);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->SetDirection(this->nx);
						koopas->SubHealth(1);
						y += dy;
					}

					if (koopas->GetState() == KOOPAS_STATE_BALL || koopas->isKicked)
					{
						koopas->SetHealth(2);
						koopas->isKicked = false;
					}
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->SetDirection(this->nx);
						koopas->SetState(KOOPAS_STATE_BALL);
						y += dy;
					}

					if (koopas->GetState() == KOOPAS_STATE_UPSIDE_BALL|| koopas->isKicked)
					{
						koopas->isUpside = true;
						koopas->isKicked = false;
					}
				}
				if (koopas->GetState() == KOOPAS_STATE_DEFEND || koopas->GetState() == KOOPAS_STATE_UPSIDE_BALL || koopas->GetState() == KOOPAS_STATE_COMEBACK||koopas->isUpside)
				{
					if (e->nx != 0)
					{
						if (isRunning)
						{
							koopas->isHeld = true;
							isHoldTurtle = true;
						}
						else
						{
							isKick = true;
							TimeKick = GetTickCount64();
							koopas->SetDirection(this->nx);
							koopas->SubHealth(1);
						}
					}
				}

			}
			if (e->obj->GetType() == COIN)
			{

				if (e-> nx!= 0)
				{
					e->obj->SetFinish(true);
					x += dx;
					score +=50;
					CoinCollect++;
				}
				if (e->ny != 0)
				{
					vy = last_vy;
					e->obj->SetFinish(true);
					score += 50;
					CoinCollect++;
				}
			}
			if (e->obj->GetType() == BLOCK_COLOR)
			{
				if (e->nx != 0)
				{
					x += dx;
				}
				if (e->ny > 0)
				{
					vy = last_vy;
					y+=dy;
				}
			}
			if (e->obj->GetType() == QUESTION_BRICK)
			{
				CQuestionBrick* questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->ny > 0)
				{
					if (questionbrick->GetHealth() !=0)
					{
						questionbrick->vy = -QB_SPEED_UP;
						questionbrick->SubHealth(1);
					}
				}
			}
			if (e->obj->GetType() == GOLD_BRICK)
			{
				GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(e->obj);
				if (e->ny > 0)
				{
					if (level != MARIO_LEVEL_SMALL)
					{
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
						case GB_CONTAIN_POWER_UP:
						{
							if (goldbrick->GetHealth() != 0)
							{
								goldbrick->vy = -QB_SPEED_UP;
								goldbrick->SubHealth(1);
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
			if (e->obj->GetType() == P_SWITCH)
			{
				P_Switch* pswitch = dynamic_cast<P_Switch*>(e->obj);
				if (e->ny < 0)
				{
					pswitch->SetState(PSWITCH_STATE_USED);
					pswitch->SetPosition(pswitch->GetX(), pswitch->GetY() + PSWITCH_SMALLER);
				}
			}
			if (e->obj->GetType() == BOX)
			{
				Box* box = dynamic_cast<Box*>(e->obj);
				if (e->ny < 0)
				{
					vy = last_vy;
					y += dy;
				}
				if (e->nx != 0)
				{
					x += dx;
					
					if (box->ani == BOX_ANI_RANDOM)
					{
						LPANIMATION current_ani = animation_set->at(box->ani);
						switch (current_ani->GetCurrentFrame())
						{
						case 0:

							SetLevel(MARIO_LEVEL_FIRE);
							break;
						}
						box->SetFinish(true);
					}
				}

			}
			if (e->obj->GetType() == WOOD)
			{
				Wood* wood = dynamic_cast<Wood*>(e->obj);
				if (e->ny < 0)
				{
					wood->isCollisMario = true;
				}
			}
			else if (e->obj->GetType()==PORTAL)
			{
				if (e->ny != 0)
				{
					if (isSitting)
					{
						CPortal* p = dynamic_cast<CPortal*>(e->obj);
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
						isSwitchScene = true;
					}
				}
			}
		}
	}
#pragma region Update Fire and Efecct
	for (int i = 0; i < ListFire.size(); i++)
	{
		ListFire[i]->Update(dt, coObjects);
			if (ListFire[i]->GetFinish() == true)
			{
				float cX, cY;
				ListFire[i]->GetPosition(cX, cY);
				EffectDisappear* effect = new EffectDisappear(cX, cY);
				ListEffect.push_back(effect);
			}
	}
	for (int i = 0; i < ListFire.size(); i++)
	{
		if (ListFire[i]->GetFinish()==true)
		{
			ListFire.erase(ListFire.begin() + i);
		}
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
			ListEffect[i]->Update(dt);
	for (int i = 0; i < ListEffect.size(); i++)
	{
		CGameObject* effect = ListEffect[i];
		if (effect->GetFinish())
		{
			if (effect->GetType() == COIN_EFFECT)
			{
				ListEffect.erase(ListEffect.begin() + i);
				ListEffect.push_back(new PointEffect(effect->GetX(), effect->GetY(), POINT_EFFECT_TYPE_ONE_HUNDRED));
				SetScore(GetScore() + 100);
				SetCoinCollect(GetCoinCollect() + 1);
			}
			else
				ListEffect.erase(ListEffect.begin() + i);
		}
	}
	tail->Update(dt, coObjects);
#pragma endregion
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else

		//MARIO SMALL
		if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0)
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_IDLE_LEFT;

			if (state == MARIO_STATE_WALKING_RIGHT)
			{
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				if (isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_WALK_RIGHT;
				}
			}
			
			if (state == MARIO_STATE_WALKING_LEFT)
			{
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}

			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_TURN_RIGHT;

				if (nx < 0) {
					ani = MARIO_ANI_SMALL_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUN_RIGHT)
			{
				ani = MARIO_ANI_SMALL_RUN_RIGHT;

			}
			if (state == MARIO_STATE_RUN_LEFT)
			{
				ani = MARIO_ANI_SMALL_RUN_LEFT;
			}
			/*if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_RUNMAXSPEED_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_RUNMAXSPEED_LEFT;
			}*/

			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			if (state == MARIO_STATE_WORLDMAP)
				ani = MARIO_SMALL_INTRO;
		/*	if (state = MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_FALL_LEFT;
			}*/
		/*	if (isFlying)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_FLY_LEFT;
			}*/
		}

	//       MARIO BIG
		else if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_IDLE_LEFT;
			if (isHoldTurtle)
			{
				if (nx > 0)
					ani = MARIO_BIG_ANI_HOLD_IDLE_RIGHT;
				if (nx < 0)
					ani = MARIO_BIG_ANI_HOLD_IDLE_LEFT;
			}
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0)
					ani = MARIO_ANI_SIT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SIT_LEFT;
			}
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_TURN_RIGHT;

				if (nx < 0) {
					ani = MARIO_ANI_BIG_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUN_RIGHT)
			{
				ani = MARIO_ANI_BIG_RUN_RIGHT;
				if (isHoldTurtle)
				{
					ani = MARIO_BIG_ANI_HOLD_WALK_RIGHT;
				}
			}
			if (state == MARIO_STATE_RUN_LEFT)
			{
					ani = MARIO_ANI_BIG_RUN_LEFT;
					if (isHoldTurtle)
					{
						ani = MARIO_BIG_ANI_HOLD_WALK_LEFT;
					}
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_BIG_RUNMAXSPEED_RIGHT;
					if (isHoldTurtle)
						ani = MARIO_BIG_ANI_HOLD_RUN_MAX_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIO_ANI_BIG_RUNMAXSPEED_LEFT;
					if (isHoldTurtle)
						ani = MARIO_BIG_ANI_HOLD_RUN_MAX_LEFT;
				}
			}
			if (state==MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_JUMP_LEFT;
				if (nx > 0 && isHoldTurtle)
				{
					ani = MARIO_BIG_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_BIG_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_FALL_LEFT;
				if (nx > 0 && isHoldTurtle)
				{
					ani = MARIO_BIG_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_BIG_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FLY_UP)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_FLY_LEFT;
			}
			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_KICK_LEFT;
			}
			if (state == MARIO_STATE_WORLDMAP)
				ani = MARIO_BIG_INTRO;
		}

	//MARIO RACCOON
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0)
				ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_IDLE_LEFT;
			if (isHoldTurtle)
			{
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_HOLD_IDLE_RIGHT;
				if(nx<0)
					ani = MARIO_RACCOON_ANI_HOLD_IDLE_LEFT;
			}
			if (state == MARIO_STATE_WALKING_RIGHT)
			{
				ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			}
		
			if (state == MARIO_STATE_WALKING_LEFT)
			{
				ani = MARIO_ANI_RACCOON_WALKING_LEFT;
			}
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_TURN_RIGHT;

				if (nx < 0) {
					ani = MARIO_ANI_RACCOON_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUN_RIGHT)
			{
				ani = MARIO_ANI_RACCOON_RUN_RIGHT;
				if (isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_WALK_RIGHT;
				}
			}
			if (state == MARIO_STATE_RUN_LEFT)
			{
				ani = MARIO_ANI_RACCOON_RUN_LEFT;
				if (isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_WALK_LEFT;
				}
			}
			if (state == MARIO_STATE_FALL_FLY)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FALL_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FALL_FLY_LEFT;
			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_SIT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_SIT_LEFT;
			}
			if (isAttack)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_ATTACK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_ATTACK_LEFT;
			}
			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_KICK_LEFT;
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_RACCOON_RUNMAXSPEED_RIGHT;
					if (isHoldTurtle)
						ani = MARIO_RACCOON_ANI_HOLD_RUN_MAX_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIO_ANI_RACCOON_RUNMAXSPEED_LEFT;
					if (isHoldTurtle)
						ani = MARIO_RACCOON_ANI_HOLD_RUN_MAX_LEFT;
				}
			}
			if (state==MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_JUMP_LEFT;
				if (nx>0 && isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FALL_LEFT;
				if (nx > 0 && isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_RACCOON_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state==MARIO_STATE_FLY_UP)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FLY_LEFT;
			}
			if (isFallSlow)
			{
				if (nx > 0)
					ani = MARIO_ANI_FALL_SLOW_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FALL_SLOW_LEFT;
			}
			if (state == MARIO_STATE_WORLDMAP)
				ani = MARIO_RACCOON_INTRO;

		}

	//MARIO FIRE
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_IDLE_RIGHT;
			else
				ani = MARIO_ANI_FIRE_IDLE_LEFT;
			if (isHoldTurtle)
			{
				if (nx > 0)
					ani = MARIO_FIRE_ANI_HOLD_IDLE_RIGHT;
				if (nx < 0)
					ani = MARIO_FIRE_ANI_HOLD_IDLE_LEFT;
			}
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
			

			if (state == MARIO_STATE_TURN)
			{
				if (nx < 0)
					ani = MARIO_ANI_FIRE_TURN_LEFT;

				if (nx > 0) {
					ani = MARIO_ANI_FIRE_TURN_RIGHT;
				}
			}
			if (state == MARIO_STATE_RUN_RIGHT)
			{
				ani = MARIO_ANI_FIRE_RUN_RIGHT;
				if (isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_WALK_RIGHT;
				}
			}
			if (state == MARIO_STATE_RUN_LEFT)
			{
				ani = MARIO_ANI_FIRE_RUN_LEFT;
				if (isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_WALK_LEFT;
				}
			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_SIT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_SIT_LEFT;
			}

			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_KICK_LEFT;
			}
			if (isAttack)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_SHOOT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_SHOOT_LEFT;
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_FIRE_RUNMAXSPEED_RIGHT;
					if (isHoldTurtle)
						ani = MARIO_FIRE_ANI_HOLD_RUN_MAX_RIGHT;
				}
				else if (nx < 0)
				{
					ani = MARIO_ANI_FIRE_RUNMAXSPEED_LEFT;
					if (isHoldTurtle)
						ani = MARIO_FIRE_ANI_HOLD_RUN_MAX_LEFT;
				}
			}

			if (state==MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_JUMP_LEFT;
				if (nx > 0 && isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_FALL_LEFT;
				if (nx > 0 && isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_JUMP_RIGHT;
				}
				if (nx < 0 && isHoldTurtle)
				{
					ani = MARIO_FIRE_ANI_HOLD_JUMP_LEFT;
				}
			}
			if (state == MARIO_STATE_FLY_UP)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_FLY_LEFT;
			}
		}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);
	for (int i = 0; i < ListFire.size(); i++)
	{
		ListFire[i]->Render();
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
			ListEffect[i]->Render();
	}
	for (UINT i = 0; i < ListTail.size(); i++)
	{
		ListTail[i]->Render();
	}
	
	tail->Render();
	//RenderBoundingBox();
}
void CMario::SetLevel(int l)
{
	level = l;
}
void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_IDLE:
		Decelerate();
		break;
	case MARIO_STATE_DIE:
		vx = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (isSitting)
		{
			ResetSit();
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		isWalkingL = true;
		if (isSitting)
		{
			ResetSit();
		}
break;
	case MARIO_STATE_SIT:
		isSitting = true;
		break;
	case MARIO_STATE_RUN_LEFT:
	{
		vx -= MARIO_RUNNING_SPEED * dt;
		if (vx <= -MARIO_RUNNING_MAXSPEED)
		{
			this->state = MARIO_STATE_RUN_MAXSPEED;
			vx = -MARIO_RUNNING_MAXSPEED;

		}
		nx = -1;
		break;
	}
	case MARIO_STATE_RUN_RIGHT:
	{
		vx += MARIO_RUNNING_SPEED * dt;
		if (vx >= MARIO_RUNNING_MAXSPEED)
		{
			this->state = MARIO_STATE_RUN_MAXSPEED;
			vx = MARIO_RUNNING_MAXSPEED;
		}
		nx = 1;
		break;
	}
	case MARIO_STATE_RACCOON_ATTACK:
		isAttack = true;
		break;
	case MARIO_STATE_SHOOT_FIRE:
	{
		isAttack = true;
		break;
	}
	case MARIO_STATE_JUMP:
	{
		if (isOnGround)
		{
			vy = -MARIO_JUMP_SPEED_Y;
			isJumping = true;
			isOnGround = false;
			if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
			{
				isFlying = true;
				isOnGround = false;
				vy = -MARIO_JUMP_SPEED_RUNNING_MAXSPEED;
			}
		}
		else
		{
			if (level == MARIO_LEVEL_RACCOON)
			{
				isOnAir = true;
				if (isFlying)
				{
					isFalling = false;
					isFlyup = true;
				}
			}
		}
		break;
	}
	}

}
#pragma region Set chuyển động
void CMario::WalkRight()
{
	if (abs(vx) > MARIO_WALKING_MAXSPEED)
	{
		vx = MARIO_WALKING_MAXSPEED;
	}
	vx += MARIO_WALKING_SPEED * dt;

}
void CMario::WalkLeft()
{
	if (abs(vx) > MARIO_WALKING_MAXSPEED)
	{
		vx = -MARIO_WALKING_MAXSPEED;
	}
	vx -= MARIO_WALKING_SPEED * dt;
}
void CMario::Sit()
{
	SetState(MARIO_STATE_SIT);
	Decelerate();
}
void CMario::JumpSlow()
{
	vy = -MARIO_JUMP_SLOW_SPEED_Y;
	isJumping = true;
	isOnGround = false;
}
void CMario::Fly()
{
	vy = -MARIO_FLY_SPEED_Y * dt;
	if (!isOnGround)
	{
		TimeFly += dt;
	}

}
void CMario::FallSlow()
{
	vy = MARIO_GRAVITY * dt / 2;
	isFallSlow = true;
}
void CMario::Decelerate()
{
	if (vx > 0)
	{
		vx -= MARIO_DECELERATE_SPEED * dt;
		if (vx <= 0)
			vx = 0;
	}
	if (vx < 0)
	{
		vx += MARIO_DECELERATE_SPEED * dt;
		if (vx >= 0)
			vx = 0;
	}
}
void CMario::ShootFire()
{
	Fire* fire = new Fire(x + 5, y + 5);
	fire->SetDirection(this->nx);
	ListFire.push_back(fire);
}
void CMario::TailAttack()
{
	if (ani == MARIO_ANI_RACCOON_ATTACK_RIGHT || ani == MARIO_ANI_RACCOON_ATTACK_LEFT)
	{
		LPANIMATION current_ani = animation_set->at(ani);
		switch (current_ani->GetCurrentFrame())
		{
		case 2:
		case 3:
		case 4:
			if (nx < 0)
			{
				tail->SetPosition(x - 2, y + 18);
				tail->SetFinish(false);
				tail->SetDirection(nx);
			}
			if (nx > 0)
			{
			tail->SetPosition(x + 15, y + 18);
				tail->SetFinish(false);
				tail->SetDirection(nx);
			}
			break;
		}
		
	}
}
void CMario::ResetSit()
{
	if (isSitting)
	{
		isSitting = false;
		y = y - PULL_UP_MARIO_AFTER_SIT;
	}
}
#pragma endregion

int CMario::GetLevel()
{
	return level;
}
#pragma region Set mario tương tác với game 
bool CMario::CheckCollision(CGameObject* obj)
{
	if (isFinish == true)
		return false;
	return isCollisionObjectWithObject(obj);
}
bool CMario::isCollisionWithItem(Item* objItem)
{
	if (objItem->GetFinish() == true)
		return false;

	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);

	objItem->GetBoundingBox(l1, t1, r1, b1);
	if (CGame::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1) == true)
	{
		return true;
	}
	return isCollisionObjectWithObject(objItem);
}
void CMario::SetHurt(LPCOLLISIONEVENT e)
{
	if (e->nx == 0 && e->ny == 0)  //không có va chạm
		return;
	StartUntouchable();

	SubHealth(1);
}
void CMario::SetLive(int l)
{
	Live = l;
}
int CMario::GetLive()
{
	return Live;
}
void CMario::SetCoinCollect(int c)
{
	CoinCollect = c;
}
int CMario::GetCoinCollect()
{
	return CoinCollect;
}
void CMario::SetScore(int s)
{
	score = s;
}
int CMario::GetScore()
{
	return score;
}
bool CMario::GetIsDeadth()
{
	return isDeadth;
}
void CMario::SetIsDeadth(bool b)
{
	isDeadth = b;
}
#pragma endregion
void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if(level==MARIO_LEVEL_SMALL)
	{
	left = x;
	top = y;
	right = x + MARIO_SMALL_BBOX_WIDTH;
	bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if(level == MARIO_LEVEL_BIG || level ==MARIO_LEVEL_FIRE)
	{
		left = x;
		top = y;
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitting)
		{
			right = x + 10;
			bottom = y + MARIO_SIT_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (nx > 0)
		{
			left = x + 8;
			top = y;
			right = left + MARIO_BIG_BBOX_WIDTH ;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		if (nx < 0)
		{
			left = x ;
			top = y ;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		if (isSitting)
		{
			right = x + 10;
			bottom = y + MARIO_SIT_BBOX_HEIGHT;
		}
	}
}
void CMario::Init()
{
	score = MARIO_DEFAULT_SCORE;
	CoinCollect = MARIO_DEFAULT_COIN;
	Live = MARIO_DEFAULT_LIVE;
}
/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	SetHealth(1);
}

