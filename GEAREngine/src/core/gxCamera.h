#ifndef GXCAMERA_H
#define GXCAMERA_H

#include "object3d.h"
#include "frustum.h"
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

	void setFOV(float fov);
	void setNear(float n);
	void setFar(float f);
	void setType(EPROJECTION_TYPE type);

	float getFOV()			{	return m_FOV;	}
	float getNear()			{	return m_fNear;	}
	float getFar()			{	return m_fFar;	}
	EPROJECTION_TYPE getProjectionType()	{	return m_eProjectionType;	}
	void		extractFrustumPlanes();
	gxFrustumf* getFrustum()	{	return &m_cFrustum;	}

private:

	void perspectiveChanged();

	float m_FOV;
	float m_fNear;
	float m_fFar;
	EPROJECTION_TYPE m_eProjectionType;

	gxFrustumf	m_cFrustum;
	matrix4x4f	m_cDummyProjMatrix;
};

#endif