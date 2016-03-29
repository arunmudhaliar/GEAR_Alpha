#include "Camera.h"

Camera* Camera::create()
{
    auto newObject = new Camera();
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

Camera::Camera():
	object3d(OBJECT3D_CAMERA)
{
	renderer=NULL;
	//m_pCameraStructPtr=NULL;
	layerCullingMask=0xffffffff;
	setName("Camera");
	setMainCamera(false);
}

Camera::~Camera()
{
	resetCamera();
}

void Camera::resetCamera()
{
	renderer=NULL;
	projectionMatrix.identity();
	inverseTransformationMatrix.identity();
}

void Camera::initCamera(gxRenderer* renderer)
{
	resetCamera();
	this->renderer=renderer;

	setFOV(45.0f);
	setNear(10.0f);
	setFar(10000.0f);
	setType(PERSPECTIVE_PROJECTION);
	updateLocalPositionf(0, 0, 300);
}

bool Camera::isLayerCullingMask(unsigned int flag)
{
	return (layerCullingMask&flag)?true:false;
}
	
void Camera::resetLayerCullingMask(unsigned int flag)
{
	layerCullingMask=layerCullingMask&~flag;
}

void Camera::setLayerCullingMask(unsigned int flag)
{
	layerCullingMask|=flag;
}

unsigned int Camera::getLayerCullingMask()
{
	return layerCullingMask;
}

void Camera::setFOV(float fov)
{
	this->fov = fov;
	perspectiveChanged();
}

void Camera::setNear(float n)
{
	nearValue = n;
	perspectiveChanged();
}

void Camera::setFar(float f)
{
	farValue = f;
	perspectiveChanged();
}

void Camera::setType(EPROJECTION_TYPE type)
{
	projectionType=type;
	perspectiveChanged();
}

void Camera::processCamera(matrix4x4f* matrix)
{
	if(matrix)
		renderer->setViewMatrixToGL(matrix);
	else
		renderer->setViewMatrixToGL(&inverseTransformationMatrix);

	renderer->setViewProjectionMatrix(&viewProjectionMatrix);
}

void Camera::updateCamera()
{
	renderer->setMainCameraEye(getWorldMatrix()->getPosition());
	inverseTransformationMatrix = getWorldMatrix()->getInverse();
	viewProjectionMatrix = projectionMatrix * inverseTransformationMatrix;
}

void Camera::perspectiveChanged()
{
	if(getProjectionType()==(int)PERSPECTIVE_PROJECTION)
		projectionMatrix.setPerspective(fov, 1.0f, nearValue, farValue);
	else
	{
		gxRectf viewportRect(renderer->getViewPortRect());
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		projectionMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, nearValue, farValue);
	}
	renderer->setProjectionMatrixToGL(&projectionMatrix);
	extractFrustumPlanes();
	updateCamera();
}

void Camera::setUpCameraPerspective(float cx, float cy/*, float fov, float nearValue, float farValue*/)
{
	if(cx==0.0f || cy==0.0f)	return;

	float aspect=cx/cy;
	if(getProjectionType()==(int)Camera::PERSPECTIVE_PROJECTION)
		projectionMatrix.setPerspective(fov, aspect, nearValue, farValue);
	else
	{
		gxRectf viewportRect(renderer->getViewPortRect());
		vector2f centerAlignedPos(viewportRect.m_pos-viewportRect.m_size*0.5f);
		projectionMatrix.setOrtho(centerAlignedPos.x, centerAlignedPos.x+viewportRect.m_size.x, centerAlignedPos.y, centerAlignedPos.y+viewportRect.m_size.y, nearValue, farValue);
	}
	
	renderer->setProjectionMatrixToGL(&projectionMatrix);
	extractFrustumPlanes();
	updateCamera();
}

vector3f Camera::getCameraSpaceLoc(const vector3f& point)
{
	return inverseTransformationMatrix * point;
}

void Camera::transformationChangedf()
{
    object3d::transformationChangedf();
    
	updateCamera();
}

void Camera::extractFrustumPlanes()
{
	matrix4x4f pvm(projectionMatrix * inverseTransformationMatrix);
	frustum.extractFrustumPlanes(pvm);
	renderFrustum.extractFrustumPlanes(projectionMatrix);
	calculateAABB();
}

void Camera::calculateAABB()
{
    object3d::calculateAABB();

	vector3f	min_v(1e16f, 1e16f, 1e16f);
	vector3f	max_v(-1e16f, -1e16f, -1e16f);
	for(int i=0;i<8;i++)
	{
		vector3f transformedFrustumPoint(*this->getWorldMatrix() * renderFrustum.frustumVertices[i]);
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
	
	aabb.set(min_v, max_v);
}

void Camera::drawFrustum(gxHWShader* shader)
{
	vector3f* frustumVerts = renderFrustum.frustumVertices;

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

void Camera::writeData(gxFile& file)
{
	//write camera data
	file.Write(projectionType);
	file.Write(isMainCam);
	file.Write(fov);
	file.Write(nearValue);
	file.Write(farValue);
	file.Write(layerCullingMask);
	//
}

void Camera::readData(gxFile& file)
{
	//read camera data
	int etype=0;
	file.Read(etype);
	projectionType=(EPROJECTION_TYPE)etype;
	file.Read(isMainCam);
	file.Read(fov);
	file.Read(nearValue);
	file.Read(farValue);
	file.Read(layerCullingMask);
	//

	perspectiveChanged();
}