#include "gxCamera.h"

gxCamera* gxCamera::create()
{
    auto newObject = new gxCamera();
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxCamera::gxCamera():
	object3d(OBJECT3D_CAMERA_STRUCT)
{
	setFOV(45.0f);
	setNear(10.0f);
	setFar(10000.0f);
	setType(PERSPECTIVE_PROJECTION);
	updateLocalPositionf(0, 0, 300);

	perspectiveChanged();
}

gxCamera::~gxCamera()
{
}

void gxCamera::setFOV(float fov)
{
	this->fov = fov;
	perspectiveChanged();
}

void gxCamera::setNear(float n)
{
	nearValue = n;
	perspectiveChanged();
}

void gxCamera::setFar(float f)
{
	farValue = f;
	perspectiveChanged();
}

void gxCamera::setType(EPROJECTION_TYPE type)
{
	projectionType=type;
	perspectiveChanged();
}

void gxCamera::perspectiveChanged()
{
	if(getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		dummyProjectionMatrix.setPerspective(fov, 1.0f, nearValue, farValue);
	else
	{
		gxRectf viewportRect(0, 0, 200, 200);
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		dummyProjectionMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, nearValue, farValue);
	}

	extractFrustumPlanes();
}

void gxCamera::extractFrustumPlanes()
{
	matrix4x4f clipMatrix;
	
	clipMatrix	= dummyProjectionMatrix; //* this->getInverse();
	//right plane
	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.x	= clipMatrix[3]-clipMatrix[0];
	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.y	= clipMatrix[7]-clipMatrix[4];
	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.z	= clipMatrix[11]-clipMatrix[8];
	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].d			= clipMatrix[15]-clipMatrix[12];
	
	//left plane
	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.x	= clipMatrix[3]+clipMatrix[0];
	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.y	= clipMatrix[7]+clipMatrix[4];
	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.z	= clipMatrix[11]+clipMatrix[8];
	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE].d			= clipMatrix[15]+clipMatrix[12];
	
	//bottom plane
	frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.x = clipMatrix[3]+clipMatrix[1];
	frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.y = clipMatrix[7]+clipMatrix[5];
	frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.z = clipMatrix[11]+clipMatrix[9];
	frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].d		 = clipMatrix[15]+clipMatrix[13];
	
	//top plane
	frustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.x	= clipMatrix[3]-clipMatrix[1];
	frustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.y	= clipMatrix[7]-clipMatrix[5];
	frustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.z	= clipMatrix[11]-clipMatrix[9];
	frustum.m_cPlanes[gxFrustumf::TOP_PLANE].d			= clipMatrix[15]-clipMatrix[13];
	
	//back plane
	frustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.x	= clipMatrix[3]-clipMatrix[2];
	frustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.y	= clipMatrix[7]-clipMatrix[6];
	frustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.z	= clipMatrix[11]-clipMatrix[10];
	frustum.m_cPlanes[gxFrustumf::NEAR_PLANE].d			= clipMatrix[15]-clipMatrix[14];
	
	//front plane
	frustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.x	= clipMatrix[3]+clipMatrix[2];
	frustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.y	= clipMatrix[7]+clipMatrix[6];
	frustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.z	= clipMatrix[11]+clipMatrix[10];
	frustum.m_cPlanes[gxFrustumf::FAR_PLANE].d			= clipMatrix[15]+clipMatrix[14];
	
	
	for (int i=0;i<6;i++)
		frustum.m_cPlanes[i].normalize();
	
	//Far Clipping Plane Vertices
	frustum.frustumVertices[4]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::FAR_PLANE], frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	frustum.frustumVertices[5]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::FAR_PLANE], frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	frustum.frustumVertices[6]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::FAR_PLANE], frustum.m_cPlanes[gxFrustumf::TOP_PLANE],		frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	frustum.frustumVertices[7]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::FAR_PLANE], frustum.m_cPlanes[gxFrustumf::TOP_PLANE],		frustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	
	//Near Clipping Plane Vertices
	frustum.frustumVertices[0]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::NEAR_PLANE], frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	frustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	frustum.frustumVertices[1]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::NEAR_PLANE], frustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	frustum.frustumVertices[2]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::NEAR_PLANE], frustum.m_cPlanes[gxFrustumf::TOP_PLANE],		frustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	frustum.frustumVertices[3]	= gxPlane3f::intersectionPoint(frustum.m_cPlanes[gxFrustumf::NEAR_PLANE], frustum.m_cPlanes[gxFrustumf::TOP_PLANE],		frustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
}
