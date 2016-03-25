#include "gxWorld.h"
#include "../util/gxUtil.h"
#include "../core/gxMetaStructures.h"
#include "../hwShader/HWShaderManager.h"
#include "../GEAREngine.h"
#include "gxSkinnedMesh.h"
#include "../util/gxCrc32.h"

gxWorld* gxWorld::create()
{
    auto newObject = new gxWorld();
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxWorld::gxWorld():
	object3d(OBJECT3D_WORLD)
{
    setName("gxWorld");
	layerID = ELAYER_DEFAULT;	//CAUTION: Do not use setLayer() for gxWorld()
	worldObserver = NULL;
	activeCamera = NULL;

	setRootObserverOfTree(this);

	activeCamera=NULL;
	createDefaultCameraAndSetActive();

	defaultMaterial.setMaterialName("Default");
	materialList.push_back(&defaultMaterial);

	textureManager.LoadDefaultTextures();
	renderer.setGEARTexture1x1(textureManager.getGEARTexture1x1());

#ifdef USE_BULLET
	m_cPhysicsEngine.initPhysics();
#endif

	octree=NULL;
}

gxWorld::~gxWorld()
{
	resetWorld(true);
	materialList.clear();	//since our Default material may reside inside: check resetWorld()
}

void gxWorld::resetWorld(bool bDontCreateDefaultCamera)
{
#ifdef USE_BULLET
	m_cPhysicsEngine.clientResetScene();
#endif

	if(octree)
	{
		octree->reset();
		GX_DELETE(octree);
	}

	for(std::vector<gxMaterial*>::iterator it = materialList.begin(); it != materialList.end(); ++it)
	{
		gxMaterial* material = *it;
		if(material==&defaultMaterial)
			continue;
		REF_RELEASE(material);
	}
	materialList.clear();
	//repush our Default Material;
	materialList.push_back(&defaultMaterial);	//must be removed on the world destructor
	//

	for(std::vector<gxAnimationSet*>::iterator it = animationSetList.begin(); it != animationSetList.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		REF_RELEASE(animationSet);
	}
	animationSetList.clear();

	lightList.clear();
	layerManager.clearLayers();
	cameraList.clear();

	//destroy all child
	for(std::vector<object3d*>::iterator it = childList.begin(); it != childList.end(); ++it)
	{
		object3d* obj = *it;
		REF_RELEASE(obj);
	}
	childList.clear();
	//

	activeCamera=NULL;
	if(!bDontCreateDefaultCamera)
		createDefaultCameraAndSetActive();
}

void gxWorld::update(float dt)
{
#ifdef GEAR_WINDOWS	//arun:special case
	//this can be removed from here since transformationchanged func will call camera update
	if(activeCamera)
	{
		activeCamera->updateCamera();
	}
	//
#endif

#ifdef USE_BULLET
	m_cPhysicsEngine.update(dt);
#endif

	//collide with octree
	if(octree && activeCamera)
	{
		activeCamera->extractFrustumPlanes();
		
		octree->resetCollidedTransformObjList();
		//octree->CheckOverlapWithOctree(octree->GetRoot(), GetCurrentCamera());
		octree->checkFrustumOverlapWithOctree(octree->getRoot(), &activeCamera->getFrustum(), activeCamera->getLayerCullingMask());
		//DEBUG_PRINT("octree item %d", octree->GetCollidedTransformObjList()->GetCount());
	}

	object3d::update(dt);
}

void gxWorld::renderFromOctreeList(gxRenderer* renderer, ExpandableArray<object3d*>* list, int renderFlag)
{
	ExpandableArrayNode<object3d*>* collidedtransformObjNode=list->GetRoot();
		
	int count=list->GetCount();
	while(collidedtransformObjNode && count--)
	{
		object3d* obj = collidedtransformObjNode->GetData();

		if((renderFlag&eObject3dBase_RenderFlag_FirstPass))
		{
			getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
			if(!obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible))
			{
				collidedtransformObjNode=collidedtransformObjNode->GetNext();
				continue;
			}
			obj->render(renderer, NULL, renderFlag);
		}

		if((renderFlag&eObject3dBase_RenderFlag_SecondPass))
		{
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
				obj->render(renderer, light, renderFlag);
			}
			CHECK_GL_ERROR(glDisable(GL_BLEND));
		}

		collidedtransformObjNode=collidedtransformObjNode->GetNext();
	}


