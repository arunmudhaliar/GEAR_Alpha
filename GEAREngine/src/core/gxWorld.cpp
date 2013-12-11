#include "gxWorld.h"
#include "../fbxImporter/fbxImporter.h"
#include "../util/gxUtil.h"
#include "../core/gxMetaStructures.h"
#include "../hwShader/HWShaderManager.h"
#include "../GEAREngine.h"
//#include "../GEAREngine.cpp"

gxWorld::gxWorld():
	object3d(OBJECT3D_WORLD)
{
	m_pObserverPtr = NULL;
	m_pActiveCameraPtr = NULL;

	m_pActiveCameraPtr=NULL;
	setDefaultCameraActive();
	m_pActiveCameraPtr->initCamera(&m_cRenderer);

	//vector3f v(1, -1, -1);
	//m_pActiveCameraPtr->setDirection(&v);
	m_pActiveCameraPtr->setCamera(&m_cDefaultCameraStruct);
	m_pActiveCameraPtr->updateLocalPositionf(0, 0, 300);

	m_cDefaultMaterial.setMaterialName("Default");
	m_cMaterialList.push_back(&m_cDefaultMaterial);

	m_cTextureManager.LoadDefaultTextures();
	m_cRenderer.setGEARTexture1x1(m_cTextureManager.getGEARTexture1x1());

	m_cPhysicsEngine.initPhysics();
}

gxWorld::~gxWorld()
{
	resetWorld();
}

void gxWorld::resetWorld()
{
	m_cPhysicsEngine.clientResetScene();

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

	m_vLightList.clear();
}

void gxWorld::update(float dt)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->updateCamera();
	}

	m_cPhysicsEngine.update(dt);

	object3d::update(dt);
}

void gxWorld::render(gxRenderer* renderer, object3d* light)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	renderer->m_nTrisRendered=0;
	renderer->m_nDrawCalls=0;

	if(m_pObserverPtr)m_pObserverPtr->preWorldRender();
	object3d::render(renderer, light);
	if(m_pObserverPtr)m_pObserverPtr->postWorldRender();
}

void gxWorld::renderSingleObject(object3d* obj, object3d* light)
{
	if(m_pActiveCameraPtr)
	{
		m_pActiveCameraPtr->processCamera();
	}

	obj->render(&m_cRenderer, light);
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

Camera* gxWorld::setDefaultCameraActive()
{
	m_pActiveCameraPtr=&m_cDefaultCamera;

	return m_pActiveCameraPtr;
}

void gxWorld::calculateAABB()
{
	object3d::calculateAABB();
}

void gxWorld::callback_object3dRemovedFromTree(object3d* child)
{
	if(child->getID()==OBJECT3D_LIGHT)
	{
		std::vector<gxLight*>* lightList=getLightList();
		lightList->erase(std::remove(lightList->begin(), lightList->end(), child), lightList->end());
	}
}

void gxWorld::callback_object3dAppendToTree(object3d* child)
{
	//if light
	if(child->getID()==OBJECT3D_LIGHT)
		getLightList()->push_back((gxLight*)child);
	//
}

void gxWorld::callback_object3dDestroyedFromTree(object3d* child)
{
	if(child->getID()==OBJECT3D_LIGHT)
	{
		std::vector<gxLight*>* lightList=getLightList();
		lightList->erase(std::remove(lightList->begin(), lightList->end(), child), lightList->end());
	}
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
				sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", material->getMainshaderName());
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
					if(material_in_list->getFileCRC()==material->getFileCRC())
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

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadMaterialFromObject3d(childobj);
	}
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

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadAnmationFromObject3d(childobj, crc);
	}
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

	std::vector<object3d*>* childlist=obj->getChildList();
	for(std::vector<object3d*>::iterator it = childlist->begin(); it != childlist->end(); ++it)
	{
		object3d* childobj = *it;
		populateBonesToMeshNode(childobj, rootNode);
	}
}

void gxWorld::read3dFile2(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;
		if(objID==OBJECT3D_MESH)
		{
			tempObj = new gxMesh();
		}
		else if(objID==OBJECT3D_SKINNED_MESH)
		{
			tempObj = new gxSkinnedMesh();
		}
		else
		{
			tempObj = new object3d(objID);
		}

		tempObj->setObject3dObserver(m_pObject3dObserver);
		tempObj->read(file);
		//tempObj->calculateInitialAABB();
		tempObj->transformationChangedf();
		obj->appendChild(tempObj);
		read3dFile2(file, tempObj);
	}
}

object3d* gxWorld::loadAndAppendFBX(const char* filename)
{
	object3d* root_object_node=NULL;
	if (gxUtil::GX_IS_EXTENSION(filename, ".fbx") || gxUtil::GX_IS_EXTENSION(filename, ".FBX") ||
		gxUtil::GX_IS_EXTENSION(filename, ".prefab") || gxUtil::GX_IS_EXTENSION(filename, ".PREFAB"))
	{
		object3d* obj = NULL;
		char metaInfoFileName[256];
		sprintf(metaInfoFileName, "%s.meta",filename);

		gxFile metaInfoFile;
		if(metaInfoFile.OpenFile(metaInfoFileName))
		{
			int crc=0;
			metaInfoFile.Read(crc);
			metaInfoFile.CloseFile();

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

				if(objID==100)
				{
					tempObj = new gxMesh();
				}
				else if(objID==101)
				{
					tempObj = new gxSkinnedMesh();
				}
				else
				{
					tempObj = new object3d(objID);
				}

				if(tempObj)
				{
					tempObj->setObject3dObserver(m_pObject3dObserver);
					tempObj->read(file_meta);
					appendChild(tempObj);
					read3dFile2(file_meta, tempObj);
					obj=tempObj;
					loadMaterialFromObject3d(obj);
					loadAnmationFromObject3d(obj, crc);
					//obj->calculateInitialAABB();
					obj->transformationChangedf();
					root_object_node=obj;


					if(m_pEngineObserver)
						m_pEngineObserver->onAppendToWorld(this, obj);
				}
				file_meta.CloseFile();
			}
		}
	}

	populateBonesToMeshNode(root_object_node, root_object_node);

	return root_object_node;
}