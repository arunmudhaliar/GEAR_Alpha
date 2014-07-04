//
#ifndef GXFRUSTUM_H
#define GXFRUSTUM_H

#include "plane.h"

template<class T> class frustum;
typedef DECLSPEC frustum<float> gxFrustumf;

template<class T>
class DECLSPEC frustum
{
public:
	enum EFRUSTUM_PLANES
	{
		LEFT_PLANE=0,
		RIGHT_PLANE,
		TOP_PLANE,
		BOTTOM_PLANE,
		NEAR_PLANE,
		FAR_PLANE
	};
	
	frustum<T>()
	{
	};
	
	frustum<T>(frustum<T>& _frustum)
	{
		int i=0;
		for(i=0;i<6;i++)
		{
			m_cPlanes[i].normal		= _frustum.m_cPlanes[i].normal;
			m_cPlanes[i].d			= _frustum.m_cPlanes[i].d;
		}
		
		for(i=0;i<8;i++)
			m_cFrustumVert[i]	= _frustum.m_cFrustumVert[i];
	};
	
	bool isPointInside(vector3<T> aPoint)
	{
		for(int i=0; i<6; ++i)
		{
			if(m_cPlanes[i].classifyPoint(aPoint)==plane3<T>::NEGATIVE)
				return false;
		}
		
		return true;
	}

	int isAABBInside(const AABB<T>& aabb)
	{
		int result =1;	//completely inside

		for(int i=0;i<6;i++)
		{
			plane3<T>& plane = m_cPlanes[i];
			vector3<T>& nrml=plane.normal;

			vector3<T> pv(
				nrml.x>0 ? aabb.m_max.x : aabb.m_min.x,
				nrml.y>0 ? aabb.m_max.y : aabb.m_min.y,
				nrml.z>0 ? aabb.m_max.z : aabb.m_min.z
				);

			vector3<T> nv(
				nrml.x<0 ? aabb.m_max.x : aabb.m_min.x,
				nrml.y<0 ? aabb.m_max.y : aabb.m_min.y,
				nrml.z<0 ? aabb.m_max.z : aabb.m_min.z
				);

			float n = nrml.dot(pv)+plane.d;
			if(n<0)	return 0;	//completely outside

			float m = nrml.dot(nv)+plane.d;
			if(m<0) result=2;	//intersects
		}

		return result;
	}