#if 0
	//check for duplicates
	for(int x=0;x<list->GetCount()-1;x++)
	{
		for(int y=x+1;y<list->GetCount();y++)
		{
			if(list->getNode(x)->GetData()==list->getNode(y)->GetData())
			{
				int isue;
				isue=0;
			}
		}
	}
#endif
}

void gxWorld::render(gxRenderer* renderer, object3d* light, int renderFlag /*EOBJECT3DRENDERFLAGS*/)
{
	if(activeCamera)
	{
		activeCamera->processCamera();
	}

	renderer->noOfTrianglesRendered=0;
	renderer->noOfDrawCalls=0;

	if(worldObserver)worldObserver->preWorldRender();


	////////////////////////////////
	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));

	if(octree)
	{
		//render octree
		renderFromOctreeList(renderer,  octree->getCollidedObjList(), renderFlag|eObject3dBase_RenderFlag_DontRenderChilds);
		renderFromOctreeList(renderer,  octree->getCollidedAlphaObjList(), renderFlag|eObject3dBase_RenderFlag_DontRenderChilds);
	}
	else
	{
		getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
		CHECK_GL_ERROR(object3d::render(renderer, NULL, renderFlag));

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
			CHECK_GL_ERROR(object3d::render(renderer, light, renderFlag));
		}
		CHECK_GL_ERROR(glDisable(GL_BLEND));
		//glDepthMask(GL_TRUE);
		//glEnable(GL_DEPTH_TEST);
	}
	////////////////////////////////

	if(worldObserver)worldObserver->postWorldRender();
}

void gxWorld::renderShadow(int renderFlag)
{
	std::vector<gxLight*>* lightList = getLightList();
	if (lightList->size() == 0)
		return;

	CHECK_GL_ERROR(glShadeModel(GL_FLAT));
	CHECK_GL_ERROR(glPushAttrib(GL_VIEWPORT_BIT));

	gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_SHADOWMAP_SHADER);
	shader->enableProgram();
	renderer.setRenderPassType(gxRenderer::RENDER_SHADOWMAP);

	for (std::vector<gxLight*>::iterator it = lightList->begin(); it != lightList->end(); ++it)
	{
		gxLight* light = *it;
		if (!light->isBaseFlag(eObject3dBaseFlag_Visible))
			continue;

		FBO& shadowMapFBO = light->getShadowMapFBO();
		shadowMapFBO.BindFBO();
		CHECK_GL_ERROR(glViewport(0, 0, shadowMapFBO.getFBOWidth(), shadowMapFBO.getFBOHeight()));
		CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
		CHECK_GL_ERROR(glClear(GL_DEPTH_BUFFER_BIT));

		//renderer.getShadowMap()->updateMatrix(light->getWorldMatrix(), renderer.getViewMatrix());
		shader->sendUniformTMfv("u_depth_mvp_m4x4", light->getShadowDepthMVP().getOGLMatrix(), false, 4);
		object3d::render(&renderer, NULL, renderFlag);
		shadowMapFBO.UnBindFBO();
	}

	CHECK_GL_ERROR(glPopAttrib());
	renderer.setRenderPassType(gxRenderer::RENDER_NORMAL);
	shader->disableProgram();
	CHECK_GL_ERROR(glShadeModel(GL_SMOOTH));
}

