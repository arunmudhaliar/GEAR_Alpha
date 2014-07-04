#ifndef GXLIGHT_H
#define GXLIGHT_H

#include "object3d.h"
#include "vector4.h"

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

	void setDiffuseColor(vector4f& clr)		{	m_cDiffuse=clr;	}
	void setAmbientColor(vector4f& clr)		{	m_cAmbient=clr;	}
	void setSpecularColor(vector4f& clr)	{	m_cSpecular=clr;	}

	const vector4f& getDiffuseColor()	{	return m_cDiffuse;	}
	const vector4f& getSpecularColor()	{	return m_cSpecular;	}
	const vector4f& getAmbientColor()	{	return m_cAmbient;	}
	ELIGHT_TYPE	getLightType()			{	return m_eType;		}
	void setLightType(ELIGHT_TYPE eType){	m_eType=eType;		}

	float getConstantAttenuation()	{	return m_fConstantAttenuation;	}
	float getLinearAttenuation()	{	return m_fLinearAttenuation;	}
	float getQuadraticAttenuation()	{	return m_fQuadraticAttenuation;	}

	void setConstantAttenuation(float value)	{	m_fConstantAttenuation=value;	}
	void setLinearAttenuation(float value)		{	m_fLinearAttenuation=value;		}
	void setQuadraticAttenuation(float value)	{	m_fQuadraticAttenuation=value;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

private:
	ELIGHT_TYPE m_eType;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	vector4f m_cAmbient;
	float m_fConstantAttenuation;
	float m_fLinearAttenuation;
	float m_fQuadraticAttenuation;
};

#endif