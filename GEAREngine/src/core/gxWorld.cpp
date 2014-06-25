#include "gxWorld.h"
//#ifdef _WIN32
//#include "../fbxImporter/fbxImporter.h"
//#endif
#include "../util/gxUtil.h"
#include "../core/gxMetaStructures.h"
#include "../hwShader/HWShaderManager.h"
#include "../GEAREngine.h"
#include "gxSkinnedMesh.h"
#include "../util/gxCrc32.h"
//#include "../util/Crc32.h"
//#include "../GEAREngine.cpp"
//class Crc32;

gxWorld::gxWorld():
	object3d(OBJECT3D_WORLD)
{
	m_iLayer = ELAYER_DEFAULT;	//CAUTION: Do not use setLayer() for gxWorld()
	m_pObserverPtr = NULL;
	m_pActiveCameraPtr = NULL;

	setRootObserverOfTree(this);

	m_pActiveCameraPtr=NULL;
	createDefaultCameraAndSetActive();

	m_cDefaultMaterial.setMaterialName("Default");
	m_cMaterialList.push_back(&m_cDefaultMaterial);

	m_cTextureManager.LoadDefaultTextures();
	m_cRenderer.setGEARTexture1x1(m_cTextureManager.getGEARTexture1x1());

#ifdef USE_BULLET
	m_cPhysicsEngine.initPhysics();
#endif

	m_pOctree=NULL;
}

gxWorld::~gxWorld()
{
	resetWorld(true);

	////remove and destroy the default camera
	//if(m_pActiveCameraPtr);
	//{
	//	removeChild(m_pActiveCameraPtr);
	//	GX_DELETE(m_pActiveCameraPtr);
	//	m_vCameraList.clear();
	//}

	m_cMaterialList.clear();	//since our Default material may reside inside: check resetWorld()
}

void gxWorld::resetWorld(bool bDontCreateDefaultCamera)
{
#ifdef USE_BULLET
	m_cPhysicsEngine.clientResetScene();
#endif

	if(m_pOctree)
	{
		m_pOctree->reset();
		GX_DELETE(m_pOctree);
	}

	for(std::vector<gxMaterial*>::iterator it = m_cMaterialList.begin(); it != m_cMaterialList.end(); ++it)
	{
		gxMaterial* material = *it;
		if(material==&m_cDefaultMaterial)
			continue;
		GX_DELETE(material);
	}
	m_cMaterialList.clear();
	//repush our Default Material;
	m_cMaterialList.push_back(&m_cDefaultMaterial);	//must be removed on the world destructor
	//

	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSetList.begin(); it != m_vAnimationSetList.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		GX_DELETE(animationSet);
	}
	m_vAnimationSetList.clear();

	m_vLightList.clear();
	m_cLayerManager.clearLayers();
	m_vCameraList.clear();

	//destroy all child
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		GX_DELETE(obj);
	}
	m_cChilds.clear();
	//

	m_pActiveCameraPtr=NULL;
	if(!bDontCreateDefaultCamera)
		createDefaultCameraAndSetActive();
}

void gxWorld::update(float dt)
{
#ifdef _WIN32	//arun:special case
	//this can be removed from here since transformationchanged func will call camera update
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->updateCamera();
	}
	//
#endif

#ifdef USE_BULLET
	m_cPhysicsEngine.update(dt);
#endif

	//collide with octree
	if(m_pOctree && m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->extractFrustumPlanes();
		
		m_pOctree->resetCollidedTransformObjList();
		//m_pOctree->CheckOverlapWithOctree(m_pOctree->GetRoot(), GetCurrentCamera());
		m_pOctree->checkFrustumOverlapWithOctree(m_pOctree->getRoot(), &m_pActiveCameraPtr->getFrustum());
		//DEBUG_PRINT("octree item %d", m_pOctree->GetCollidedTransformObjList()->GetCount());
	}

	object3d::update(dt);
}