/*
void gearSceneWorldEditor::drawShadowMapPass()
{
	std::vector<gxLight*>* lightList = getLightList();
	if (lightList->size() == 0)
		return;

	gxLight* light = lightList->at(0);

	shadowMapFBO.BindFBO();
	//projection
	//glm::vec3 lightInvDir = glm::vec3(0.5f,2,2);
	//// Compute the MVP matrix from the light's point of view
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	matrix4x4f depthProjectionMatrix;
	depthProjectionMatrix.setOrtho(-shadowMapFBO.getFBOWidth()*0.5f, shadowMapFBO.getFBOWidth()*0.5f,
		-shadowMapFBO.getFBOHeight()*0.5f, shadowMapFBO.getFBOHeight()*0.5f, 0, 10000);
	matrix4x4f depthViewMatrix;

	vector3f light_pos(light->getPosition());
	vector3f forward(light_pos);
	forward.normalize();
	vector3f up(0, 0, 1);
	vector3f left(up.cross(forward));
	left.normalize();
	up = forward.cross(left);
	up.normalize();
	depthViewMatrix.setXAxis(left);
	depthViewMatrix.setYAxis(up);
	depthViewMatrix.setZAxis(forward);

	depthViewMatrix.setPosition(light_pos);

	//matrix4x4f biasMatrix(
	//0.5, 0.0, 0.0, 0.0,
	//0.0, 0.5, 0.0, 0.0,
	//0.0, 0.0, 0.5, 0.0,
	//0.5, 0.5, 0.5, 1.0
	//);

	matrix4x4f depthMVP(depthProjectionMatrix * depthViewMatrix.getInverse());
	//matrix4x4f depthBiasMVP(depthMVP * biasMatrix);
	//const float* u_mvp_m4x4= (*m_pMainWorldPtr->getRenderer()->getViewProjectionMatrix() * (*light->getWorldMatrix() * lightBillBoardSprite)).getMatrix();

	CHECK_GL_ERROR(glViewport(0, 0, shadowMapFBO.getFBOWidth(), shadowMapFBO.getFBOHeight()));
	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_SHADOWMAP);

	gxHWShader* shader = engine_getHWShaderManager()->GetHWShader(HW_BUILTIN_DEFAULT_SHADER_ONLY_SHADOWMAP_SHADER);
	shader->enableProgram();
	shader->sendUniformTMfv("u_depth_mvp_m4x4", depthMVP.getOGLMatrix(), false, 4);
	m_pMainWorldPtr->renderShadow(object3d::eObject3dBase_RenderFlag_NormalRenderPass);
	m_pMainWorldPtr->getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);

	shader->disableProgram();
	shadowMapFBO.UnBindFBO();
}*/

void gxWorld::renderSingleObject(object3d* obj, object3d* lightPtr, int renderFlag)
{
	if(activeCamera)
	{
		activeCamera->processCamera();
	}

	CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
	getRenderer()->setRenderPassType(gxRenderer::RENDER_NORMAL);
	obj->render(&renderer, NULL, renderFlag);

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
		obj->render(&renderer, light, renderFlag);
	}
	CHECK_GL_ERROR(glDisable(GL_BLEND));
}

void gxWorld::resizeWorld(float x, float y, float cx, float cy, float nearplane, float farplane)
{
	renderer.setViewPort(x, y, cx, cy);
	if(activeCamera)
	{	
		activeCamera->setUpCameraPerspective(cx, cy/*, 45.0f, nearplane, farplane*/);
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
				triInfo->getMaterial()->loadTextureFromDirectory(textureManager, directorPathPtr);
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
	if(octree)
	{
		octree->reset();
		GX_DELETE(octree);
	}
	octree			= new COctree();
	octree->createOctree(this, minTransformObj, maxLevel);
}

bool gxWorld::appendAnimationSetToWorld(gxAnimationSet* animset)
{
	if(std::find(animationSetList.begin(), animationSetList.end(), animset)==animationSetList.end())
	{
		animationSetList.push_back(animset);
        REF_RETAIN(animset);
		return true;
	}

	return false;
}

void gxWorld::setMetaDataFolder(const char* metaFolder)
{
	GX_STRCPY(metaDataFolder, metaFolder);
	textureManager.setMetaDataFolder(metaDataFolder);
}

const char* gxWorld::getMetaDataFolder()
{
	return metaDataFolder;
}

void gxWorld::setActiveCamera(Camera* camera)
{
    if(activeCamera==camera) return;

    if(activeCamera)
		activeCamera->setMainCamera(false);
    REF_RELEASE(activeCamera);
	activeCamera = camera;
    REF_RETAIN(activeCamera);
	if(activeCamera)
		activeCamera->setMainCamera(true);
}

Camera* gxWorld::createDefaultCameraAndSetActive()
{
	if(activeCamera)
	{
		removeChild(activeCamera);
	}
    activeCamera = Camera::create();
	activeCamera->initCamera(&renderer);
	activeCamera->setMainCamera(true);
	activeCamera->setObject3dObserver(object3DObserver);
	appendChild(activeCamera);
    REF_RELEASE(activeCamera);
	return activeCamera;
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
		cameraList.erase(std::remove(cameraList.begin(), cameraList.end(), child), cameraList.end());
	}
}

