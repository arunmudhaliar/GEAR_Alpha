#include "gxCamera.h"

gxCamera::gxCamera():
	object3d(OBJECT3D_CAMERA)
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
	m_FOV=fov;
	perspectiveChanged();
}

void gxCamera::setNear(float n)
{
	m_fNear=n;
	perspectiveChanged();
}

void gxCamera::setFar(float f)
{
	m_fFar=f;
	perspectiveChanged();
}

void gxCamera::setType(EPROJECTION_TYPE type)
{
	m_eProjectionType=type;
	perspectiveChanged();
}

void gxCamera::perspectiveChanged()
{
	if(getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		m_cDummyProjMatrix.setPerspective(m_FOV, 1.0f, m_fNear, m_fFar);
	else
	{
		gxRectf viewportRect(0, 0, 200, 200);
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		m_cDummyProjMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, m_fNear, m_fFar);
	}

	extractFrustumPlanes();
}

void gxCamera::extractFrustumPlanes()
{
	matrix4x4f clipMatrix;
	
	clipMatrix	= m_cDummyProjMatrix; //* this->getInverse();
	//right plane
	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.x	= clipMatrix[3]-clipMatrix[0];
	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.y	= clipMatrix[7]-clipMatrix[4];
	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.z	= clipMatrix[11]-clipMatrix[8];
	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE].d			= clipMatrix[15]-clipMatrix[12];
	
	//left plane
	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.x	= clipMatrix[3]+clipMatrix[0];
	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.y	= clipMatrix[7]+clipMatrix[4];
	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE].normal.z	= clipMatrix[11]+clipMatrix[8];
	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE].d			= clipMatrix[15]+clipMatrix[12];
	
	//bottom plane
	m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.x = clipMatrix[3]+clipMatrix[1];
	m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.y = clipMatrix[7]+clipMatrix[5];
	m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.z = clipMatrix[11]+clipMatrix[9];
	m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE].d		 = clipMatrix[15]+clipMatrix[13];
	
	//top plane
	m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.x	= clipMatrix[3]-clipMatrix[1];
	m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.y	= clipMatrix[7]-clipMatrix[5];
	m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE].normal.z	= clipMatrix[11]-clipMatrix[9];
	m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE].d			= clipMatrix[15]-clipMatrix[13];
	
	//back plane
	m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.x	= clipMatrix[3]-clipMatrix[2];
	m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.y	= clipMatrix[7]-clipMatrix[6];
	m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE].normal.z	= clipMatrix[11]-clipMatrix[10];
	m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE].d			= clipMatrix[15]-clipMatrix[14];
	
	//front plane
	m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.x	= clipMatrix[3]+clipMatrix[2];
	m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.y	= clipMatrix[7]+clipMatrix[6];
	m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE].normal.z	= clipMatrix[11]+clipMatrix[10];
	m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE].d			= clipMatrix[15]+clipMatrix[14];
	
	
	for (int i=0;i<6;i++)
		m_cFrustum.m_cPlanes[i].normalize();
	
	//Far Clipping Plane Vertices
	m_cFrustum.m_cFrustumVert[4]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	m_cFrustum.m_cFrustumVert[5]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	m_cFrustum.m_cFrustumVert[6]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE],		m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	m_cFrustum.m_cFrustumVert[7]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::FAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE],		m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	
	//Near Clipping Plane Vertices
	m_cFrustum.m_cFrustumVert[0]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
	m_cFrustum.m_cFrustumVert[1]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	m_cFrustum.m_cFrustumVert[2]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE],		m_cFrustum.m_cPlanes[gxFrustumf::RIGHT_PLANE]);
	m_cFrustum.m_cFrustumVert[3]	= gxPlane3f::intersectionPoint(m_cFrustum.m_cPlanes[gxFrustumf::NEAR_PLANE], m_cFrustum.m_cPlanes[gxFrustumf::TOP_PLANE],		m_cFrustum.m_cPlanes[gxFrustumf::LEFT_PLANE]);
}
