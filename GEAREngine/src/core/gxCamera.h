#pragma once

#include "object3d.h"
#include "frustum.h"
#include "../renderer/gxRenderer.h"

class DECLSPEC gxCamera : public object3d
{
public:
	enum EPROJECTION_TYPE
	{
		PERSPECTIVE_PROJECTION,
		ORTHOGRAPHIC_PROJECTION
	};

	gxCamera();
	~gxCamera();
    static gxCamera* create();
    
	void setFOV(float fov);
	void setNear(float n);
	void setFar(float f);
	void setType(EPROJECTION_TYPE type);

	float getFOV()			{	return fov;         }
	float getNear()			{	return nearValue;	}
	float getFar()			{	return farValue;	}
	EPROJECTION_TYPE getProjectionType()	{	return projectionType;	}
	void		extractFrustumPlanes();
	gxFrustumf* getFrustum()	{	return &frustum;	}

private:

	void perspectiveChanged();

	float fov;
	float nearValue;
	float farValue;
	EPROJECTION_TYPE projectionType;

	gxFrustumf	frustum;
	matrix4x4f	dummyProjectionMatrix;
};