void gxWorld::callback_object3dAppendToTree(object3d* child)
{
	//if light
	if(child->getID()==OBJECT3D_LIGHT)
		getLightList()->push_back((gxLight*)child);
	else if(child->getID()==OBJECT3D_CAMERA)
	{
		cameraList.push_back((Camera*)child);
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
		if(activeCamera==child)
			activeCamera=NULL;
		cameraList.erase(std::remove(cameraList.begin(), cameraList.end(), child), cameraList.end());
	}

	layerManager.removeFromLayer(child, child->getLayer());
}

void gxWorld::callback_object3dLayerChanged(object3d* child, int oldLayerID)
{
	if(oldLayerID>=0)
		layerManager.removeFromLayer(child, oldLayerID);
	layerManager.appendOnLayer(child, child->getLayer());
}

bool gxWorld::appendMaterialToWorld(gxMaterial* mat)
{
    if(std::find(materialList.begin(), materialList.end(), mat)!=materialList.end())
        return false;
    
    REF_RETAIN(mat);
    materialList.push_back(mat);
    return true;
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
			char crcFile[FILENAME_MAX];
			sprintf(crcFile, "%s/%x", getMetaDataFolder(), materialCRC);

#if !CREATE_MATERIAL_FOR_MESH_IF_NOT_FOUND_IN_METAFILE
			if(materialCRC==0)
				continue;
#endif

			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
                metaHeader.readMetaHeader(file_meta);

                gxMaterial* material = gxMaterial::create();
				material->read(file_meta);

				HWShaderManager* hwShaderManager = engine_getHWShaderManager();
				//load surface shader
				char mainshaderfilename[FILENAME_MAX];
#ifdef GEAR_EDITOR
				sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", material->getMainshaderName());
#elif defined(GEAR_ANDROID)
				sprintf(mainshaderfilename, "//storage//emulated//0//gear//shadersAndroid//surfaceShader//%s.shader", material->getMainshaderName());
#else
                #error not implemented
#endif
				material->setSurfaceShader(hwShaderManager->LoadSurfaceShader(mainshaderfilename));
				
				std::vector<gxSubMap*>* maplist=material->getSubMapList();
				for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it)
				{
					gxSubMap* submap = *it;
					submap->loadTextureFromMeta(*getTextureManager(), submap->getTextureCRC());
				}

				//check if the material name already exists in our list or not
				//const std::vector<gxMaterial*>* materialList = getMaterialList();
				for(std::vector<gxMaterial*>::iterator it = materialList.begin(); it != materialList.end(); ++it)
				{
					gxMaterial* material_in_list = *it;
					if(material_in_list->getAssetFileCRC()==material->getAssetFileCRC())
					{
						//match found, so assing and delete the new material object
						triInfo->setMaterial(material_in_list);
						REF_RELEASE(material);
						break;
					}
				}

				if(triInfo->getMaterial()==NULL)
				{
					//assign the new materiak
					triInfo->setMaterial(material);
					appendMaterialToWorld(material);
                    REF_RELEASE(material);
				}
				file_meta.CloseFile();
			}
			else
			{
                auto newMat = gxMaterial::createNewMaterial();
				triInfo->setMaterial(newMat);
                appendMaterialToWorld(newMat);
                REF_RELEASE(newMat);
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
	const std::vector<object3d*>* childList=obj3d->getChildList();
    for (auto childobj : *childList)
    {
		loadMaterialFromObject3d(childobj);
	}
#endif
}

