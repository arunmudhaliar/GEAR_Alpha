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

	m_fConstantAttenuation = 0.1f;
	m_fLinearAttenuation = 0.01f;
	m_fQuadraticAttenuation = 0.001f;
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

	////matrix4x4f* viewMatrix = renderer->getViewMatrix();
	//vector3f eye(renderer->getMainCameraEye());
	//shader->sendUniform3fv("u_eyedirection_v3", &eye.x);

	vector3f lightPos(*renderer->getViewMatrix() * getWorldMatrix()->getPosition());
	//float t=lightPos.x;
	//lightPos.x=-lightPos.y;
	//lightPos.y=t;
	shader->sendUniform4f("light.position", lightPos.x, lightPos.y, lightPos.z, 1.0f);
#endif
}