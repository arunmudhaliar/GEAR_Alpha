#include "Camera.h"

Camera::Camera():
	object3d(OBJECT3D_CAMERA)
{
	m_pRendererPtr=NULL;
	//m_pCameraStructPtr=NULL;
	m_iLayerCullingMask=0xffffffff;
	setName("Camera");
	setMainCamera(false);
}

Camera::~Camera()
{
	resetCamera();
}

void Camera::resetCamera()
{
	m_pRendererPtr=NULL;
	m_cProjMatrix.identity();
	m_cInvTranfMatrix.identity();
}

void Camera::initCamera(gxRenderer* renderer)
{
	resetCamera();
	m_pRendererPtr=renderer;

	setFOV(45.0f);
	setNear(10.0f);
	setFar(10000.0f);
	setType(PERSPECTIVE_PROJECTION);
	updateLocalPositionf(0, 0, 300);
}

bool Camera::isLayerCullingMask(unsigned int flag)
{
	return (m_iLayerCullingMask&flag)?true:false;
}
	
void Camera::resetLayerCullingMask(unsigned int flag)
{
	m_iLayerCullingMask=m_iLayerCullingMask&~flag;
}

void Camera::setLayerCullingMask(unsigned int flag)
{
	m_iLayerCullingMask|=flag;
}

unsigned int Camera::getLayerCullingMask()
{
	return m_iLayerCullingMask;
}

void Camera::setFOV(float fov)
{
	m_fFOV=fov;
	perspectiveChanged();
}

void Camera::setNear(float n)
{
	m_fNear=n;
	perspectiveChanged();
}

void Camera::setFar(float f)
{
	m_fFar=f;
	perspectiveChanged();
}

void Camera::setType(EPROJECTION_TYPE type)
{
	m_eProjectionType=type;
	perspectiveChanged();
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

void Camera::perspectiveChanged()
{
	if(getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		m_cProjMatrix.setPerspective(m_fFOV, 1.0f, m_fNear, m_fFar);
	else
	{
		gxRectf viewportRect(m_pRendererPtr->getViewPortRect());
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		m_cProjMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, m_fNear, m_fFar);
	}
	m_pRendererPtr->setProjectionMatrixToGL(&m_cProjMatrix);
	extractFrustumPlanes();
	updateCamera();
}

void Camera::setUpCameraPerspective(float cx, float cy/*, float fov, float nearValue, float farValue*/)
{
	if(cx==0.0f || cy==0.0f)	return;

	float aspect=cx/cy;
	if(getProjectionType()==gxCamera::PERSPECTIVE_PROJECTION)
		m_cProjMatrix.setPerspective(m_fFOV, aspect, m_fNear, m_fFar);
	else
	{
		gxRectf viewportRect(m_pRendererPtr->getViewPortRect());
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		m_cProjMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, m_fNear, m_fFar);
	}
	
	m_pRendererPtr->setProjectionMatrixToGL(&m_cProjMatrix);
	extractFrustumPlanes();
	updateCamera();
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
	matrix4x4f pvm(m_cProjMatrix * m_cInvTranfMatrix);
	m_cFrustum.extractFrustumPlanes(pvm);
	m_cRenderFrustum.extractFrustumPlanes(m_cProjMatrix);
	calculateAABB();
}

void Camera::calculateAABB()
{
    object3d::calculateAABB();

	vector3f	min_v(1e16f, 1e16f, 1e16f);
	vector3f	max_v(-1e16f, -1e16f, -1e16f);
	for(int i=0;i<8;i++)
	{
		vector3f transformedFrustumPoint(*this->getWorldMatrix() * m_cRenderFrustum.m_cFrustumVert[i]);
		if(transformedFrustumPoint.x<min_v.x)
			min_v.x=transformedFrustumPoint.x;
		if(transformedFrustumPoint.y<min_v.y)
			min_v.y=transformedFrustumPoint.y;
		if(transformedFrustumPoint.z<min_v.z)
			min_v.z=transformedFrustumPoint.z;
			
		if(transformedFrustumPoint.x>max_v.x)
			max_v.x=transformedFrustumPoint.x;
		if(transformedFrustumPoint.y>max_v.y)
			max_v.y=transformedFrustumPoint.y;
		if(transformedFrustumPoint.z>max_v.z)
			max_v.z=transformedFrustumPoint.z;
	}//for
	
	m_cAABB.set(min_v, max_v);
}

