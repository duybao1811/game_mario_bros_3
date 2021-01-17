#include "Board.h"
#include "Font.h"
#include "Power.h"
#include <string>
Board::Board(float X,float Y)
{
	this->x = X;
	this->y = Y;
	Tex = CTextures::GetInstance()->Get(TexID_Board);
	Sprite = new CSprite(0, 0, 0, 314, 64, Tex);
}
string Board::FillNumber(string s, UINT amoumt_of_number)
{
	while (s.size() < amoumt_of_number)
		s = "0" + s;
	return s;
}
void Board::Render(CMario * mario, int RemainingTime )
{
	now = GetTickCount64();
	Sprite->Draw(x, y);
	font.Draw(x + 70, y + 20, FillNumber(std::to_string(mario->GetScore()), 7)); //score
	if (CGame::GetInstance()->GetScene() != 1)
	{
		font.Draw(x + 141, y + 20, FillNumber(std::to_string(RemainingTime), 3)); //remain time
	}
	else
	{
		font.Draw(x + 141, y + 20, FillNumber(std::to_string(000), 3)); //remain time
	}
	font.Draw(x + 150, y + 12, FillNumber(std::to_string(mario->GetCoinCollect()), 2));  //coin
	font.Draw(x + 47, y + 20, FillNumber(std::to_string(mario->GetLive()), 1));// mạng
	if (mario->isRunning || mario->isFlying)
	{
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 7)
		{
			power.Draw(x + 70, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 6)
		{
			power.Draw(x + 78, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 5)
		{
			power.Draw(x + 86, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 4)
		{
			power.Draw(x + 94, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 3)
		{
			power.Draw(x + 102, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 2)
		{
			power.Draw(x + 110, y + 12);
		}
		if (abs(mario->vx) >= MARIO_RUNNING_MAXSPEED / 1)
		{
			fpower.Draw(x + 118, y + 12);
		}
	}
}