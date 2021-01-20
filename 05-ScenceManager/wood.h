#include "GameObject.h"
#define WOOD_BBOX_WIDTH 48
#define WOOD_BBOX_HEIGHT 16
#define WOOD_SPEED_X 0.003f
#define WOOD_GRAVITY 0.002f
#define WOOD_ANI 0
#define WOOD_STATE_FLY 100
#define WOOD_STATE_FALLING 200
class Wood : public CGameObject
{
public:
	bool isCollisMario;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	Wood();
	~Wood() {}
};
