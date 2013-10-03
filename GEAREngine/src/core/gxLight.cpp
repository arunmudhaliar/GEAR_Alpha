#include "gxLight.h"

gxLight::gxLight():
object3d(3)
{
	setName("Point Light");
	setLightType(LIGHT_POINT);
}

gxLight::~gxLight()
{
}

void gxLight::update(float dt)
{
	object3d::update(dt);
}

void gxLight::render(gxRenderer* renderer)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	object3d::render(renderer);
}