#include "gxLight.h"
#include "../GEAREngine.h"

////////////////////GXLIGHT C# BRIDGE////////////////////
extern "C" {
    DECLSPEC void gxLight_setLightType(gxLight* light, gxLight::ELIGHT_TYPE eType)
    {
        light->setLightType(eType);
    }
    DECLSPEC gxLight::ELIGHT_TYPE gxLight_getLightType(gxLight* light)
    {
        return light->getLightType();
    }
    DECLSPEC void gxLight_setDiffuseColor(gxLight* light, float clr[4])
    {
        light->setDiffuseColor(clr);
    }
    DECLSPEC void gxLight_setAmbientColor(gxLight* light, float clr[4])
    {
        light->setAmbientColor(clr);
    }
    DECLSPEC void gxLight_setSpecularColor(gxLight* light, float clr[4])
    {
        light->setSpecularColor(clr);
    }
    
    DECLSPEC void gxLight_getDiffuseColor(gxLight* light, float* clr)
    {
        clr[0]=light->getDiffuseColor().x;
        clr[1]=light->getDiffuseColor().y;
        clr[2]=light->getDiffuseColor().z;
        clr[3]=light->getDiffuseColor().w;
    }
    
    DECLSPEC void gxLight_getAmbientColor(gxLight* light, float* clr)
    {
        clr[0]=light->getAmbientColor().x;
        clr[1]=light->getAmbientColor().y;
        clr[2]=light->getAmbientColor().z;
        clr[3]=light->getAmbientColor().w;
    }
    
    DECLSPEC void gxLight_getSpecularColor(gxLight* light, float* clr)
    {
        clr[0]=light->getSpecularColor().x;
        clr[1]=light->getSpecularColor().y;
        clr[2]=light->getSpecularColor().z;
        clr[3]=light->getSpecularColor().w;
    }
    
    DECLSPEC void gxLight_setConstantAttenuation(gxLight* light, float value)
    {
        light->setConstantAttenuation(value);
    }
    
    DECLSPEC void gxLight_setLinearAttenuation(gxLight* light, float value)
    {
        light->setLinearAttenuation(value);
    }
    
    DECLSPEC void gxLight_setQuadraticAttenuation(gxLight* light, float value)
    {
        light->setQuadraticAttenuation(value);
    }
    
    DECLSPEC float gxLight_getConstantAttenuation(gxLight* light)
    {
        return light->getConstantAttenuation();
    }
    
    DECLSPEC float gxLight_getLinearAttenuation(gxLight* light)
    {
        return light->getLinearAttenuation();
    }
    
    DECLSPEC float gxLight_getQuadraticAttenuation(gxLight* light)
    {
        return light->getQuadraticAttenuation();
    }
}

gxLight* gxLight::create(monoClassDef* script, object3d* obj)
{
    auto newObject = new gxLight(script, obj);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxLight::gxLight(monoClassDef* script, object3d* obj):
    monoScriptObjectInstance(script, obj)
{
	//setName("Point Light");
	setLightType(LIGHT_POINT);
	//oobb.set(vector3f(-5, -5, -5), vector3f(5, 5, 5));

	diffuseColor.set(0.7f, 0.7f, 0.7f, 1.0f);
	ambientColor.set(0.05f, 0.05f, 0.05f, 1.0f);
	specularColor.set(0.2f, 0.2f, 0.2f, 1.0f);
	//setPosition(0, 0, 1);

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
	monoScriptObjectInstance::update(dt);

	matrix4x4f depthViewMatrix;
	vector3f light_pos(getAttachedObject()->getPosition());
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
    if(!getAttachedObject()->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
		return;

	monoScriptObjectInstance::render(renderer, light, renderFlag);
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
	vector3f lightPos(getAttachedObject()->getWorldMatrix()->getPosition());

	shader->sendUniform4f("light.position", lightPos.x, lightPos.y, lightPos.z, (lightType==LIGHT_POINT)?1.0f:0.0f);
#endif
}

void gxLight::writeData(gxFile& file)
{
	//write light data
	file.Write(lightType);
	file.WriteBuffer((unsigned char*)&ambientColor, sizeof(ambientColor));
	file.WriteBuffer((unsigned char*)&diffuseColor, sizeof(diffuseColor));
	file.WriteBuffer((unsigned char*)&specularColor, sizeof(specularColor));
	file.Write(constantAttenuation);
	file.Write(linearAttenuation);
	file.Write(quadraticAttenuation);
	//
}

void gxLight::readData(gxFile& file)
{
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