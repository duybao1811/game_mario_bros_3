#include "Koopas.h"
#include "Brick.h"
CKoopas::CKoopas()
{
	type = Type::KOOPAS;
	SetState(KOOPAS_STATE_WALKING);
	SetHealth(3);
	direction = 1;
	
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_BALL || state == KOOPAS_STATE_DEFEND || state==KOOPAS_STATE_ATTACKED || isUpside )
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
	
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += KOOPAS_GRAVITY * dt;
	CGameObject::Update(dt, coObjects);
	if (Health <= 0)
	{
		IsDie = true;
	}
	if (IsDie)
		return;
	if (Health == 3)   //Koopas
	{
		SetState(KOOPAS_STATE_WALKING);
	}
	if (Health == 2)
	{
		SetState(KOOPAS_STATE_DEFEND);
	}
	if (Health == 1)
	{
		if (state != KOOPAS_STATE_BALL)
			SetState(KOOPAS_STATE_BALL);
	}
	//bị lật ngược thì nằm im
	if (isUpside)
	{
		vx = 0;
	}
	if (isAttacked)
	{
		state = KOOPAS_STATE_ATTACKED;
		vx = direction * 0.1f;
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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (state == KOOPAS_STATE_WALKING)
				{

					if ( nx != 0)
					{
						this->direction = -this->direction;
						vx = this->direction * KOOPAS_WALKING_SPEED;
					}
				}
				if (state == KOOPAS_STATE_BALL)
				{
						if (nx != 0)
						{
							this->direction = -this->direction;
							vx = this->direction * KOOPAS_BALL_SPEED;
						}					
				}
				if (state == KOOPAS_STATE_ATTACKED)
				{
					if (ny != 0)
					{
						vy = -KOOPAS_BOUNCE_AFTER_LANDFALL;
						isAttacked = false;
						isUpside = true;
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
	if (state == KOOPAS_STATE_BALL)
	{
		ani = KOOPAS_ANI_BALL;
	}
	if (state == KOOPAS_STATE_DEFEND) {
		ani = KOOPAS_ANI_DEFEND;
	}
	if (state == KOOPAS_STATE_WALKING)
	{
		if (direction > 0)
			ani = KOOPAS_ANI_WALKING_RIGHT;
		else if (direction < 0)
			ani = KOOPAS_ANI_WALKING_LEFT;
	}
	if (state == KOOPAS_STATE_ATTACKED)
	{
		ani = KOOPAS_ANI_ATTACKED;
	}
	if (isUpside)
	{
		ani = KOOPAS_ANI_ATTACKED;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
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
		isDefend = true;
		break;
	case KOOPAS_STATE_BALL:
		vx =  direction*KOOPAS_BALL_SPEED;
		break;
	case KOOPAS_STATE_ATTACKED:	
		isAttacked = true;
		vy = -KOOPAS_SPEED_Y_AFTER_ATTACKED;
		break;

	}

}