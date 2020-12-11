#include "Board.h"
#include "Font.h"
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
	Sprite->Draw(x, y);
	font.Draw(x + 70, y + 20, FillNumber(std::to_string(mario->GetScore()), 7)); //score
	font.Draw(x + 141, y + 20, FillNumber(std::to_string(RemainingTime), 3)); //remain time
	font.Draw(x + 150, y + 12, FillNumber(std::to_string(mario->GetCoinCollect()), 2));  //coin
	font.Draw(x + 47, y + 20, FillNumber(std::to_string(20), 2)); // mạng
}