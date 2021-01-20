#include "QuestionBrick.h"
CQuestionBrick::CQuestionBrick(float X, float Y,int Model)
{
	this->model = Model;
	this -> x = X;
	this->y = Y;
	starty = Y;
	minY = Y - QB_MINY;
	eType = Type::QUESTION_BRICK;
	switch (model)
	{
	case QUESTION_BRICK_MODEL_COIN:
		SetHealth(1);
		break;
	case QUESTION_BRICK_MODEL_POWER_UP:
		SetHealth(1);
		break;
	}
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (Health == 0 )
	{
		if (y <= minY)
		{
			vy = QB_SPEED_UP;
			isUnbox = true;
		}
		if (y >= starty)
		{
			y = starty;
			vy = 0;
		}
	}
}
void CQuestionBrick::Render()
{
	int ani = QB_ANI;
	if (Health == 0)
	{
		ani = QB_ANI_EMPTY;
	}
	animation_set->at(ani)->Render(x, y);
}
void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + QB_BBOX_WIDTH;
	b = y + QB_BBOX_HEIGHT;
}
int CQuestionBrick::GetModel()
{
	return model;
}