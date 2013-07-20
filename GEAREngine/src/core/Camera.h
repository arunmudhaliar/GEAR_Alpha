#ifndef CAMERA_H
#define CAMERA_H

//#include "../renderer/rendererGL10.h"
//#include "../renderer/rendererGL20.h"
//#include "gxCamera.h"
//#include "frustum.h"

#include "object3d.h"
#include "../renderer/gxRenderer.h"

class Camera : public object3d
{
public:
	Camera();
	~Camera();

	void		resetCamera();

	void		initCamera(gxRenderer* renderer);
	void		processCamera(matrix4x4f* matrix=NULL);
	void		updateCamera();
	//void		setUpViewPort(GXint aWidth=1, GXint aHeight=1);
	void		setUpCameraPerspective(float cx, float cy, float fov, float nearValue, float farValue);
	//void		setCamera(gxCamera* aCamera);
	//gxCamera*	getCameraStructure()						{	return m_pCurrentCamPtr;	}

	void		extractFrustumPlanes();
	virtual void		transformationChangedf();
	virtual void		calculateAABB();
	vector3f	getCameraSpaceLoc(const vector3f& point);

	void		drawFrustum();
    //void render(const matrix4x4f* parentTM);

	//gxFrustumf&	getFrustum()						{	return m_cFrustum;			}
	const matrix4x4f*	getProjectionMatrix()		{	return &m_cProjMatrix;		}
	const matrix4x4f*	getInverseTMViewMatrix()	{	return &m_cInvTranfMatrix;	}
	const matrix4x4f*	getViewMatrix()				{	return &m_cInvTranfMatrix;	}

private:
	gxRenderer* m_pRendererPtr;	//must not delete this pointer
	//gxCamera*	m_pCurrentCamPtr;	//must not delete this pointer
	matrix4x4f	m_cProjMatrix;
	matrix4x4f	m_cInvTranfMatrix;
	matrix4x4f	m_cViewProjectionMatrix;

	//gxFrustumf	m_cFrustum;
	float m_fFOV;	//in angles
	float m_fNear;
	float m_fFar;
};

#endif