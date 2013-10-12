#include "gxLight.h"
#include "../GEAREngine.h"

gxLight::gxLight():
object3d(3)
{
	setName("Point Light");
	setLightType(LIGHT_POINT);

	m_cDiffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
	m_cAmbient.set(0.0f, 0.0f, 0.0f, 1.0f);
	m_cSpecular.set(0.0f, 0.0f, 0.0f, 1.0f);
	setPosition(0, 0, 1);
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

void gxLight::renderPass(gxRenderer* renderer, gxHWShader* shader)
{
#if defined (USE_ProgrammablePipeLine)
	shader->sendUniform4fv("u_light_diffuse_v4", &m_cDiffuse.x);
	shader->sendUniform4fv("u_light_ambient_v4", &m_cAmbient.x);
	shader->sendUniform4fv("u_light_specular_v4", &m_cSpecular.x);

	//matrix4x4f* viewMatrix = renderer->getViewMatrix();
	vector3f eye(renderer->getMainCameraEye());
	shader->sendUniform3fv("u_eyedirection_v3", &eye.x);

	vector3f lightPos(/**renderer->getViewMatrix() * */getWorldMatrix()->getPosition());
	float t=lightPos.x;
	lightPos.x=-lightPos.y;
	lightPos.y=t;
	shader->sendUniform3fv("u_lightposition_v3", &lightPos.x);
#endif
}