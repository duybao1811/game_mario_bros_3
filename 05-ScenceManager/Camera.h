#pragma once
#include <d3dx9.h>
class Camera
{
protected:
	int width; 
	int height;
	float xCam;
	float yCam;
	float xCamBackup;
	float yCamBackup;
	float BorderRight;
	float BorderLeft;
	float BorderRightBackup;
	float BorderLeftBackup;
	float vx;
	//int Width;
	//int Height;
public: 
	Camera(float w,float h);
	~Camera();
	void Update(DWORD dt);
	D3DXVECTOR2 Transform(float, float);
	void SetPosition(float x, float y);
	float GetxCam();
	float GetyCam();
	int GetWidth();
	int GetHeight();
	void SetBorder(float left, float right);
	float GetBorderLeft();
	float GetBorderRight();
	void SetPositonBackup(float X, float Y);
	void SetBorderBackup(float LEFT, float RIGHT);
	void RestoreBorder();
	void RestorePosition();
	bool checkObjectInCamera(float x, float y, float w, float h);
};