void gxWorld::tryLoadTexturesFromObject3d(object3d* obj3d, const char* filepath)
{
	gxWorld* mainworld=this;
	char buffer[FILENAME_MAX];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, gxUtil::getFileNameFromPath(filepath));
	char dirname[FILENAME_MAX];
	memset(dirname, 0, sizeof(dirname));
	strncpy(dirname, filepath, strlen(filepath)-strlen(buffer));


	if(obj3d->getID()==OBJECT3D_MESH || obj3d->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxMesh* mesh = (gxMesh*)obj3d;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triInfo = mesh->getTriInfo(x);
			gxMaterial* material = triInfo->getMaterial();
			if(!material) continue;
			std::vector<gxSubMap*>* maplist=material->getSubMapList();
			if(maplist->size()==0 || material->getListOfTextureNamesFromFBX()->size()==0) continue;

			char relativetexturefilepath[FILENAME_MAX];
			memset(relativetexturefilepath, 0, sizeof(relativetexturefilepath));
			sprintf(relativetexturefilepath, "%s%s", dirname, material->getListOfTextureNamesFromFBX()->at(0).c_str());

			////tif hack
			//if(gxUtil::GX_IS_EXTENSION(relativetexturefilepath, ".tif") || gxUtil::GX_IS_EXTENSION(relativetexturefilepath, ".TIF"))
			//	strcpy(&relativetexturefilepath[strlen(relativetexturefilepath)-3], "png");
			////

			gxSubMap* submap = maplist->at(0);
			int textureCRC=gxCrc32::Calc((unsigned char*)relativetexturefilepath);
			char crcFile[FILENAME_MAX];
			sprintf(crcFile, "%s/%x", mainworld->getMetaDataFolder(), textureCRC);
			if(submap->loadTextureFromMeta(*mainworld->getTextureManager(), textureCRC))
			{
				submap->setTextureCRC(textureCRC);
			}
		}
	}

#ifdef USE_BXLIST
	stLinkNode<object3d*>* node=obj3d->getChildList()->getHead();
	while(node)
	{
		object3d* childobj=node->getData();
		tryLoadTexturesFromObject3d(childobj);
		node=node->getNext();
	}
