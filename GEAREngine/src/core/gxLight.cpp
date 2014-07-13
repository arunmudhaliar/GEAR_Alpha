#include "gxLight.h"
#include "../GEAREngine.h"

gxLight::gxLight():
object3d(OBJECT3D_LIGHT)
{
	setName("Point Light");
	setLightType(LIGHT_POINT);
	m_cOOBB.set(vector3f(-5, -5, -5), vector3f(5, 5, 5));

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

void gxLight::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

	object3d::render(renderer, light, renderFlag);
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

void gxLight::write(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Write(m_iAssetFileCRC);
	writeAnimationController(file);

	//write light data
	file.Write(m_eType);
	file.WriteBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
	file.WriteBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
	file.WriteBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
	file.Write(m_fConstantAttenuation);
	file.Write(m_fLinearAttenuation);
	file.Write(m_fQuadraticAttenuation);
	//

	file.Write((int)m_cChilds.size());
#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=m_cChilds.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->write(file);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
#endif
}

void gxLight::read(gxFile& file)
{
	file.Read(m_eBaseFlags);
	char* temp=file.ReadString();
	GX_STRCPY(m_cszName, temp);
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Read(m_iAssetFileCRC);
	readAnimationController(file);

	//read light data
	int type=0;
	file.Read(type);
	m_eType=(ELIGHT_TYPE)type;
	file.ReadBuffer((unsigned char*)&m_cAmbient, sizeof(m_cAmbient));
	file.ReadBuffer((unsigned char*)&m_cDiffuse, sizeof(m_cDiffuse));
	file.ReadBuffer((unsigned char*)&m_cSpecular, sizeof(m_cSpecular));
	file.Read(m_fConstantAttenuation);
	file.Read(m_fLinearAttenuation);
	file.Read(m_fQuadraticAttenuation);
	//
}