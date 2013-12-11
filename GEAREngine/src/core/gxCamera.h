#ifndef GXCAMERA_H
#define GXCAMERA_H

#include "object3d.h"
#include "../renderer/gxRenderer.h"

class DllExport gxCamera : public object3d
{
public:
	enum EPROJECTION_TYPE
	{
		PERSPECTIVE_PROJECTION,
		ORTHOGRAPHIC_PROJECTION
	};

	gxCamera();
	~gxCamera();

	void setFOV(float fov)	{	m_FOV=fov;	}
	void setNear(float n)	{	m_fNear=n;	}
	void setFar(float f)	{	m_fFar=f;	}
	void setType(EPROJECTION_TYPE type)	{	m_eProjectionType=type;	}

	float getFOV()			{	return m_FOV;	}
	float getNear()			{	return m_fNear;	}
	float getFar()			{	return m_fFar;	}
	EPROJECTION_TYPE getProjectionType()	{	return m_eProjectionType;	}

private:
	float m_FOV;
	float m_fNear;
	float m_fFar;
	EPROJECTION_TYPE m_eProjectionType;
};

#endif