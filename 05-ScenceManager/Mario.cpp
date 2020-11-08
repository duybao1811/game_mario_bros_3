﻿#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "Koopas.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	//set mario roi tu do va mario roi khi bay va nhay


	vy += MARIO_GRAVITY * dt;


	if (isJumping || isFlying)
	{
		if (vy > 0) //dang xuong
		{
			MarioGravity += 0.0001f;
			vy += MarioGravity * dt;
		}
	}
	else //khong nhay khong bay roi binh thuong
		vy += MARIO_GRAVITY * dt;
	if (isWalking) {
		if (vx == 0)
		{
			vx = nx * MARIO_WALKING_SPEED;
			if (vx > 0)
				state = MARIO_STATE_WALKING_RIGHT;
			else
				state = MARIO_STATE_WALKING_LEFT;
		}
		else
			if (vx > 0)
			{
				if (isWalkingR) 
				{
					vx += a;
					state = MARIO_STATE_WALKING_RIGHT;
						if (!isRunning && vx >= MARIO_WALKING_MAXSPEED)
						{
							vx = MARIO_WALKING_MAXSPEED;
						}
						else if (isRunning) {
							if (vx >= MARIO_RUNNING_MAXSPEED)
							{
								vx = MARIO_RUNNING_MAXSPEED;
								state = MARIO_STATE_RUN_MAXSPEED;
							}
							else
								state = MARIO_STATE_RUNNING;
						}

				}
				else {
					if (isWalkingL) {
						vx -= turn;
						if (vx >= 0)
						{
							state = MARIO_STATE_TURN;
						}
						else {
							vx = 0;
						}
					}
					else if (isOnGround)
					{
						vx = -a;
						if (vx <= 0)
						{
							vx = 0;
							isWalking = false;
							if (!isSitting)
							{
								state = MARIO_STATE_IDLE;
							}
						}
					}
				}

			}
			else 
			{
					if (isWalkingL)
					{
						vx -=a;
						state = MARIO_STATE_WALKING_LEFT;
						if (!isRunning && vx <= -MARIO_WALKING_MAXSPEED)
						{
							vx = -MARIO_WALKING_MAXSPEED;

						}
						else if (isRunning)
						{
							if (vx <= -MARIO_RUNNING_MAXSPEED)
							{
								vx = -MARIO_RUNNING_MAXSPEED;
								state = MARIO_STATE_RUN_MAXSPEED;
							}
							else
								state = MARIO_STATE_RUNNING;
						}
					}
					else {
						if (isWalkingR) {
							vx += turn;
							if (vx <= 0)
							{
								state = MARIO_STATE_TURN;
							}
							else {
								vx = 0;
							}
						}
						else if (isOnGround)
						{
							vx += a;
							if (vx >= 0)
							{
								vx = 0;
								isWalking = false;
								if (!isSitting)
								{
									state = MARIO_STATE_IDLE;
								}
							}
						}
					}
			
			}
	}
	if (isJumping)
	{
		state = MARIO_STATE_JUMP;
		if (vy >= 0)
		{
			if (!isFalling) {
				MarioGravity = 0;
				isFalling = true;
			}
			state = MARIO_STATE_FALLING;
		}
	}
	if (isSitting)
	{
		state = MARIO_STATE_SIT;
	}
	if (isFalling && level != MARIO_LEVEL_RACCOON)
	{
		state = MARIO_STATE_FALLING;
	}
	if (isFalling)
	{
		if (isSitting)
			state = MARIO_STATE_SIT;
	}
	if (isKick)
	{
		state = MARIO_STATE_KICK;

	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	DWORD Now = GetTickCount64();
	Time = Now - StartFly;
	if (level == MARIO_LEVEL_RACCOON && Now - Attack > 450 && isAttack)
	{
		isAttack = false;
		state = MARIO_STATE_IDLE;
	}
	if (level == MARIO_LEVEL_FIRE && Now - Attack > 240 && isAttack)
	{
		isAttack = false;
		state = MARIO_STATE_IDLE;
	}
	if (Now - Kick > 300 && isKick)
	{
		isKick = false;
		state = MARIO_STATE_IDLE;
	}
	if (isAttack && level == MARIO_LEVEL_FIRE)
	{
		state = MARIO_STATE_FIRE;
	}
	else if (isAttack && level == MARIO_LEVEL_RACCOON)
	{
		state = MARIO_STATE_RACCOON_ATTACK;
	}
	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				if (e->ny < 0)
				{
					isOnGround = true;
					isJumping = false;
					if (isFalling)
					{
						isFalling = false;
						if (vx != 0)
						{
							if (vx < 0)
								state = MARIO_STATE_WALKING_LEFT;
							if (vx > 0)
								state = MARIO_STATE_WALKING_RIGHT;
						}
						else 
							if (!isSitting)
							{
								state = MARIO_STATE_IDLE;
							}
					}
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj)) // 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						koopas->SubHealth(1);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->nx = 1;
						koopas->SubHealth(1);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
				}
				if (e->nx > 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->nx = -1;
						koopas->SubHealth(1);
						isKick = true;
						Kick = GetTickCount();

					}
				}
				else if (e->nx < 0 )
				{
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->nx = 1;
						koopas->SubHealth(1);
						isKick = true;
						Kick = GetTickCount();
					}
				}

			} // if Koopas
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
		if (level == MARIO_LEVEL_SMALL)
		{
			if (nx > 0)
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
			
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_TURN_RIGHT;
				
				 if(nx<0){
					ani = MARIO_ANI_SMALL_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_RUN_LEFT;
			}
			/*if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_RUNMAXSPEED_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_RUNMAXSPEED_LEFT;
			}*/
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else
				ani = MARIO_ANI_BIG_IDLE_LEFT;

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
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_TURN_RIGHT;

				if (nx < 0) {
					ani = MARIO_ANI_BIG_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_RUN_LEFT;
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_RUNMAXSPEED_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_RUNMAXSPEED_LEFT;
			}
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_FALL_LEFT;
			}
			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_KICK_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_RACCOON)
		{
			if (nx > 0)
				ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
			else
				ani = MARIO_ANI_RACCOON_IDLE_LEFT;
			if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_RACCOON_WALKING_LEFT;
			if (state == MARIO_STATE_JUMP)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_JUMP_LEFT;
			}
			if (state == MARIO_STATE_TURN)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_TURN_RIGHT;

				if (nx < 0) {
					ani = MARIO_ANI_RACCOON_TURN_LEFT;
				}
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_RUN_LEFT;

			}
			if (state == MARIO_STATE_SIT)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_SIT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_SIT_LEFT;
			}
			if (state == MARIO_STATE_RACCOON_ATTACK)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_ATTACK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_ATTACK_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
		if (nx > 0)
			ani = MARIO_ANI_FIRE_IDLE_RIGHT;
		else
			ani = MARIO_ANI_FIRE_IDLE_LEFT;
		if (state == MARIO_STATE_WALKING_RIGHT)
			ani = MARIO_ANI_FIRE_WALKING_RIGHT;
		if (state == MARIO_STATE_WALKING_LEFT)
			ani = MARIO_ANI_FIRE_WALKING_LEFT;
		if (state == MARIO_STATE_JUMP)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_JUMP_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_FIRE_JUMP_LEFT;
		}
		if (state == MARIO_STATE_TURN)
		{
			if (nx < 0)
				ani = MARIO_ANI_FIRE_TURN_LEFT;

			if (nx > 0) {
				ani = MARIO_ANI_FIRE_TURN_RIGHT;
			}
		}
		if (state == MARIO_STATE_RUNNING)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_RUN_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_FIRE_RUN_LEFT;
		}
		if (state == MARIO_STATE_SIT)
		{
			if (nx > 0)
				ani = MARIO_ANI_FIRE_SIT_RIGHT;
			else if (nx < 0)
				ani = MARIO_ANI_FIRE_SIT_LEFT;
		}
 }

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		isWalkingL = false;
		isWalking = true;
		isWalkingR = true;
		if (isSitting)
		{
			ResetSit();
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		isWalkingR = false;
		isWalking = true;
		isWalkingL = true;
		if (isSitting)
		{
			ResetSit();
		}
		break;
	case MARIO_STATE_SIT:
		if (!isWalkingR&&!isWalkingL && !isRunning)
		{
			isSitting = true;
		}
		else
		{
			isSitting = false;
		}

		break;
	case MARIO_STATE_RUN_MAXSPEED:
		isRunning = true;
		vx = MARIO_RUNNING_MAXSPEED;
		break;
	case MARIO_STATE_RUNNING:
		vx += nx * 0.00045f;
		break;
	case MARIO_STATE_RACCOON_ATTACK:
		isAttack = true;
		break;
	case MARIO_STATE_UP:
		if (isOnGround)
		{
			if (nx<0 && vx > -MARIO_RUNNING_MAXSPEED || nx > 0 && vx < MARIO_RUNNING_MAXSPEED) //nhay binh thuong
			{
				isJumping = true;
				isOnGround = false;
				vy = -MARIO_JUMP_SPEED_Y;
			}
			else //nhay khi chay nhanh
			{
				isFlying = true;
				isFlyup = true;
				isOnGround = false;
				vy = -MARIO_JUMP_SPEED_RUUNING_MAXSPEED;

			}
		}
		if (level == MARIO_LEVEL_RACCOON)
		{
			if (isJumping)
			{
				if (vy > 0)
				{
					if (!isFallSlow)
					{
						MarioGravity = -0.0006f;
						isFallSlow = true;
					}
				}
			}
			if (isFlying)
			{
				if (Time <= 3000)
				{
					isFalling = true;
					isFlying = false;
					MarioGravity = -0.00095f;   //hạ cánh
					vx -= nx * 0.002f; // V giảm
					isFlyup = true;
				}
				else
				{
					MarioGravity = -0.0007f;
					isFlyup = false;
				}
			}
		}
	
	break;	
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
void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG||level==MARIO_LEVEL_RACCOON||level==MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (isSitting)
		{
			right = x + 10;
			bottom = y + MARIO_SIT_BBOX_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
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
}

