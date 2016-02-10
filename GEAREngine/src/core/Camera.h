#pragma once

#include "gxCamera.h"
#include "object3d.h"
#include "../renderer/gxRenderer.h"

class DECLSPEC Camera : public object3d
{
public:
	Camera();
	~Camera();

	enum EPROJECTION_TYPE
	{
		PERSPECTIVE_PROJECTION,
		ORTHOGRAPHIC_PROJECTION
	};

	void resetCamera();
		 
	void initCamera(gxRenderer* renderer);
	void processCamera(matrix4x4f* matrix=NULL);
	void updateCamera();
	void setUpCameraPerspective(float cx, float cy/*, float fov, float nearValue, float farValue*/);

	void extractFrustumPlanes();
	virtual void transformationChangedf();
	virtual void calculateAABB();
	vector3f getCameraSpaceLoc(const vector3f& point);

	void drawFrustum(gxHWShader* shader);

	gxFrustumf&	getFrustum()					{	return frustum;			}
	const matrix4x4f* getProjectionMatrix()		{	return &projectionMatrix;		}
	const matrix4x4f* getInverseTMViewMatrix()	{	return &inverseTransformationMatrix;	}
	const matrix4x4f* getViewMatrix()			{	return &inverseTransformationMatrix;	}

	void setFOV(float fov);
	void setNear(float n);
	void setFar(float f);
	void setType(EPROJECTION_TYPE type);
	void perspectiveChanged();

	float getFOV()			{	return fov;         }
	float getNear()			{	return nearValue;	}
	float getFar()			{	return farValue;         }
	EPROJECTION_TYPE getProjectionType()	{	return projectionType;	}

	bool isLayerCullingMask(unsigned int flag);
	void resetLayerCullingMask(unsigned int flag);
	void setLayerCullingMask(unsigned int flag);
	unsigned int getLayerCullingMask();
	
	void setMainCamera(bool flag)	{	isMainCam=flag;     }
	bool isMainCamera()             {	return isMainCam;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

private:
	gxRenderer* renderer;	//must not delete this pointer
	matrix4x4f	projectionMatrix;
	matrix4x4f	inverseTransformationMatrix;
	matrix4x4f	viewProjectionMatrix;

	gxFrustumf	frustum;
	gxFrustumf	renderFrustum;	//used only for rendering purpose;

	float fov;	//in angles
	float nearValue;
	float farValue;
	EPROJECTION_TYPE projectionType;
	unsigned int layerCullingMask;
	bool isMainCam;
};