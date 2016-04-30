#pragma once

#include "../core/object3d.h"
#include "../core/gxColor.h"
#include "../core/fbo.h"

#define GX_SHADOW_MAP_SIZE	256

//http://antonholmquist.com/blog/opengl-es-2-0-shader-lighting-examples/
//http://xissburg.com/faster-gaussian-blur-in-glsl/
//http://blog.angusforbes.com/openglglsl-render-to-texture/

class DECLSPEC gxLight : public monoScriptObjectInstance
{
public:
	~gxLight();

    static gxLight* create(monoClassDef* script, object3d* obj);
    
	enum ELIGHT_TYPE
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
	};

	void update(float dt);
	void render(gxRenderer* renderer, monoScriptObjectInstance* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);

	void renderPass(gxRenderer* renderer, gxHWShader* shader);

	void setDiffuseColor(const gxColor& clr)		{	diffuseColor=clr;	}
	void setAmbientColor(const gxColor& clr)		{	ambientColor=clr;	}
	void setSpecularColor(const gxColor& clr)	{	specularColor=clr;	}

	const gxColor& getDiffuseColor()	{	return diffuseColor;	}
	const gxColor& getSpecularColor()	{	return specularColor;	}
	const gxColor& getAmbientColor()	{	return ambientColor;	}
	ELIGHT_TYPE	getLightType()			{	return lightType;		}
	void setLightType(ELIGHT_TYPE eType){	lightType=eType;		}

	float getConstantAttenuation()	{	return constantAttenuation;	}
	float getLinearAttenuation()	{	return linearAttenuation;	}
	float getQuadraticAttenuation()	{	return quadraticAttenuation;	}

	void setConstantAttenuation(float value)	{	constantAttenuation=value;	}
	void setLinearAttenuation(float value)		{	linearAttenuation=value;		}
	void setQuadraticAttenuation(float value)	{	quadraticAttenuation=value;	}

	//shadow mapping
	matrix4x4f& getShadowDepthMVP()		{ return depthMVPMatrix; }
	matrix4x4f& getShadowDepthBiasMVP()	{ return depthBiasMVPMatrix; }
	FBO& getShadowMapFBO()				{ return shadowMapFBO; }

protected:
    void readScriptObject(gxFile& file);
    void writeScriptObject(gxFile& file);

private:
    gxLight(monoClassDef* script, object3d* obj);

	ELIGHT_TYPE lightType;
	gxColor diffuseColor;
	gxColor specularColor;
	gxColor ambientColor;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;

	//shadow mapping
	FBO shadowMapFBO;

	matrix4x4f depthMVPMatrix;
	matrix4x4f biasMatrix;
	matrix4x4f depthBiasMVPMatrix;
	matrix4x4f depthProjectionMatrix;
};

////////////////////GXLIGHT C# BRIDGE////////////////////
extern "C" {
    DECLSPEC void gxLight_setLightType(gxLight* light, gxLight::ELIGHT_TYPE eType);
    DECLSPEC gxLight::ELIGHT_TYPE gxLight_getLightType(gxLight* light);
    DECLSPEC void gxLight_setDiffuseColor(gxLight* light, float clr[4]);
    DECLSPEC void gxLight_setAmbientColor(gxLight* light, float clr[4]);
    DECLSPEC void gxLight_setSpecularColor(gxLight* light, float clr[4]);
    DECLSPEC void gxLight_getDiffuseColor(gxLight* light, float* clr);
    DECLSPEC void gxLight_getAmbientColor(gxLight* light, float* clr);
    DECLSPEC void gxLight_getSpecularColor(gxLight* light, float* clr);
    
    DECLSPEC void gxLight_setConstantAttenuation(gxLight* light, float value);
    DECLSPEC void gxLight_setLinearAttenuation(gxLight* light, float value);
    DECLSPEC void gxLight_setQuadraticAttenuation(gxLight* light, float value);
    DECLSPEC float gxLight_getConstantAttenuation(gxLight* light);
    DECLSPEC float gxLight_getLinearAttenuation(gxLight* light);
    DECLSPEC float gxLight_getQuadraticAttenuation(gxLight* light);
}