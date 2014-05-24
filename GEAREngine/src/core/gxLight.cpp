#include "gxLight.h"
#include "../GEAREngine.h"

gxLight::gxLight():
object3d(OBJECT3D_LIGHT)
{
	setName("Point Light");
	setLightType(LIGHT_POINT);

	m_cDiffuse.set(0.7f, 0.7f, 0.7f, 1.0f);
	m_cAmbient.set(0.05f, 0.05f, 0.05f, 1.0f);
	m_cSpecular.set(0.2f, 0.2f, 0.2f, 1.0f);
	setPosition(0, 0, 1);

	m_fConstantAttenuation = 0.1f;
	m_fLinearAttenuation = 0.0025f;
	m_fQuadraticAttenuation = 0.0006f;
}

gxLight::~gxLight()
{
}

void gxLight::update(float dt)
{
	object3d::update(dt);
}

void gxLight::render(gxRenderer* renderer, object3d* light)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	object3d::render(renderer, light);
}

void gxLight::renderPass(gxRenderer* renderer, gxHWShader* shader)
{
#if defined (USE_ProgrammablePipeLine)
	shader->sendUniform4fv("light.diffuse", &m_cDiffuse.x);
	shader->sendUniform4fv("light.ambient", &m_cAmbient.x);
	shader->sendUniform4fv("light.specular", &m_cSpecular.x);
	shader->sendUniform1f("light.constant_attenuation", m_fConstantAttenuation);
	shader->sendUniform1f("light.linear_attenuation", m_fLinearAttenuation);
	shader->sendUniform1f("light.quadratic_attenuation", m_fQuadraticAttenuation);

	vector3f eye(renderer->getMainCameraEye());
	shader->sendUniform3fv("_WorldSpaceCameraPos", &eye.x);
	//vector3f lightPos(*renderer->getViewMatrix() * getWorldMatrix()->getPosition());
	vector3f lightPos(getWorldMatrix()->getPosition());

	shader->sendUniform4f("light.position", lightPos.x, lightPos.y, lightPos.z, (m_eType==LIGHT_POINT)?1.0f:0.0f);
#endif
}