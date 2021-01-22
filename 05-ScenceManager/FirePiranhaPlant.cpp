#include "FirePiranhaPlant.h"
CFirePiranhaPlant::CFirePiranhaPlant(float X,float Y,int Model, vector<FirePlant*>* listFireEnemy)
{
	this->x = X;
	this->y = Y;
	this->model = Model;
	this->ListFireEnemy = listFireEnemy;
	TimeHidding = 0;
	TimeAttack = 0;
	TimeAttackDelay = 0;
	maxY = Y;
	this->startX = X;
	SetState(PLANT_STATE_GROW_UP);
	eType = Type::FIRE_PIRANHA;
	objType = ObjectType::ENEMY;
	switch (model) 	//Model 1: hoa bắn lửa màu đỏ Model 2: hoa bắn lửa màu xanh
	{
	case 1:                //Fire piranha red
		SetHealth(1);
		minY = Y - FPLANT_RED_BBOX_HEIGHT;
		break;
	case 2:
		SetHealth(1);
		minY=y-FPLANT_GREEN_BBOX_HEIGHT;
		break;
	}
}
void CFirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (isFinish)
		return;
	y += dy;
	x += dx;
	if (y <= minY)
	{
		y = minY;
		vy = 0;
		SetState(PLANT_STATE_ATTACK);
		TimeAttack += dt;
		TimeAttackDelay += dt;
	}
	else if (y >= maxY)
	{
		y = maxY;
		vy = 0;
		TimeHidding += dt;
	}
	if (GetSafeZone()==false && TimeHidding > TIME_DELAY_GROW_UP)  // nếu time trống trong ống lớn hơn time delay và không phải trong vùng an toàn thì grow up
	{
		SetState(PLANT_STATE_GROW_UP);
		TimeHidding = 0;
	}
	if (TimeAttackDelay > TIME_ATTACK_DELAY && Health > 0) 
	{
		if (ListFireEnemy->size() < 1)
		{
			CreateFire();
		}
		TimeAttackDelay = 0;
	}
	if (TimeAttack > TIME_ATTACK) // sau khi bắn đạn delay 1 khoảng thì chui vào cống
	{
		SetState(PLANT_STATE_HIDDING);
		TimeAttack = 0;
	}
	if (Health <= 0)
	{
		isFinish = true;
	}
}
void CFirePiranhaPlant::Render()
{
	if (isFinish)
		return;
	marioRange = GetMarioRangeCurrent();
	int ani = FPLANT_ANI_LEFT_TOP;
	if (state == PLANT_STATE_ATTACK)
	{
		if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
		{
			ani = FPLANT_ANI_ATTACK_LEFT_TOP;
		}
		if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
		{
			ani = FPLANT_ANI_ATTACK_LEFT_BOTTOM;
		}
	    if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
		{
			ani = FPLANT_ANI_ATTACK_RIGHT_TOP;
		}
		if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
		{
			ani = FPLANT_ANI_ATTACK_RIGHT_BOTTOM;
		}
	}
	else
	{
		if (marioRange == LEFT_TOP_SIDE_NEAR || marioRange == LEFT_TOP_SIDE_FAR)
		{
			ani = FPLANT_ANI_LEFT_TOP;
		}
		if (marioRange == LEFT_BOTTOM_SIDE_NEAR || marioRange == LEFT_BOTTOM_SIDE_FAR)
		{
			ani = FPLANT_ANI_LEFT_BOTTOM;
		}
		if (marioRange == RIGHT_TOP_SIDE_NEAR || marioRange == RIGHT_TOP_SIDE_FAR)
		{
			ani = FPLANT_ANI_RIGHT_TOP;
		}
		if (marioRange == RIGHT_BOTTOM_SIDE_NEAR || marioRange == RIGHT_BOTTOM_SIDE_FAR)
		{
			ani = FPLANT_ANI_RIGHT_BOTTOM;
		}
	}

	animation_set->at(ani)->Render(x, y);
}
void CFirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PLANT_STATE_GROW_UP:
	{
		vy = -PLANT_SPEED_GROW_UP*dt ;
		break;
	}
	case PLANT_STATE_HIDDING:
	{
		vy = PLANT_SPEED_HIDDING*dt;
		break;
	}
	case ENEMY_STATE_FIRE_ATTACK:
	{
		isFinish = true;
		break;
	}
	case ENEMY_ATTACKED:
	{
		isFinish = true;
		break;
	}
	}
}
void CFirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FPLANT_BBOX_WIDTH;
	bottom = y + FPLANT_RED_BBOX_HEIGHT;
	if (isFinish)
		left = top = right = bottom = 0;
}
Range CFirePiranhaPlant::GetMarioRangeCurrent()
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->x < this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_TOP_SIDE_FAR;
		}
	}
	if (mario->x < this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_NEAR;
		}
		if (this->x - mario->x > DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return LEFT_BOTTOM_SIDE_FAR;
		}

	}
	if (mario->x > this->x && mario->y < this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_TOP_SIDE_FAR;
		}
	}
	if (mario->x > this->x && mario->y > this->y)
	{
		if (this->x - mario->x < DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_NEAR;
		}
		if (this->x - mario->x >= DISTANCE_FROM_MARIO_TO_PLANT)
		{
			return RIGHT_BOTTOM_SIDE_FAR;
		}
	}

}
bool CFirePiranhaPlant::GetSafeZone()
{
	LPSCENE scence = CGame::GetInstance()->GetCurrentScene();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->x < this->x)
	{
		if (this->x - mario->x <= DISTANCE_SAFE_ZONE)   // nếu khoảng cách từ mario đến plant trong vùng an toàn thì plant không grow up
		{
			return true;
		}
	}
	if (mario->x > this->x)
	{
		if (mario->x - this->x <= DISTANCE_SAFE_ZONE)
		{
			return true;
		}
	}
	return false;
}
void CFirePiranhaPlant::CreateFire()
{
	marioRange = GetMarioRangeCurrent();
	FirePlant* fireplant = new FirePlant(this->x + 5, this->y + 5, marioRange);
	ListFireEnemy->push_back(fireplant);
}