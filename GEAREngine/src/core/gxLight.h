#ifndef GXLIGHT_H
#define GXLIGHT_H

#include "object3d.h"
#include "vector4.h"

//http://antonholmquist.com/blog/opengl-es-2-0-shader-lighting-examples/
//http://xissburg.com/faster-gaussian-blur-in-glsl/
//http://blog.angusforbes.com/openglglsl-render-to-texture/

class DllExport gxLight : public object3d
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
	void render(gxRenderer* renderer);

	void renderPass(gxRenderer* renderer, gxHWShader* shader);

	void setDiffuseColor(vector4f& clr)		{	m_cDiffuse=clr;	}
	void setAmbientColor(vector4f& clr)		{	m_cAmbient=clr;	}
	void setSpecularColor(vector4f& clr)	{	m_cSpecular=clr;	}

	const vector4f& getDiffuseColor()	{	return m_cDiffuse;	}
	const vector4f& getSpecularColor()	{	return m_cSpecular;	}
	const vector4f& getAmbientColor()	{	return m_cAmbient;	}
	ELIGHT_TYPE	getLightType()			{	return m_eType;		}
	void setLightType(ELIGHT_TYPE eType){	m_eType=m_eType;	}
private:
	ELIGHT_TYPE m_eType;
	vector4f m_cDiffuse;
	vector4f m_cSpecular;
	vector4f m_cAmbient;
};

#endif