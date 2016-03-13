#pragma once

#include "object3d.h"
#include "vector4.h"
#include "fbo.h"

#define GX_SHADOW_MAP_SIZE	1024

//http://antonholmquist.com/blog/opengl-es-2-0-shader-lighting-examples/
//http://xissburg.com/faster-gaussian-blur-in-glsl/
//http://blog.angusforbes.com/openglglsl-render-to-texture/

class DECLSPEC gxLight : public object3d
{
public:
	gxLight();
	~gxLight();

	enum ELIGHT_TYPE
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
	};

	void update(float dt);
	void render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/);

	void renderPass(gxRenderer* renderer, gxHWShader* shader);

	void setDiffuseColor(const vector4f& clr)		{	diffuseColor=clr;	}
	void setAmbientColor(const vector4f& clr)		{	ambientColor=clr;	}
	void setSpecularColor(const vector4f& clr)	{	specularColor=clr;	}

	const vector4f& getDiffuseColor()	{	return diffuseColor;	}
	const vector4f& getSpecularColor()	{	return specularColor;	}
	const vector4f& getAmbientColor()	{	return ambientColor;	}
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
    virtual void writeData(gxFile& file);
    virtual void readData(gxFile& file);

private:
	ELIGHT_TYPE lightType;
	vector4f diffuseColor;
	vector4f specularColor;
	vector4f ambientColor;
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