void gxWorld::renderFromOctreeList(gxRenderer* renderer, ExpandableArray<object3d*>* list)
{
	ExpandableArrayNode<object3d*>* collidedtransformObjNode=list->GetRoot();
		
	int count=list->GetCount();
	while(collidedtransformObjNode && count--)
	{
		getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);

		object3d* obj = collidedtransformObjNode->GetData();
		if(!obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
		{
			collidedtransformObjNode=collidedtransformObjNode->GetNext();
			continue;
		}

		obj->render(renderer, NULL);

		getRenderer()->setRenderPassType(gxRenderer::RENDER_LIGHTING_ONLY);
		CHECK_GL_ERROR(glEnable(GL_BLEND));
		CHECK_GL_ERROR(glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR));		//really good result	(2x Multiplicative)
		//CHECK_GL_ERROR(glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE));	//not good result	(Soft Additive)
		std::vector<gxLight*>* lightList = getLightList();
		for(int x=0;x<lightList->size();x++)
		{
			gxLight* light = lightList->at(x);
			if(!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
				continue;
				
			//Note:- glDepthFunc(GL_LEQUAL); by default its GL_LEQUAL in engine so no need to change here
			obj->render(renderer, light);
		}
		CHECK_GL_ERROR(glDisable(GL_BLEND));

		collidedtransformObjNode=collidedtransformObjNode->GetNext();
	}
}

void gxWorld::render(gxRenderer* renderer, object3d* lightPtr)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	renderer->m_nTrisRendered=0;
	renderer->m_nDrawCalls=0;

	if(m_pObserverPtr)m_pObserverPtr->preWorldRender();


	////////////////////////////////
	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	

	if(m_pOctree)
	{
		//render octree
		renderFromOctreeList(renderer,  m_pOctree->getCollidedObjList());
		renderFromOctreeList(renderer,  m_pOctree->getCollidedAlphaObjList());
	}
	else
	{
		getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
		CHECK_GL_ERROR(object3d::render(renderer, NULL));

		//glDisable(GL_DEPTH_TEST);
		//glDepthMask(GL_FALSE);
		//glDepthFunc(GL_LEQUAL);
		CHECK_GL_ERROR(glEnable(GL_BLEND));
		CHECK_GL_ERROR(glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR));		//really good result	(2x Multiplicative)
		//glBlendFunc(GL_DST_COLOR, GL_ZERO);			//good result	(Multiplicative)
		//glBlendFunc(GL_ONE, GL_ONE);					//not good result	(Additive)
		//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);	//not good result	(Soft Additive)
		getRenderer()->setRenderPassType(gxRenderer::RENDER_LIGHTING_ONLY);
		std::vector<gxLight*>* lightList = getLightList();
		for(int x=0;x<lightList->size();x++)
		{
			gxLight* light = lightList->at(x);
			if(!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
				continue;

			//Note:- glDepthFunc(GL_LEQUAL); by default its GL_LEQUAL in engine so no need to change here
			CHECK_GL_ERROR(object3d::render(renderer, light));
		}
		CHECK_GL_ERROR(glDisable(GL_BLEND));
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
	}
	////////////////////////////////

	if(m_pObserverPtr)m_pObserverPtr->postWorldRender();
}

void gxWorld::renderShadow(gxRenderer* renderer)
{
	object3d::render(renderer, NULL);
}

void gxWorld::renderSingleObject(object3d* obj, object3d* lightPtr)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
	obj->render(&m_cRenderer, NULL);

	CHECK_GL_ERROR(glEnable(GL_BLEND));
	CHECK_GL_ERROR(glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR));		//really good result	(2x Multiplicative)
	//glBlendFunc(GL_DST_COLOR, GL_ZERO);			//good result	(Multiplicative)
	//glBlendFunc(GL_ONE, GL_ONE);					//not good result	(Additive)
	//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);	//not good result	(Soft Additive)
	std::vector<gxLight*>* lightList = getLightList();
	for(int x=0;x<lightList->size();x++)
	{
		gxLight* light = lightList->at(x);
		if(!light->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
			continue;

		getRenderer()->setRenderPassType(gxRenderer::RENDER_LIGHTING_ONLY);
		//Note:- glDepthFunc(GL_LEQUAL); by default its GL_LEQUAL in engine so no need to change here
		obj->render(&m_cRenderer, light);
	}
	CHECK_GL_ERROR(glDisable(GL_BLEND));
}