#else
    const std::vector<object3d*>* childList=obj3d->getChildList();
    for (auto childobj : *childList)
	{
		tryLoadTexturesFromObject3d(childobj, filepath);
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
		const std::vector<gxAnimationSet*>* animationSetList=animationController->getAnimationSetList();
        for(auto animationSet : *animationSetList)
		{
			bool bFound=false;
			//check if the animation already on world list
			const std::vector<gxAnimationSet*>* world_animationSetList=getAnimationSetList();
            for(auto worldanimationSet : *world_animationSetList)
			{
				if(worldanimationSet->getCRCOfMeshData()==crc && worldanimationSet->getAnimationName().compare(animationSet->getAnimationName())==0)
				{
					bFound=true;
					duplicateList.push_back(animationSet);
                    if(std::find(reinsertFromWorldList.begin(), reinsertFromWorldList.end(), worldanimationSet)==reinsertFromWorldList.end())
                    {
                        reinsertFromWorldList.push_back(worldanimationSet);
                    }
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
        for(auto duplicate_animationSet : duplicateList)
		{
            animationController->removeAnimationSet(duplicate_animationSet);
		}
		duplicateList.clear();

		//reinsert any animset from world to object's animlist
        for(auto reinsert_animationSet : reinsertFromWorldList)
		{
            animationController->appendAnimationSet(reinsert_animationSet);
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
    const std::vector<object3d*>* childList=obj3d->getChildList();
    for (auto childobj : *childList)
	{
		loadAnmationFromObject3d(childobj, crc);
	}
#endif
}

void gxWorld::populateBonesToMeshNode(object3d* obj3d, object3d* rootNode)
{
	if(obj3d->getID()==OBJECT3D_SKINNED_MESH)
	{
		gxSkinnedMesh* skinMesh = (gxSkinnedMesh*)obj3d;
		int index=0;
//		skinMesh->setRootNode(rootNode);
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
    const std::vector<object3d*>* childList=obj3d->getChildList();
    for (auto childobj : *childList)
	{
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
                tempObj = gxMesh::create();
			break;
		case OBJECT3D_SKINNED_MESH:
                tempObj = gxSkinnedMesh::create();
			break;
		case OBJECT3D_LIGHT:
                tempObj = gxLight::create();
			break;
		case OBJECT3D_CAMERA:
			{
                tempObj = Camera::create();
				((Camera*)tempObj)->initCamera(&renderer);
			}
			break;
		default:
            tempObj = object3d::create(objID);
		}

		tempObj->setObject3dObserver(object3DObserver);
		tempObj->read(file);
		obj->appendChild(tempObj);
        REF_RELEASE(tempObj);
		read3dFile(file, tempObj);
	}
}

object3d* gxWorld::loadObjectsFromFile(gxFile& file, int crc)
{
    object3d* tempObj=NULL;
    
    int objID=0;
    file.Read(objID);
    switch(objID)
    {
        case OBJECT3D_MESH:
            tempObj = gxMesh::create();
            break;
        case OBJECT3D_SKINNED_MESH:
            tempObj = gxSkinnedMesh::create();
            break;
        case OBJECT3D_LIGHT:
            tempObj = gxLight::create();
            break;
        case OBJECT3D_CAMERA:
        {
            tempObj = Camera::create();
            ((Camera*)tempObj)->initCamera(&renderer);
        }
            break;
        default:
            tempObj = object3d::create(objID);
    }
    
    if(tempObj)
    {
        tempObj->setObject3dObserver(object3DObserver);
        tempObj->read(file);
        appendChild(tempObj);
        REF_RELEASE(tempObj);
        read3dFile(file, tempObj);
        loadMaterialFromObject3d(tempObj);
        loadAnmationFromObject3d(tempObj, crc);
    }

    transformationChangedf();
    
    if(engineObserver)
        engineObserver->onAppendToWorld(this, tempObj);
    
    //cache the transform
    matrix4x4f temp(*tempObj);
    populateBonesToMeshNode(tempObj, tempObj);
    
    //restore the original transform
    tempObj->copyMatrix(temp);

    return tempObj;
}

object3d* gxWorld::loadFromCRCFile(int crc)
{
	char crcFile[FILENAME_MAX];
	sprintf(crcFile, "%s/%x", getMetaDataFolder(), crc);

	gxFile file_meta;
	if(!file_meta.OpenFile(crcFile))
	{
		return NULL;
	}

    //read the meta header from the crc file
	stMetaHeader metaHeader;
    metaHeader.readMetaHeader(file_meta);
    
    //read the actual objects
    object3d* root_object_node = loadObjectsFromFile(file_meta, crc);
	file_meta.CloseFile();
	
	return root_object_node;
}

object3d* gxWorld::loadAndAppendFBXForDevice(const char* filename)
{
	if (gxUtil::GX_IS_EXTENSION(filename, ".fbx") || gxUtil::GX_IS_EXTENSION(filename, ".FBX") ||
		gxUtil::GX_IS_EXTENSION(filename, ".prefab") || gxUtil::GX_IS_EXTENSION(filename, ".PREFAB"))
	{
		int crc=gxCrc32::Calc((unsigned char*)filename);
		return loadFromCRCFile(crc);
	}

	return NULL;
}
