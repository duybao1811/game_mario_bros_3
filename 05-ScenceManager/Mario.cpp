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
#include "Brick.h"
#include "Koopas.h"
#include "Fire.h"
#include "BlockColor.h"
#include "Coin.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	TimeFly = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	vy += MARIO_GRAVITY * dt;
	if (isJumping || isFlying)
	{
		if (vy > 0)
		{
			//MarioGravity += 0.0001f;
			vy += MarioGravity * dt;
		}
	}
	else
		vy += MARIO_GRAVITY * dt;
	if (isWalking) {
		if (vx == 0)
		{
			vx = nx * MARIO_WALKING_SPEED;
			if (nx > 0)
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
					if (!isRunning && abs(vx) >= MARIO_WALKING_MAXSPEED)
					{
						vx = MARIO_WALKING_MAXSPEED;
					}
					else if (isRunning) {
						if (vx >= MARIO_RUNNING_MAXSPEED)
						{
							vx = MARIO_RUNNING_MAXSPEED;
							state = MARIO_STATE_RUN_MAXSPEED;
							isReadyToFly = true;
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
					vx -= a;
					state = MARIO_STATE_WALKING_LEFT;
					if (!isRunning && abs(vx) >= MARIO_WALKING_MAXSPEED)
					{
						vx = -MARIO_WALKING_MAXSPEED;
					}
					else if (isRunning)
					{
						if (abs(vx) >= MARIO_RUNNING_MAXSPEED)
						{
							vx = -MARIO_RUNNING_MAXSPEED;
							state = MARIO_STATE_RUN_MAXSPEED;
							isReadyToFly = true;
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
	if (isFalling && isSitting)
	{
		state = MARIO_STATE_SIT;
	}
	if (isKick)
	{
		state = MARIO_STATE_KICK;
	}
	if (isFlying)
	{
		state = MARIO_STATE_FLY_UP;
	}
	if (isFallSlow)
	{
		state = MARIO_RACCOON_STATE_FALL_SLOW;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	Time = GetTickCount64() - StartFly;
	if (level == MARIO_LEVEL_RACCOON && GetTickCount64() - Attack > 450 && isAttack)
	{
		isAttack = false;
		state = MARIO_STATE_IDLE;
	}
	if (level == MARIO_LEVEL_FIRE && GetTickCount64() - Attack > 240 && isAttack)
	{
		isAttack = false;
		state = MARIO_STATE_IDLE;
	}
	if (GetTickCount64() - Kick > 230 && isKick)
	{
		isKick = false;
		state = MARIO_STATE_IDLE;
	}
	if (isAttack && level == MARIO_LEVEL_FIRE)
	{
		state = MARIO_STATE_SHOOT_FIRE;
	}
	else if (isAttack && level == MARIO_LEVEL_RACCOON)
	{
		state = MARIO_STATE_RACCOON_ATTACK;
	}

	if (isFalling && level == MARIO_LEVEL_RACCOON)
	{
		if (isFlying)
		{
			state = MARIO_STATE_FALL_FLY;
		}
	}
	if (isOnAir)
	{
		if (TimeFly < 130)
		{
			TimeFly++;
		}
		else
		{
			isFlying = false;
		}
	}
	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetHealth() == 1)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						goomba->SubHealth(1);
					}
				}
				else if (e->nx != 0)
				{
					if (level == MARIO_LEVEL_RACCOON && isAttack)
					{
						goomba->SetDirection(-nx);
						goomba->SetState(GOOMBA_STATE_ATTACKED);

					}
					if (untouchable == 0 && !isAttack)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
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
					isFlying = false;
					isFallFly = false;
					isFallSlow = false;
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
				else {
					isOnGround = false;
				}
				if(brick->BType!=0 || brick->BType!=1)
				{
					if (e->ny > 0)
					{
						if (brick->GetState() != QB_STATE_EMPTY)
						{
							brick->SetState(QB_STATE_UNBOX);
							brick->SubHealth(1);
						}
					}
				}
				if (brick->BType == 1)
				{
					if (e->nx != 0)
					{
						x += dx;
					}
				}

			}
			if (e->obj->GetType() == KOOPAS)
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
						koopas->direction = 1;
						koopas->SubHealth(1);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
					if (koopas->GetState() == KOOPAS_STATE_BALL)
					{
						koopas->SetHealth(2);
						vy = -MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION;
					}
				}
				if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_DEFEND)
					{
						koopas->SetDirection(this->nx);
						koopas->SubHealth(1);
						isKick = true;
						Kick = GetTickCount64();

					}
				}
				if (level == MARIO_LEVEL_RACCOON && isAttack)
				{
					if (nx != 0) {
						koopas->SetDirection(-nx);
						koopas->SetState(KOOPAS_STATE_ATTACKED);
					}
				}
			}
			if (e->obj->GetType() == COIN)
			{
				if (e->nx != 0 || e->ny != 0)
				{
					CCoin* coin = dynamic_cast<CCoin*>(e->obj);
					coin->isFinish = true;
				}

			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}

		}
	}
	for (int i = 0; i < ListFire.size(); i++)
	{
		ListFire[i]->Update(dt, coObjects);
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

		//MARIO SMALL
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

				if (nx < 0) {
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

	//       MARIO BIG
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
			if (state == MARIO_STATE_FLY_UP)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_BIG_FLY_LEFT;
			}
		}

	//MARIO RACCOON
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
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FALL_LEFT;
			}
			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_KICK_LEFT;
			}
			if (state == MARIO_RACCOON_STATE_FALL_SLOW)
			{
				if (nx > 0)
					ani = MARIO_ANI_FALL_SLOW_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FALL_SLOW_LEFT;
			}
			if (state == MARIO_STATE_RUNNING)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_RUN_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_RUN_LEFT;
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_RUNMAXSPEED_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_RUNMAXSPEED_LEFT;
			}
			if (state == MARIO_STATE_FLY_UP)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FLY_LEFT;
			}
			if (state == MARIO_STATE_FALL_FLY)
			{
				if (nx > 0)
					ani = MARIO_ANI_RACCOON_FALL_FLY_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_RACCOON_FALL_FLY_LEFT;
			}

		}

	//MARIO FIRE
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
			if (state == MARIO_STATE_FALLING)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_FALL_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_FALL_LEFT;
			}
			if (state == MARIO_STATE_KICK)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_KICK_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_KICK_LEFT;
			}
			if (state == MARIO_STATE_SHOOT_FIRE)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_SHOOT_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_SHOOT_LEFT;
			}
			if (state == MARIO_STATE_RUN_MAXSPEED)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_RUNMAXSPEED_RIGHT;
				else if (nx < 0)
					ani = MARIO_ANI_FIRE_RUNMAXSPEED_LEFT;
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
		isWalking = true;
		isWalkingR = true;
		if (isSitting)
		{
			ResetSit();
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		isWalking = true;
		isWalkingL = true;
		if (isSitting)
		{
			ResetSit();
		}
		break;
	case MARIO_STATE_SIT:
		if (!isWalkingR && !isWalkingL && !isRunning || vx == 0)
		{
			isSitting = true;
		}
		else
		{
			isSitting = false;
		}

		break;
	case MARIO_STATE_RUNNING:
		isRunning = true;
		vx += nx * 0.00045f;
		break;
	case MARIO_STATE_RACCOON_ATTACK:
		isAttack = true;
		break;
	case MARIO_STATE_JUMP:
		if (isOnGround) {
			vy = -MARIO_JUMP_SPEED_Y;
			isJumping = true;
			isOnGround = false;
			if (abs(vx) > MARIO_RUNNING_MAXSPEED)
			{
				isFlying = true;
				isOnGround = false;
				vy = -MARIO_JUMP_SPEED_RUNING_MAXSPEED;
			}
		}
		else {
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
	case MARIO_STATE_SHOOT_FIRE:
	{
		Fire* fire = NULL;
		fire = new Fire();
		fire->SetPosition(x + 5, y + 5);
		fire->SetDirection(this->nx);
		ListFire.push_back(fire);
		isAttack = true;
		break;
	}
	case MARIO_STATE_FLY:
	{

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
void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


	if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE || level == MARIO_LEVEL_RACCOON)
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
	else
	{
		left = x;
		top = y;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_RACCOON && nx > 0) //Raccoon khi quay qua phai sai bounding box
	{
		right = x + MARIO_BIG_BBOX_WIDTH + 5;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		//	left = right - MARIO_BIG_BBOX_WIDTH;
		if (isAttack) {
			right = x + MARIO_BIG_BBOX_WIDTH + 15;
		}
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