	void extractFrustumPlanes(matrix4x4f& clipMatrix)
	{
		//right plane
		m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.x	= clipMatrix[3]-clipMatrix[0];
		m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.y	= clipMatrix[7]-clipMatrix[4];
		m_cPlanes[gxFrustumf::RIGHT_PLANE].normal.z	= clipMatrix[11]-clipMatrix[8];
		m_cPlanes[gxFrustumf::RIGHT_PLANE].d			= clipMatrix[15]-clipMatrix[12];
	
		//left plane
		m_cPlanes[gxFrustumf::LEFT_PLANE].normal.x	= clipMatrix[3]+clipMatrix[0];
		m_cPlanes[gxFrustumf::LEFT_PLANE].normal.y	= clipMatrix[7]+clipMatrix[4];
		m_cPlanes[gxFrustumf::LEFT_PLANE].normal.z	= clipMatrix[11]+clipMatrix[8];
		m_cPlanes[gxFrustumf::LEFT_PLANE].d			= clipMatrix[15]+clipMatrix[12];
	
		//bottom plane
		m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.x = clipMatrix[3]+clipMatrix[1];
		m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.y = clipMatrix[7]+clipMatrix[5];
		m_cPlanes[gxFrustumf::BOTTOM_PLANE].normal.z = clipMatrix[11]+clipMatrix[9];
		m_cPlanes[gxFrustumf::BOTTOM_PLANE].d		 = clipMatrix[15]+clipMatrix[13];
	
		//top plane
		m_cPlanes[gxFrustumf::TOP_PLANE].normal.x	= clipMatrix[3]-clipMatrix[1];
		m_cPlanes[gxFrustumf::TOP_PLANE].normal.y	= clipMatrix[7]-clipMatrix[5];
		m_cPlanes[gxFrustumf::TOP_PLANE].normal.z	= clipMatrix[11]-clipMatrix[9];
		m_cPlanes[gxFrustumf::TOP_PLANE].d			= clipMatrix[15]-clipMatrix[13];
	
		//back plane
		m_cPlanes[gxFrustumf::NEAR_PLANE].normal.x	= clipMatrix[3]-clipMatrix[2];
		m_cPlanes[gxFrustumf::NEAR_PLANE].normal.y	= clipMatrix[7]-clipMatrix[6];
		m_cPlanes[gxFrustumf::NEAR_PLANE].normal.z	= clipMatrix[11]-clipMatrix[10];
		m_cPlanes[gxFrustumf::NEAR_PLANE].d			= clipMatrix[15]-clipMatrix[14];
	
		//front plane
		m_cPlanes[gxFrustumf::FAR_PLANE].normal.x	= clipMatrix[3]+clipMatrix[2];
		m_cPlanes[gxFrustumf::FAR_PLANE].normal.y	= clipMatrix[7]+clipMatrix[6];
		m_cPlanes[gxFrustumf::FAR_PLANE].normal.z	= clipMatrix[11]+clipMatrix[10];
		m_cPlanes[gxFrustumf::FAR_PLANE].d			= clipMatrix[15]+clipMatrix[14];
	
	
		for (int i=0;i<6;i++)
			m_cPlanes[i].normalize();
	
		//Far Clipping Plane Vertices
		m_cFrustumVert[4]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::FAR_PLANE], m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cPlanes[gxFrustumf::LEFT_PLANE]);
		m_cFrustumVert[5]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::FAR_PLANE], m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cPlanes[gxFrustumf::RIGHT_PLANE]);
		m_cFrustumVert[6]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::FAR_PLANE], m_cPlanes[gxFrustumf::TOP_PLANE],		m_cPlanes[gxFrustumf::RIGHT_PLANE]);
		m_cFrustumVert[7]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::FAR_PLANE], m_cPlanes[gxFrustumf::TOP_PLANE],		m_cPlanes[gxFrustumf::LEFT_PLANE]);
	
		//Near Clipping Plane Vertices
		m_cFrustumVert[0]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::NEAR_PLANE], m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cPlanes[gxFrustumf::LEFT_PLANE]);
		m_cFrustumVert[1]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::NEAR_PLANE], m_cPlanes[gxFrustumf::BOTTOM_PLANE],	m_cPlanes[gxFrustumf::RIGHT_PLANE]);
		m_cFrustumVert[2]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::NEAR_PLANE], m_cPlanes[gxFrustumf::TOP_PLANE],		m_cPlanes[gxFrustumf::RIGHT_PLANE]);
		m_cFrustumVert[3]	= gxPlane3f::intersectionPoint(m_cPlanes[gxFrustumf::NEAR_PLANE], m_cPlanes[gxFrustumf::TOP_PLANE],		m_cPlanes[gxFrustumf::LEFT_PLANE]);
	}
//	//is a AABB in the frustum?
//	PXbool IsAABBInside(const AABB<T>& aAABB)
//	{
//		//loop through planes
//		for(PXint i=0; i<6; ++i)
//		{
//			//if a point is not behind this plane, try next plane
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_0())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_1())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_2())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_3())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_4())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_5())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_6())!=Plane3<T>::NEGATIVE)
//				continue;
//			if(m_cPlanes[i].ClassifyPoint(aAABB.GetPoint_7())!=Plane3<T>::NEGATIVE)
//				continue;
//			
//			//All vertices of the box are behind this plane
//			return PX_FALSE;
//		}
//		
//		return PX_TRUE;
//	}
	
	~frustum<T>()
	{
	}
	
	//frustum Planes
	plane3<T>	m_cPlanes[6];
	vector3<T>	m_cFrustumVert[8];
};
#endif
