#include "Camera.h"

Camera::Camera():
	object3d(400)
{
	//m_pCurrentCamPtr=NULL;
	m_pRendererPtr=NULL;
	m_pCameraStructPtr=NULL;
}

Camera::~Camera()
{
	resetCamera();
}

void Camera::resetCamera()
{
	//m_pCurrentCamPtr=NULL;
	m_pRendererPtr=NULL;
	m_cProjMatrix.identity();
	m_cInvTranfMatrix.identity();
}

void Camera::initCamera(gxRenderer* renderer)
{
	resetCamera();

	m_pRendererPtr=renderer;
}

void Camera::processCamera(matrix4x4f* matrix)
{
	if(matrix)
		m_pRendererPtr->setViewMatrixToGL(matrix);
	else
		m_pRendererPtr->setViewMatrixToGL(&m_cInvTranfMatrix);

	m_pRendererPtr->setViewProjectionMatrix(&m_cViewProjectionMatrix);
}

void Camera::updateCamera()
{
	m_pRendererPtr->setMainCameraEye(getWorldMatrix()->getPosition());
	m_cInvTranfMatrix = getWorldMatrix()->getInverse();
	m_cViewProjectionMatrix = m_cProjMatrix * m_cInvTranfMatrix;
}

void Camera::setUpCameraPerspective(float cx, float cy/*, float fov, float nearValue, float farValue*/)
{
	if(cx==0.0f || cy==0.0f)	return;

	float aspect=cx/cy;
	m_fFOV=m_pCameraStructPtr->getFOV();
	m_fNear=m_pCameraStructPtr->getNear();
	m_fFar=m_pCameraStructPtr->getFar();

	if(m_pCameraStructPtr->getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		m_cProjMatrix.setPerspective(m_fFOV, aspect, m_fNear, m_fFar);
	else
		m_cProjMatrix.setOrtho(m_pRendererPtr->getViewPortRect().m_pos.x, m_pRendererPtr->getViewPortSz().x, m_pRendererPtr->getViewPortRect().m_pos.y, m_pRendererPtr->getViewPortSz().y, m_fNear, m_fFar);
	
	m_pRendererPtr->setProjectionMatrixToGL(&m_cProjMatrix);
	updateCamera();
}


void Camera::setCamera(gxCamera* camera)
{
	if(!camera) return;

	m_pCameraStructPtr = camera;
	copy((matrix4x4f)*camera);

	setUpCameraPerspective(m_pRendererPtr->getViewPortSz().x, m_pRendererPtr->getViewPortSz().y/*, camera->getFOV(), camera->getNear(), camera->getFar()*/);
}


vector3f Camera::getCameraSpaceLoc(const vector3f& point)
{
	return m_cInvTranfMatrix * point;
}

void Camera::transformationChangedf()
{
    object3d::transformationChangedf();
    
	updateCamera();
}

void Camera::extractFrustumPlanes()
{
	/*
	matrix4x4f clipMatrix;
	
	clipMatrix	= m_cProjMatrix * m_cInvTranfMatrix;
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
	
	calculateAABB();
	*/
}

void Camera::calculateAABB()
{
	/*
    objectBase::calculateAABB();

	vector3f	min;
	vector3f	max;
	for(int i=0;i<8;i++)
	{
		if(i==0)
		{
			min	= m_cFrustum.m_cFrustumVert[i];
			max	= m_cFrustum.m_cFrustumVert[i];
		}
		else
		{
			if(m_cFrustum.m_cFrustumVert[i].x<min.x)
				min.x=m_cFrustum.m_cFrustumVert[i].x;
			if(m_cFrustum.m_cFrustumVert[i].y<min.y)
				min.y=m_cFrustum.m_cFrustumVert[i].y;
			if(m_cFrustum.m_cFrustumVert[i].z<min.z)
				min.z=m_cFrustum.m_cFrustumVert[i].z;
			
			if(m_cFrustum.m_cFrustumVert[i].x>max.x)
				max.x=m_cFrustum.m_cFrustumVert[i].x;
			if(m_cFrustum.m_cFrustumVert[i].y>max.y)
				max.y=m_cFrustum.m_cFrustumVert[i].y;
			if(m_cFrustum.m_cFrustumVert[i].z>max.z)
				max.z=m_cFrustum.m_cFrustumVert[i].z;
		}
	}//for
	
	m_cAABB.set(min, max);
	*/

	object3d::calculateAABB();
}

void Camera::drawFrustum()
{
	/*
#if defined (USE_ProgrammablePipeLine)
    
#else
	float lineAry[]={
		m_cFrustum.m_cFrustumVert[0].x, m_cFrustum.m_cFrustumVert[0].y, m_cFrustum.m_cFrustumVert[0].z,		//0	near
		m_cFrustum.m_cFrustumVert[1].x, m_cFrustum.m_cFrustumVert[1].y, m_cFrustum.m_cFrustumVert[1].z,		//1
		m_cFrustum.m_cFrustumVert[2].x, m_cFrustum.m_cFrustumVert[2].y, m_cFrustum.m_cFrustumVert[2].z,		//2
		m_cFrustum.m_cFrustumVert[3].x, m_cFrustum.m_cFrustumVert[3].y, m_cFrustum.m_cFrustumVert[3].z,		//3

		m_cFrustum.m_cFrustumVert[4].x, m_cFrustum.m_cFrustumVert[4].y, m_cFrustum.m_cFrustumVert[4].z,		//4	far
		m_cFrustum.m_cFrustumVert[5].x, m_cFrustum.m_cFrustumVert[5].y, m_cFrustum.m_cFrustumVert[5].z,		//5
		m_cFrustum.m_cFrustumVert[6].x, m_cFrustum.m_cFrustumVert[6].y, m_cFrustum.m_cFrustumVert[6].z,		//6
		m_cFrustum.m_cFrustumVert[7].x, m_cFrustum.m_cFrustumVert[7].y, m_cFrustum.m_cFrustumVert[7].z,		//7

		m_cFrustum.m_cFrustumVert[0].x, m_cFrustum.m_cFrustumVert[0].y, m_cFrustum.m_cFrustumVert[0].z,		//0	near
		m_cFrustum.m_cFrustumVert[4].x, m_cFrustum.m_cFrustumVert[4].y, m_cFrustum.m_cFrustumVert[4].z,		//4	far

		m_cFrustum.m_cFrustumVert[1].x, m_cFrustum.m_cFrustumVert[1].y, m_cFrustum.m_cFrustumVert[1].z,		//1
		m_cFrustum.m_cFrustumVert[5].x, m_cFrustum.m_cFrustumVert[5].y, m_cFrustum.m_cFrustumVert[5].z,		//5

		m_cFrustum.m_cFrustumVert[2].x, m_cFrustum.m_cFrustumVert[2].y, m_cFrustum.m_cFrustumVert[2].z,		//2
		m_cFrustum.m_cFrustumVert[6].x, m_cFrustum.m_cFrustumVert[6].y, m_cFrustum.m_cFrustumVert[6].z,		//6

		m_cFrustum.m_cFrustumVert[3].x, m_cFrustum.m_cFrustumVert[3].y, m_cFrustum.m_cFrustumVert[3].z,		//3
		m_cFrustum.m_cFrustumVert[7].x, m_cFrustum.m_cFrustumVert[7].y, m_cFrustum.m_cFrustumVert[7].z,		//7
	};

	glColor4f(0.721f, 0.874f, 0.96f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	//near
	glVertexPointer(3, GL_FLOAT, 0, lineAry);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//far
	glVertexPointer(3, GL_FLOAT, 0, &lineAry[12]);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	//side lines
	glVertexPointer(3, GL_FLOAT, 0, &lineAry[24+6*0]);
	glDrawArrays(GL_LINE_LOOP, 0, 2);

	//side lines
	glVertexPointer(3, GL_FLOAT, 0, &lineAry[24+6*1]);
	glDrawArrays(GL_LINE_LOOP, 0, 2);

	//side lines
	glVertexPointer(3, GL_FLOAT, 0, &lineAry[24+6*2]);
	glDrawArrays(GL_LINE_LOOP, 0, 2);

	//side lines
	glVertexPointer(3, GL_FLOAT, 0, &lineAry[24+6*3]);
	glDrawArrays(GL_LINE_LOOP, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
#endif
	*/
}
