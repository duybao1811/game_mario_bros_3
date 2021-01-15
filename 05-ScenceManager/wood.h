#include "GameObject.h"
#define WOOD_BBOX_WIDTH 48
#define WOOD_BBOX_HEIGHT 16
#define WOOD_SPEED_X 0.2f
#define WOOD_SPEED_Y 0.1f
#define WOOD_ANI 0
class Wood : public CGameObject
{
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	Wood();
	~Wood() {}
};