void gxWorld::resizeWorld(float x, float y, float cx, float cy, float nearplane, float farplane)
{
	m_cRenderer.setViewPort(x, y, cx, cy);
	if(m_pActiveCameraPtr)
	{	
		m_pActiveCameraPtr->setUpCameraPerspective(cx, cy/*, 45.0f, nearplane, farplane*/);
	}
}

#if 0
void gxWorld::loadTextures(object3d* obj, const char* fbxFileName)
{
	if(obj->getID()==OBJECT3D_MESH || obj->getID()==OBJECT3D_SKINNED_MESH)
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
#endif

void gxWorld::createOctree(int minTransformObj, int maxLevel)
{
	if(m_pOctree)
	{
		m_pOctree->reset();
		GX_DELETE(m_pOctree);
	}
	m_pOctree			= new COctree();
	m_pOctree->createOctree(this, minTransformObj, maxLevel);
}

bool gxWorld::appendAnimationSetToWorld(gxAnimationSet* animset)
{
	if(std::find(m_vAnimationSetList.begin(), m_vAnimationSetList.end(), animset)==m_vAnimationSetList.end())
	{
		m_vAnimationSetList.push_back(animset);
		return true;
	}

	return false;
}

void gxWorld::setMetaDataFolder(const char* metaFolder)
{
	GX_STRCPY(m_szMetaDataFolder, metaFolder);
	m_cTextureManager.setMetaDataFolder(m_szMetaDataFolder);
}

const char* gxWorld::getMetaDataFolder()
{
	return m_szMetaDataFolder;
}

Camera* gxWorld::createDefaultCameraAndSetActive()
{
	//m_pActiveCameraPtr=&m_cDefaultCamera;
	if(m_pActiveCameraPtr)
	{
		removeChild(m_pActiveCameraPtr);
		GX_DELETE(m_pActiveCameraPtr);
	}
	m_pActiveCameraPtr = new Camera();
	m_pActiveCameraPtr->initCamera(&m_cRenderer);
	m_pActiveCameraPtr->setObject3dObserver(m_pObject3dObserver);
	appendChild(m_pActiveCameraPtr);
	return m_pActiveCameraPtr;
}

void gxWorld::callback_object3dRemovedFromTree(object3d* child)
{
	if(child->getID()==OBJECT3D_LIGHT)
	{
		std::vector<gxLight*>* lightList=getLightList();
		lightList->erase(std::remove(lightList->begin(), lightList->end(), child), lightList->end());
	}
	else if(child->getID()==OBJECT3D_CAMERA)
	{
		m_vCameraList.erase(std::remove(m_vCameraList.begin(), m_vCameraList.end(), child), m_vCameraList.end());
	}
}

void gxWorld::callback_object3dAppendToTree(object3d* child)
{
	//if light
	if(child->getID()==OBJECT3D_LIGHT)
		getLightList()->push_back((gxLight*)child);
	else if(child->getID()==OBJECT3D_CAMERA)
	{
		m_vCameraList.push_back((Camera*)child);
	}
	//
}

void gxWorld::callback_object3dDestroyedFromTree(object3d* child)
{
	if(child->getID()==OBJECT3D_LIGHT)
	{
		std::vector<gxLight*>* lightList=getLightList();
		lightList->erase(std::remove(lightList->begin(), lightList->end(), child), lightList->end());
	}
	else if(child->getID()==OBJECT3D_CAMERA)
	{
		if(m_pActiveCameraPtr==child)
			m_pActiveCameraPtr=NULL;
		m_vCameraList.erase(std::remove(m_vCameraList.begin(), m_vCameraList.end(), child), m_vCameraList.end());
	}

	m_cLayerManager.removeFromLayer(child, child->getLayer());
}

void gxWorld::callback_object3dLayerChanged(object3d* child, int oldLayerID)
{
	if(oldLayerID>=0)
		m_cLayerManager.removeFromLayer(child, oldLayerID);
	m_cLayerManager.appendOnLayer(child, child->getLayer());
}

void gxWorld::loadMaterialFromObject3d(object3d* obj3d)
{
	if(obj3d->getID()==OBJECT3D_MESH || obj3d->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxMesh* mesh = (gxMesh*)obj3d;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triInfo = mesh->getTriInfo(x);
			int materialCRC=triInfo->getMaterialCRCFromFileReadInfo();
			char crcFile[1024];
			sprintf(crcFile, "%s/%x", getMetaDataFolder(), materialCRC);

#if !CREATE_MATERIAL_FOR_MESH_IF_NOT_FOUND_IN_METAFILE
			if(materialCRC==0)
				continue;
#endif

			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				gxMaterial* material = new gxMaterial();
				material->read(file_meta);

				HWShaderManager* hwShaderManager = engine_getHWShaderManager();
				//load surface shader
				char mainshaderfilename[1024];
#ifdef _WIN32
				sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", material->getMainshaderName());
#elif defined(ANDROID)
				sprintf(mainshaderfilename, "//storage//emulated//0//gear//shadersAndroid//surfaceShader//%s.shader", material->getMainshaderName());
#else
				//not implemented
#endif
				material->setSurfaceShader(hwShaderManager->LoadSurfaceShader(mainshaderfilename));
				
				std::vector<gxSubMap*>* maplist=material->getSubMapList();
				for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it)
				{
					gxSubMap* submap = *it;
					submap->loadTextureFromMeta(*getTextureManager(), submap->getTextureCRC());
				}

				//check if the material name already exists in our list or not
				std::vector<gxMaterial*>* materialList = getMaterialList();
				for(std::vector<gxMaterial*>::iterator it = materialList->begin(); it != materialList->end(); ++it)
				{
					gxMaterial* material_in_list = *it;
					if(material_in_list->getAssetFileCRC()==material->getAssetFileCRC())
					{
						//match found, so assing and delete the new material object
						triInfo->setMaterial(material_in_list);
						GX_DELETE(material);
						break;
					}
				}

				if(triInfo->getMaterial()==NULL)
				{
					//assign the new materiak
					triInfo->setMaterial(material);
					materialList->push_back(material);
				}
				file_meta.CloseFile();
			}
			else
			{
				triInfo->setMaterial(gxMaterial::createNewMaterial());
				getMaterialList()->push_back(triInfo->getMaterial());
			}
		}
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=obj3d->getChildList()->getHead();
    while(node)
    {
		object3d* childobj=node->getData();
		loadMaterialFromObject3d(childobj);
        node=node->getNext();
	}
