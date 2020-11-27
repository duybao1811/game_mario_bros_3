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
	case BRICK_MODEL_COIN:
		SetHealth(1);
		break;
	case BRICK_MODEL_POWER_UP:
		SetHealth(1);
		break;
	}
}
void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (Health <= 0 )
	{
		state = QB_STATE_EMPTY;
	}
	if (y <= QB_MINY)
	{
		vy = QB_SPEED_DOWN;
		y = starty;
		SetState(QB_STATE_EMPTY);
	}
}
void CQuestionBrick::Render()
{
	int ani = QB_ANI;
	if (state == QB_STATE_UNBOX)
	{
		ani = QB_ANI_EMPTY;
	}
	if (state == QB_STATE_EMPTY)
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
void CQuestionBrick::SetState(int state)
{
	switch (state)
	{
	case QB_STATE_UNBOX:
		vy = -QB_SPEED_UP;
		break;
	case QB_STATE_EMPTY:
		break;
	}
}