#include "gxWorld.h"
#include "../fbxImporter/fbxImporter.h"
#include "../util/gxUtil.h"

gxWorld::gxWorld():
	object3d(999)
{
	m_pObserverPtr = NULL;
	m_pActiveCameraPtr = NULL;

	m_pActiveCameraPtr=NULL;
	setDefaultCameraActive();
	m_pActiveCameraPtr->initCamera(&m_cRenderer);

	vector3f v(1, -1, -1);
	m_pActiveCameraPtr->setDirection(&v);

	m_pActiveCameraPtr->updateLocalPositionf(0, 0, 40);

	m_cDefaultMaterial.setMaterialName("Default");
	m_cMaterialList.push_back(&m_cDefaultMaterial);
}

gxWorld::~gxWorld()
{
	for(std::vector<gxMaterial*>::iterator it = m_cMaterialList.begin(); it != m_cMaterialList.end(); ++it)
	{
		gxMaterial* material = *it;
		if(material==&m_cDefaultMaterial)
			continue;
		GX_DELETE(material);
	}
	m_cMaterialList.clear();

	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSetList.begin(); it != m_vAnimationSetList.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		GX_DELETE(animationSet);
	}
	m_vAnimationSetList.clear();
}

void gxWorld::update(float dt)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->updateCamera();
	}

	object3d::update(dt);
}

void gxWorld::render()
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	if(m_pObserverPtr)m_pObserverPtr->preWorldRender();
	object3d::render();
	if(m_pObserverPtr)m_pObserverPtr->postWorldRender();
}

void gxWorld::renderSingleObject(object3d* obj)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	obj->render();
}

void gxWorld::resizeWorld(float x, float y, float cx, float cy)
{
	m_cRenderer.setViewPort(x, y, cx, cy);
	if(m_pActiveCameraPtr)
	{	
		m_pActiveCameraPtr->setUpCameraPerspective(cx, cy, 45.0f, 10.0f, 100000.0f);
	}
}

void gxWorld::loadTextures(object3d* obj, const char* fbxFileName)
{
	if(obj->getID()==100)
	{
		const char* directorPathPtr=gxUtil::getFolderPathFromFileName(fbxFileName);

		gxMesh* mesh= (gxMesh*)obj;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triInfo=mesh->getTriInfo(x);
			if(triInfo->getMaterial())
				triInfo->getMaterial()->loadTextureFromDirectory(m_cTextureManager, directorPathPtr);
		}
	}

	std::vector<object3d*>* list=obj->getChildList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* childobj = *it;
		loadTextures(childobj, fbxFileName);
	}
}

Camera* gxWorld::setDefaultCameraActive()
{
	m_pActiveCameraPtr=&m_cDefaultCamera;

	return m_pActiveCameraPtr;
}

void gxWorld::calculateAABB()
{
	object3d::calculateAABB();
}