#else
	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadMaterialFromObject3d(childobj);
	}
#endif
}

void gxWorld::loadAnmationFromObject3d(object3d* obj3d, int crc)
{
	gxAnimation* animationController = obj3d->getAnimationController();
	if(animationController)
	{
		std::vector<gxAnimationSet*> duplicateList;
		std::vector<gxAnimationSet*> reinsertFromWorldList;
		std::vector<gxAnimationSet*>* animationSetList=animationController->getAnimationSetList();
		for(std::vector<gxAnimationSet*>::iterator it = animationSetList->begin(); it != animationSetList->end(); ++it)
		{
			gxAnimationSet* animationSet = *it;

			bool bFound=false;
			//check if the animation already on world list
			std::vector<gxAnimationSet*>* world_animationSetList=getAnimationSetList();
			for(std::vector<gxAnimationSet*>::iterator it2 = world_animationSetList->begin(); it2 != world_animationSetList->end(); ++it2)
			{
				gxAnimationSet* worldanimationSet = *it2;
				if(worldanimationSet->getCRCOfMeshData()==crc && strcmp(worldanimationSet->getAnimationName(), animationSet->getAnimationName())==0)
				{
					bFound=true;
					duplicateList.push_back(animationSet);
					reinsertFromWorldList.push_back(worldanimationSet);
					break;
				}
			}

			if(!bFound)
			{
				animationSet->setCRCOfMeshData(crc);
				appendAnimationSetToWorld(animationSet);
			}
		}

		//destroy duplicates
		for(std::vector<gxAnimationSet*>::iterator it = duplicateList.begin(); it != duplicateList.end(); ++it)
		{
			gxAnimationSet* duplicate_animationSet = *it;
			animationSetList->erase(std::remove(animationSetList->begin(), animationSetList->end(), duplicate_animationSet), animationSetList->end());
			GX_DELETE(duplicate_animationSet);
		}
		duplicateList.clear();

		//reinsert any animset from world to object's animlist
		for(std::vector<gxAnimationSet*>::iterator it = reinsertFromWorldList.begin(); it != reinsertFromWorldList.end(); ++it)
		{
			gxAnimationSet* reinsert_animationSet = *it;
			animationSetList->push_back(reinsert_animationSet);
		}
		reinsertFromWorldList.clear();
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=obj3d->getChildList()->getHead();
    while(node)
    {
		object3d* childobj=node->getData();
		loadAnmationFromObject3d(childobj, crc);
        node=node->getNext();
	}
#else
	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadAnmationFromObject3d(childobj, crc);
	}
#endif
}

