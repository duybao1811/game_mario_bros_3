#include "Camera.h"
#include <d3d9.h>
Camera::Camera(float w, float h)
{
	width = w;
	height = h;
	BorderLeft = 0;
	BorderRight = 0;
	xCamBackup = 0;
	yCamBackup = 0;
	SetBorderBackup(BorderLeft, BorderRight);
}
Camera::~Camera()
{

}
void Camera::SetPosition(float x, float y)
{
	xCam = x;
	yCam = y;
}
float Camera::GetxCam()
{
	return xCam;
}
float Camera::GetyCam()
{
	return yCam;
}
int Camera::GetWidth()
{
	return width;
}
int Camera::GetHeight()
{
	return height;
}
void Camera::SetBorder(float left, float right)
{
	BorderLeft = left;
	BorderRight = right;
}
float Camera::GetBorderLeft()
{
	return BorderLeft;
}
float Camera::GetBorderRight()
{
	return BorderRight;
}
void Camera::SetPositonBackup(float X, float Y)
{
	xCamBackup = X;
	yCamBackup = Y;
}
void Camera::SetBorderBackup(float LEFT, float RIGHT)
{
	BorderLeftBackup = LEFT;
	BorderRightBackup = RIGHT;
}
void Camera::RestoreBorder()
{
	BorderLeft = BorderLeftBackup;
	BorderRight = BorderRightBackup;
}
void Camera::RestorePosition()
{
	xCam = xCamBackup;
	yCam = yCamBackup;
	BorderLeft = BorderLeftBackup;
	BorderRight = BorderRightBackup;
}
bool Camera::checkObjectInCamera(float x, float y, float w, float h)
{
	if (x + w < xCam || xCam + width < x)
		return false;
	if (y + h < yCam || yCam + height < y)
		return false;
	return true;
}
D3DXVECTOR2 Camera::Transform(float xWorld,float yWorld)
{
	return D3DXVECTOR2(xWorld - xCam, yWorld - yCam);
}