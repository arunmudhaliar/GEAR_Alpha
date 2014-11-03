#ifndef CAMERA_H
#define CAMERA_H

//#include "../renderer/rendererGL10.h"
//#include "../renderer/rendererGL20.h"
//#include "gxCamera.h"
//#include "frustum.h"

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

	gxFrustumf&	getFrustum()					{	return m_cFrustum;			}
	const matrix4x4f* getProjectionMatrix()		{	return &m_cProjMatrix;		}
	const matrix4x4f* getInverseTMViewMatrix()	{	return &m_cInvTranfMatrix;	}
	const matrix4x4f* getViewMatrix()			{	return &m_cInvTranfMatrix;	}

	void setFOV(float fov);
	void setNear(float n);
	void setFar(float f);
	void setType(EPROJECTION_TYPE type);
	void perspectiveChanged();

	float getFOV()			{	return m_fFOV;	}
	float getNear()			{	return m_fNear;	}
	float getFar()			{	return m_fFar;	}
	EPROJECTION_TYPE getProjectionType()	{	return m_eProjectionType;	}

	bool isLayerCullingMask(unsigned int flag);
	void resetLayerCullingMask(unsigned int flag);
	void setLayerCullingMask(unsigned int flag);
	unsigned int getLayerCullingMask();
	
	void setMainCamera(bool flag)	{	m_bMainCamera=flag;	}
	bool isMainCamera()		{	return m_bMainCamera;	}

	virtual void write(gxFile& file);
	virtual void read(gxFile& file);

private:
	gxRenderer* m_pRendererPtr;	//must not delete this pointer
	matrix4x4f	m_cProjMatrix;
	matrix4x4f	m_cInvTranfMatrix;
	matrix4x4f	m_cViewProjectionMatrix;

	gxFrustumf	m_cFrustum;
	gxFrustumf	m_cRenderFrustum;	//used only for rendering purpose;

	float m_fFOV;	//in angles
	float m_fNear;
	float m_fFar;
	EPROJECTION_TYPE m_eProjectionType;
	unsigned int m_iLayerCullingMask;
	bool m_bMainCamera;
};

#endif