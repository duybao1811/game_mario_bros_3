#include "FullPower.h"
FullPower::FullPower()
{
	Tex = CTextures::GetInstance()->Get(TexID_BOARD);
	Sprite = new CSprite(1, 12, 33, 27, 40, Tex);
}
void FullPower::Draw(float x, float y)
{
	Sprite->Draw(x, y);
}