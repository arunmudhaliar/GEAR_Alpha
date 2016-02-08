#include "gxLight.h"
#include "../GEAREngine.h"

gxLight::gxLight():
object3d(OBJECT3D_LIGHT)
{
	setName("Point Light");
	setLightType(LIGHT_POINT);
	m_cOOBB.set(vector3f(-5, -5, -5), vector3f(5, 5, 5));

	diffuseColor.set(0.7f, 0.7f, 0.7f, 1.0f);
	ambientColor.set(0.05f, 0.05f, 0.05f, 1.0f);
	specularColor.set(0.2f, 0.2f, 0.2f, 1.0f);
	setPosition(0, 0, 1);

	constantAttenuation = 0.1f;
	linearAttenuation = 0.0025f;
	quadraticAttenuation = 0.0006f;

	//shadow mapping
	shadowMapFBO.ReInitFBO(GX_SHADOW_MAP_SIZE, GX_SHADOW_MAP_SIZE);
	shadowMapFBO.CreateDepthOnlyBuffer();
	shadowMapFBO.AttachDepthBuffer();
	shadowMapFBO.UnBindFBO();

	matrix4x4f biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	biasMatrix = biasMatrix;	//TODO: optimize this.

	depthProjectionMatrix.setOrtho(-100, 100,
		-100, 100, 0, 1000);
	//
}

gxLight::~gxLight()
{
}

void gxLight::update(float dt)
{
	object3d::update(dt);

	matrix4x4f depthViewMatrix;
	vector3f light_pos(getPosition());
	vector3f forward(light_pos);
	forward.normalize();
	vector3f up(0, 0, 1);
	vector3f left(up.cross(forward));
	left.normalize();
	up = forward.cross(left);
	up.normalize();
	depthViewMatrix.setXAxis(left);
	depthViewMatrix.setYAxis(up);
	depthViewMatrix.setZAxis(forward);
	depthViewMatrix.setPosition(light_pos);

	depthMVPMatrix = depthProjectionMatrix * depthViewMatrix.getInverse();
	depthBiasMVPMatrix = biasMatrix * depthMVPMatrix;
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
	shader->sendUniform4fv("light.diffuse", &diffuseColor.x);
	shader->sendUniform4fv("light.ambient", &ambientColor.x);
	shader->sendUniform4fv("light.specular", &specularColor.x);
	shader->sendUniform1f("light.constant_attenuation", constantAttenuation);
	shader->sendUniform1f("light.linear_attenuation", linearAttenuation);
	shader->sendUniform1f("light.quadratic_attenuation", quadraticAttenuation);

	vector3f eye(renderer->getMainCameraEye());
	shader->sendUniform3fv("_WorldSpaceCameraPos", &eye.x);
	//vector3f lightPos(*renderer->getViewMatrix() * getWorldMatrix()->getPosition());
	vector3f lightPos(getWorldMatrix()->getPosition());

	shader->sendUniform4f("light.position", lightPos.x, lightPos.y, lightPos.z, (lightType==LIGHT_POINT)?1.0f:0.0f);
#endif
}

void gxLight::write(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Write(assetFileCRC);
	writeAnimationController(file);

	//write light data
	file.Write(lightType);
	file.WriteBuffer((unsigned char*)&ambientColor, sizeof(ambientColor));
	file.WriteBuffer((unsigned char*)&diffuseColor, sizeof(diffuseColor));
	file.WriteBuffer((unsigned char*)&specularColor, sizeof(specularColor));
	file.Write(constantAttenuation);
	file.Write(linearAttenuation);
	file.Write(quadraticAttenuation);
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
	file.Read(assetFileCRC);
	readAnimationController(file);

	//read light data
	int type=0;
	file.Read(type);
	lightType=(ELIGHT_TYPE)type;
	file.ReadBuffer((unsigned char*)&ambientColor, sizeof(ambientColor));
	file.ReadBuffer((unsigned char*)&diffuseColor, sizeof(diffuseColor));
	file.ReadBuffer((unsigned char*)&specularColor, sizeof(specularColor));
	file.Read(constantAttenuation);
	file.Read(linearAttenuation);
	file.Read(quadraticAttenuation);
	//
}