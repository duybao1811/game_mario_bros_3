#include "Power.h"
Power::Power()
{
	Tex = CTextures::GetInstance()->Get(TexID_BOARD);
	Sprite = new CSprite(1,3,33,11,40, Tex);
}
void Power::Draw(float x, float y)
{
	Sprite->Draw(x, y);
}