#include "gxCamera.h"

gxCamera::gxCamera():
	object3d(OBJECT3D_CAMERA)
{
	setFOV(45.0f);
	setNear(1.0f);
	setFar(10000.0f);
	setType(PERSPECTIVE_PROJECTION);
	updateLocalPositionf(0, 0, 300);
}

gxCamera::~gxCamera()
{
}