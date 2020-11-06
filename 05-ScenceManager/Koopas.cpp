#include "Koopas.h"
#include "Brick.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	SetHealth(3);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_BALL || state==KOOPAS_STATE_DEFEND)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
	if (Health == 3)   //Koopas
	{

	}
	if (Health == 2)
	{
		isWalking = false;
		SetState(KOOPAS_STATE_DEFEND);
	}
	if (Health == 1)
	{
		isBall = true;
		isDefend = false;
	}
	if (isWalking)
	{
		state == KOOPAS_STATE_WALKING;
	}
	if (isDefend)
	{
		state = KOOPAS_STATE_DEFEND;
		vx = 0;
	}
	if (isBall)
	{
		state = KOOPAS_STATE_BALL;
		vx = nx * KOOPAS_BALL_SPEED;
	}
	CGameObject::Update(dt, coObjects);
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
		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (state == KOOPAS_STATE_WALKING)
			{

				if (dynamic_cast<CBrick*>(e->obj)) {
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * KOOPAS_WALKING_SPEED;
					}
				}
				else {
					x += dx;
				}
			}
			if (state == KOOPAS_STATE_BALL)
			{
				if (dynamic_cast<CBrick*>(e->obj)) {
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * KOOPAS_BALL_SPEED;
					}
					else {
						x += dx;
					}
				}
			}

		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DEFEND || state==KOOPAS_STATE_BALL) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		isWalking = true;
		break;
	case KOOPAS_STATE_DEFEND:
		isWalking = false;
		isDefend = true;
		break;
	case KOOPAS_STATE_BALL:
		isBall = true;
		break;
	}

}