void gxWorld::populateBonesToMeshNode(object3d* obj, object3d* rootNode)
{
	if(obj->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxSkinnedMesh* skinMesh = (gxSkinnedMesh*)obj;
		int index=0;
		skinMesh->clearPrivateIterator();
		skinMesh->populateBoneList(rootNode, index);
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=obj->getChildList()->getHead();
    while(node)
    {
		object3d* childobj=node->getData();
		populateBonesToMeshNode(childobj, rootNode);
        node=node->getNext();
	}
#else
	std::vector<object3d*>* childlist=obj->getChildList();
	for(std::vector<object3d*>::iterator it = childlist->begin(); it != childlist->end(); ++it)
	{
		object3d* childobj = *it;
		populateBonesToMeshNode(childobj, rootNode);
	}
#endif
}

void gxWorld::read3dFile(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;

		switch(objID)
		{
		case OBJECT3D_MESH:
			tempObj = new gxMesh();
			break;
		case OBJECT3D_SKINNED_MESH:
			tempObj = new gxSkinnedMesh();
			break;
		case OBJECT3D_LIGHT:
			tempObj = new gxLight();
			break;
		default:
			tempObj = new object3d(objID);
		}

		tempObj->setObject3dObserver(m_pObject3dObserver);
		tempObj->read(file);
		obj->appendChild(tempObj);
		read3dFile(file, tempObj);
	}
}

object3d* gxWorld::loadAndAppendFBXForDevice(const char* filename)
{
	object3d* root_object_node=NULL;
	if (gxUtil::GX_IS_EXTENSION(filename, ".fbx") || gxUtil::GX_IS_EXTENSION(filename, ".FBX") ||
		gxUtil::GX_IS_EXTENSION(filename, ".prefab") || gxUtil::GX_IS_EXTENSION(filename, ".PREFAB"))
	{
		object3d* obj = NULL;
		int crc=gxCrc32::Calc((unsigned char*)filename);

		char crcFile[1024];
		sprintf(crcFile, "%s/%x", getMetaDataFolder(), crc);

		gxFile file_meta;
		if(file_meta.OpenFile(crcFile))
		{
			stMetaHeader metaHeader;
			file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

			int objID=0;
			file_meta.Read(objID);

			object3d* tempObj=NULL;

			switch(objID)
			{
			case OBJECT3D_MESH:
				tempObj = new gxMesh();
				break;
			case OBJECT3D_SKINNED_MESH:
				tempObj = new gxSkinnedMesh();
				break;
			case OBJECT3D_LIGHT:
				tempObj = new gxLight();
				break;
			default:
				tempObj = new object3d(objID);
			}

			if(tempObj)
			{
				tempObj->setObject3dObserver(m_pObject3dObserver);
				tempObj->read(file_meta);
				appendChild(tempObj);
				read3dFile(file_meta, tempObj);
				obj=tempObj;
				loadMaterialFromObject3d(obj);
				loadAnmationFromObject3d(obj, crc);
				root_object_node=obj;
			}
			file_meta.CloseFile();
		}
	}

	transformationChangedf();

	if(m_pEngineObserver)
			m_pEngineObserver->onAppendToWorld(this, root_object_node);

	populateBonesToMeshNode(root_object_node, root_object_node);

	return root_object_node;
}
