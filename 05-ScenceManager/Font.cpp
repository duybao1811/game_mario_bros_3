#include "Font.h"
Font::Font()
{
	Tex= CTextures::GetInstance()->Get(TexID_Font);
}
void Font::Draw(float x, float y, const string& s)
{
	for (UINT i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			int left = (s[i] - 'A') % 26 * TILE_FONT_WIDTH;
			int top = (s[i] - 'A') / 26 * TILE_FONT_A_HEIGHT;
			int right = left + TILE_FONT_WIDTH;
			int bottom = top + TILE_FONT_A_HEIGHT;
			LPSPRITE SpriteTile = new CSprite((s[i] - 'A'), left, top, right, bottom, Tex);
			SpriteTile->Draw(x + i * 8, y);
		}
		else
			if (s[i] >= '0' && s[i] <= '9')
			{
				int left = (s[i] - '0' + 26) % 26 * TILE_FONT_WIDTH;
				int top = ((s[i] - '0' + 26) / 26 * TILE_FONT_NUMBER_HEIGHT)+1;
				int right = left + TILE_FONT_WIDTH;
				int bottom = top + TILE_FONT_NUMBER_HEIGHT;
				LPSPRITE SpriteTile = new CSprite((s[i] - '0' + 26), left, top, right, bottom, Tex);
				SpriteTile->Draw(x + i * 8, y);
			}
	}
}