void Camera::drawFrustum(gxHWShader* shader)
{
	vector3f* frustumVerts = m_cRenderFrustum.m_cFrustumVert;

	float lineAry[]={
		frustumVerts[0].x, frustumVerts[0].y, frustumVerts[0].z,		//0	near
		frustumVerts[1].x, frustumVerts[1].y, frustumVerts[1].z,		//1
		frustumVerts[2].x, frustumVerts[2].y, frustumVerts[2].z,		//2
		frustumVerts[3].x, frustumVerts[3].y, frustumVerts[3].z,		//3

		frustumVerts[4].x, frustumVerts[4].y, frustumVerts[4].z,		//4	far
		frustumVerts[5].x, frustumVerts[5].y, frustumVerts[5].z,		//5
		frustumVerts[6].x, frustumVerts[6].y, frustumVerts[6].z,		//6
		frustumVerts[7].x, frustumVerts[7].y, frustumVerts[7].z,		//7

		frustumVerts[0].x, frustumVerts[0].y, frustumVerts[0].z,		//0	near
		frustumVerts[4].x, frustumVerts[4].y, frustumVerts[4].z,		//4	far

		frustumVerts[1].x, frustumVerts[1].y, frustumVerts[1].z,		//1
		frustumVerts[5].x, frustumVerts[5].y, frustumVerts[5].z,		//5

		frustumVerts[2].x, frustumVerts[2].y, frustumVerts[2].z,		//2
		frustumVerts[6].x, frustumVerts[6].y, frustumVerts[6].z,		//6

		frustumVerts[3].x, frustumVerts[3].y, frustumVerts[3].z,		//3
		frustumVerts[7].x, frustumVerts[7].y, frustumVerts[7].z,		//7
	};

	unsigned int mode=GL_LINE_LOOP;

#if defined (USE_ProgrammablePipeLine)
        glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		//near
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, lineAry);
		glDrawArrays(mode, 0, 4);

		//far
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[12]);
		glDrawArrays(mode, 0, 4);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*0]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*1]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*2]);
		glDrawArrays(mode, 0, 2);

		//side lines
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, &lineAry[24+6*3]);
		glDrawArrays(mode, 0, 2);

		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
#endif
}

void Camera::write(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Write(m_iAssetFileCRC);
	writeAnimationController(file);

	//write camera data
	file.Write(m_eProjectionType);
	file.Write(m_bMainCamera);
	file.Write(m_fFOV);
	file.Write(m_fNear);
	file.Write(m_fFar);
	file.Write(m_iLayerCullingMask);
	//

	file.Write((int)m_cChilds.size());
#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=m_cChilds.getHead();
    while(node)
    {
		object3d* obj=node->getData();
		obj->write(file);
        node=node->getNext();
	}
#else
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
#endif
}

void Camera::read(gxFile& file)
{
	file.Read(m_eBaseFlags);
	char* temp=file.ReadString();
	GX_STRCPY(m_cszName, temp);
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Read(m_iAssetFileCRC);
	readAnimationController(file);

	//read camera data
	int etype=0;
	file.Read(etype);
	m_eProjectionType=(EPROJECTION_TYPE)etype;
	file.Read(m_bMainCamera);
	file.Read(m_fFOV);
	file.Read(m_fNear);
	file.Read(m_fFar);
	file.Read(m_iLayerCullingMask);
	//

	perspectiveChanged();
}