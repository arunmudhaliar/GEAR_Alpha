#include "gearSceneHierarchy.h"
#include "../EditorApp.h"
#include "assetUserData.h"
#include "../../../GEAREngine/src/core/gxMetaStructures.h"
#include "../../../GEAREngine/src/GEAREngine.h"

gearSceneHierarchy::gearSceneHierarchy(geFontManager* fontManager):
geWindow("Hierarchy", fontManager)
{
	createToolBarDropMenuButton = NULL;
	toolBarClearButton=NULL;
    gameObjectTreeView = new geTreeView(fontManager);
}

gearSceneHierarchy::~gearSceneHierarchy()
{
	//engine_setEngineObserver(NULL);
	//engine_setObject3dObserver(NULL);
	//gxWorld* world=engine_getWorld(0);
	//world->setObject3dObserver(NULL);
    engine_destroy();
    GE_DELETE(gameObjectTreeView);
}

void gearSceneHierarchy::onCreate(float cx, float cy)
{
	engine_setEngineObserver(this);
	engine_setObject3dObserver(this);
	EditorGEARApp::getSceneWorldEditor()->getMainWorld()->setObject3dObserverRecursive(this);
	EditorGEARApp::getSceneWorldEditor()->getMainWorld()->setEngineObserver(this);

	createToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "Create", getToolBar(), fontManagerGUI);
	createToolBarDropMenuButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(createToolBarDropMenuButton);

	createToolBarDropMenuButton->appendMenuItem("Create Object", 0x00004000);
	createToolBarDropMenuButton->appendMenuItem("Create Object on Selected Node", 0x00004001);
	createToolBarDropMenuButton->appendMenuItem("", 0, NULL, true);
	createToolBarDropMenuButton->appendMenuItem("Point Light", 0x00004002);
	createToolBarDropMenuButton->appendMenuItem("Directional Light", 0x00004003);
	createToolBarDropMenuButton->appendMenuItem("", 0, NULL, true);
	createToolBarDropMenuButton->appendMenuItem("Camera", 0x00004004);

	//clear btn
	toolBarClearButton=new geToolBarButton(rendererGUI, "Clear All", getToolBar(), fontManagerGUI);
	toolBarClearButton->setGUIObserver(this);
	getToolBar()->appendToolBarControl(toolBarClearButton);

	gameObjectTreeView->create(rendererGUI, this, "gameObjectsTV", this);

	spriteArray[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[0].setClip(88, 382, 16, 16);
	spriteArray[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[1].setClip(68, 488, 16, 16);

	EditorGEARApp::getSceneWorldEditor()->getMainWorld()->setEditorUserData(gameObjectTreeView->getRoot());

	//HACK: Since the default camera of world0 wont be added to the heirarchy when it initially created, we create it here.
	object3d* defaultCamera=EditorGEARApp::getSceneWorldEditor()->getMainWorld()->getActiveCamera()->getAttachedObject();
	object3d* parent_obj=defaultCamera->getParent();
	geTreeNode* rootNode = (geTreeNode*)parent_obj->getEditorUserData();//gameObjectTreeView->getRoot();
	createTVNode(rootNode, defaultCamera, defaultCamera->getName().c_str());
	rootNode->traverseSetWidth(m_cSize.x);
	gameObjectTreeView->refreshTreeView();
	//
}

void gearSceneHierarchy::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	gameObjectTreeView->draw();

	//m_cPushButton.draw();
	//m_cButton.draw();
}

void gearSceneHierarchy::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	EditorGEARApp::getSceneWorldEditor()->selectedObject3D((object3d*)tvnode->getUserData());
	EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject((object3d*)tvnode->getUserData());
    EditorGEARApp::getAnimationEditor()->populatePropertyOfObject3d((object3d*)tvnode->getUserData());
}

void gearSceneHierarchy::selectObject3dInTreeView(object3d* objtoselect)
{
	selectObject3dInTreeView(gameObjectTreeView->getRoot(), objtoselect);
}

bool gearSceneHierarchy::selectObject3dInTreeView(geTreeNode* node, object3d* objtoselect)
{
	if(node->getUserData()==objtoselect)
	{
		gameObjectTreeView->selectNode(node);
		return true;
	}

	std::vector<geGUIBase*>* list=node->getChildControls();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		if(selectObject3dInTreeView((geTreeNode*)tvnode, objtoselect))
			return true;
	}
	return false;
}

bool gearSceneHierarchy::onMouseMove(float x, float y, int flag)
{
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	std::vector<geTreeNode*>* selectedNodeList=gameObjectTreeView->getSelectedNodeList();
	//geTreeNode* selectedNode=gameObjectTreeView->getSelectedNode();
	geTreeNode* nodeAtMousePos=gameObjectTreeView->getTVNode(x, y);

	bool bMouseClickedOnList=false;
	for(std::vector<geTreeNode*>::iterator it = selectedNodeList->begin(); it != selectedNodeList->end(); ++it)
	{
		geTreeNode* node = *it;
		if(node==nodeAtMousePos)
		{
			bMouseClickedOnList=true;
			break;
		}
	}

	if((flag&MK_LBUTTON) && bMouseClickedOnList)
	{
		if(gameObjectTreeView->getScrollBar()->isScrollBarGrabbed())
			return true;

		std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();
		for(std::vector<geTreeNode*>::iterator it = selectedNodeList->begin(); it != selectedNodeList->end(); ++it)
		{
			geTreeNode* node = *it;
			newlist->push_back(node);
		}

        MDropData* dataObject = new MDropData(newlist, this, SDL_GetWindowID(getRenderer()->getWindow()));
        doDragDropSynchronous(dataObject);
        
//		MDataObject* dataObject = new MDataObject(newlist, this);
//		MDropSource* dropSource = new MDropSource();
//
//		DWORD lpd=0;
//		HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);
//
//		dataObject->Release();
//		dropSource->Release();
		return true;
	}
    
	return true;
}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
void gearSceneHierarchy::onDragEnter(int x, int y)
{
}

void gearSceneHierarchy::onDragDrop(int x, int y, MDropData* dropObject)
{
	geTreeNode* selectedNode=gameObjectTreeView->getTVNode(x, y/*-getTopMarginOffsetHeight()*/);

	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		if(dropObject->getSourcePtr()==EditorGEARApp::getSceneFileView())
		{
			const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetPath();

			if (util::GE_IS_EXTENSION(relativePath, ".fbx") || util::GE_IS_EXTENSION(relativePath, ".FBX") ||
				util::GE_IS_EXTENSION(relativePath, ".prefab") || util::GE_IS_EXTENSION(relativePath, ".PREFAB"))
			{
				engine_loadAndAppendMesh(EditorGEARApp::getSceneWorldEditor()->getMainWorld(), relativePath);
			}
			else if(util::GE_IS_EXTENSION(relativePath, ".cs") || util::GE_IS_EXTENSION(relativePath, ".CS"))
			{
				if(selectedNode)
				{
					object3d* selectedObj=(object3d*)selectedNode->getUserData();

					monoClassDef* script = monoWrapper::mono_getMonoScriptClass(relativePath);
                    if(script==nullptr)
                    {
                        DEBUG_PRINT("monoScriptObjectInstance not found for the script %s", relativePath);
                        continue;
                    }
                    auto scriptInstance = monoScriptObjectInstance::create(script, selectedObj);
					selectedObj->attachMonoScrip(scriptInstance);
                    REF_RELEASE(scriptInstance);
				}
			}
		}
		else if(dropObject->getSourcePtr()==this)
		{
			if(selectedNode && !droppedDataObject->isNodeExistsInTree(selectedNode))
			{
				object3d* selectedObj=(object3d*)selectedNode->getUserData();
				object3d* droppedObj3d=(object3d*)droppedDataObject->getUserData();
				
				matrix4x4f inselectedNodesSpace = selectedObj->getInverse() * *droppedObj3d->getWorldMatrix();
				droppedObj3d->copyMatrix(inselectedNodesSpace);

                REF_RETAIN(droppedObj3d); //prevent from deleting this object by Ref::release()
				if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj3d))
				{
					selectedObj->appendChild(droppedObj3d);
				}
                REF_RELEASE(droppedObj3d);
			}
			else if(selectedNode==NULL)
			{
				//add to the root node
				object3d* droppedObj3d=(object3d*)droppedDataObject->getUserData();
		
                REF_RETAIN(droppedObj3d); //prevent from deleting this object by Ref::release()
				if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj3d))
				{
					droppedObj3d->copyMatrix(*droppedObj3d->getWorldMatrix());	//thinking world is at the center; if not we need to get it in to the world space
					//monoWrapper::mono_engine_appendObject3dToRoot(monoWrapper::mono_engine_getWorld(0), droppedObj);
					monoWrapper::mono_engine_getWorld(0)->appendChild(droppedObj3d);
				}
                REF_RELEASE(droppedObj3d);
			}
		}
		else if(dropObject->getSourcePtr()==EditorGEARApp::getScenePropertyEditor())
		{
			if(droppedDataObject->getParent() && droppedDataObject->getParent()==EditorGEARApp::getScenePropertyEditor()->getAnimationParentNode())
			{
				gxAnimationSet* animSet=(gxAnimationSet*)droppedDataObject->getUserData();
				//geTreeNode* selectedNode=gameObjectTreeView->getTVNode(x, y);
				if(selectedNode)
				{
					object3d* selectedObj=(object3d*)selectedNode->getUserData();
					gxAnimation* animationController = selectedObj->getAnimationController();	//wont create new if there is already an animatiion controller exists
					if(animationController->appendAnimationSet(animSet))
                    {
                        selectedObj->applyAnimationSetRecursive(animSet);
                        animationController->play(animSet);
                    }
				}
			}
		}
	}
}

void gearSceneHierarchy::onDragLeave()
{
}
//#endif

void gearSceneHierarchy::createTVNode(geTreeNode* parentNode, object3d* obj, const char* filename)
{
	if(obj==NULL || parentNode==NULL) return;

	Sprite2Dx* sprite=NULL;
	const char* name=NULL;

	if(obj->getParent() && obj->getParent()->getParent()==NULL)
	{
		name=filename;
		sprite=&spriteArray[1];
	}
	else
	{
		name=obj->getName().c_str();
		sprite=&spriteArray[0];
	}

	geTreeNode* newtvNode = new geTreeNode(rendererGUI, parentNode, name, sprite, fontManagerGUI);
	newtvNode->setUserData(obj);
	newtvNode->closeNode();
	obj->setEditorUserData(newtvNode);

    const std::vector<object3d*>* childlist = obj->getChildList();
    for (auto childobj : *childlist)
	{
		createTVNode(newtvNode, childobj, NULL);
	}
}

void gearSceneHierarchy::destroyTVUserData(geGUIBase* parent)
{
	std::vector<geGUIBase*>* list=parent->getChildControls();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		destroyTVUserData(tvnode);
	}
}

void gearSceneHierarchy::onAppendToWorld(object3d* world, object3d* obj)
{
	//recreateOctree();
	if(((gxWorld*)world)->getOctree())
	{
		((gxWorld*)world)->getOctree()->pushToRootNode(obj);
	}
	else
	{
		recreateOctree();
	}
}

void gearSceneHierarchy::onRemoveFromWorld(object3d* world, object3d* obj)
{
	recreateOctree();
}

void gearSceneHierarchy::recreateOctree()
{
	gxWorld* world=monoWrapper::mono_engine_getWorld(0);
	if(world->getOctree())
		world->getOctree()->resetCollidedTransformObjList();

	int nTransformPerNodes, nLevels;
	EditorGEARApp::getPropertyOctree()->getOctreeVars(nTransformPerNodes, nLevels);
	world->createOctree(nTransformPerNodes, nLevels);
}

void gearSceneHierarchy::onConsoleLogFromMono(const char* msg, int msgtype)
{
	EditorGEARApp::getSceneConsole()->appendConsoleMsg(msg, msgtype);
}

void gearSceneHierarchy::onObject3dChildAppend(object3d* child)
{
	object3d* parent_obj=child->getParent();
	geTreeNode* rootNode = (geTreeNode*)parent_obj->getEditorUserData();//gameObjectTreeView->getRoot();
	createTVNode(rootNode, child, child->getName().c_str());
	rootNode->traverseSetWidth(m_cSize.x);
	gameObjectTreeView->refreshTreeView();
	monoWrapper::mono_object3d_onObject3dChildAppend(parent_obj, child);

	if(child->getID()==OBJECT3D_CAMERA)
	{
        auto camera = child->getMonoScriptInstance<Camera*>();
		if(camera->isMainCamera())
		{
			monoWrapper::mono_engine_getWorld(0)->setActiveCamera(camera);
		}
	}
}

void gearSceneHierarchy::onObject3dChildRemove(object3d* child)
{
	object3d* parent_obj=child->getParent();
	geTreeNode* dtv=(geTreeNode*)child->getEditorUserData();
	geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
	dtv_parent->removeTVChild(dtv);
	destroyTVUserData(dtv);
	GE_DELETE(dtv);
	gameObjectTreeView->resetSelectedNodePtr();
	gameObjectTreeView->refreshTreeView();
	monoWrapper::mono_object3d_onObject3dChildRemove(parent_obj, child);
}

void gearSceneHierarchy::onObject3dDestroy(object3d* obj)
{
}

bool gearSceneHierarchy::onKeyDown(int charValue, int flag)
{
	if(charValue==42)	//DEL key
	{
		geTreeNode* selectedNode=gameObjectTreeView->getSelectedNode();
		if(selectedNode)
		{
			object3d* obj=(object3d*)selectedNode->getUserData();
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), obj))
			{
				EditorGEARApp::getSceneWorldEditor()->selectedObject3D(NULL);
				EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject(NULL);
			}
		}
	}
	else if(charValue==70)	//F key
	{
		EditorGEARApp::getSceneWorldEditor()->startFollowCam();
	}

	return geWindow::onKeyDown(charValue, flag);
}

bool gearSceneHierarchy::onKeyUp(int charValue, int flag)
{
	return geWindow::onKeyUp(charValue, flag);
}

void gearSceneHierarchy::clearHierarchy()
{
	EditorGEARApp::getSceneWorldEditor()->stopSimulation(true);
	gameObjectTreeView->clearAndDestroyAll();
	gameObjectTreeView->resetSelectedNodePtr();
	EditorGEARApp::getSceneWorldEditor()->getMainWorld()->setEditorUserData(gameObjectTreeView->getRoot());
	monoWrapper::mono_engine_getWorld(0)->resetWorld();
	EditorGEARApp::getScenePreview()->reinitPreviewWorld();
    EditorGEARApp::getAnimationEditor()->populatePropertyOfObject3d(nullptr);
	EditorGEARApp::getSceneWorldEditor()->selectedObject3D(nullptr);
	EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject(nullptr);
//	geTreeNode* selectedProjectFile = EditorApp::getSceneProject()->getSelectedNode();
	EditorGEARApp::getSceneFileView()->populateFileView();
    AutoReleasePool::getInstance().clearPool();
}

void gearSceneHierarchy::onButtonClicked(geGUIBase* btn)
{
	if(toolBarClearButton==btn)
	{
		if(toolBarClearButton->isButtonPressed())
		{
			//if(MessageBox(EditorGEARApp::getMainWindowHandle(),"Are you sure to reset the world.","Warning",MB_YESNO|MB_ICONWARNING)==IDYES)
			{
				clearHierarchy();
				//if(selectedProjectFile)
				//{
				//	EditorGEARApp::getSceneFileView()->populateFiles(((assetUserData*)selectedProjectFile->getUserData())->getAssetAbsolutePath());
				//}

				//EditorGEARApp::getScenePreview()->selectedObject3D(NULL);	//for safety
			}
			toolBarClearButton->buttonNormal(true);
		}
	}
}

void gearSceneHierarchy::onSliderChange(geGUIBase* slider)
{
}

geGUIBase* gearSceneHierarchy::getSelectedTreeNode()
{
	geTreeNode* selectedNode=gameObjectTreeView->getSelectedNode();
	return selectedNode;
}

bool gearSceneHierarchy::saveCurrentScene(const std::string& sceneFilePath)
{
	gxFile sceneFile;
	if(!sceneFile.OpenFile(sceneFilePath.c_str(), gxFile::FILE_w))
	{
		return false;
	}

	std::vector<geGUIBase*>* childNodeList = gameObjectTreeView->getRoot()->getChildControls();
	int nRootObjects=0;
	for(std::vector<geGUIBase*>::iterator it = childNodeList->begin(); it != childNodeList->end(); ++it)
	{
		geTreeNode* node = (geTreeNode*)*it;
		object3d* obj = (object3d*)node->getUserData();
		if(obj==monoWrapper::mono_engine_getWorld(0)->getActiveCamera()->getAttachedObject())
		{
			DEBUG_PRINT("Active camera (main camera) wont be saved", sceneFilePath.c_str());
			continue;
		}
		nRootObjects++;
	}
    
    sceneFile.Write(nRootObjects);
    for(std::vector<geGUIBase*>::iterator it = childNodeList->begin(); it != childNodeList->end(); ++it)
    {
        geTreeNode* node = (geTreeNode*)*it;
        object3d* obj = (object3d*)node->getUserData();
        
        //dont save the active camera since it will be created when the world resets or reloads.
        if(obj==monoWrapper::mono_engine_getWorld(0)->getActiveCamera()->getAttachedObject())
            continue;
        sceneFile.Write(obj->getAssetFileCRC());
        if(obj->getAssetFileCRC()==0)
        {
            DEBUG_PRINT("WARNING saveCurrentScene(%s) obj->getAssetFileCRC()==0", sceneFilePath.c_str());
            //continue;
            obj->write(sceneFile);
        }
        else
        {
            sceneFile.WriteBuffer((unsigned char*)obj->getOGLMatrix(), sizeof(float)*16);
        }
    }

    if(monoWrapper::mono_engine_getWorld(0)->getActiveCamera())
    {
        sceneFile.Write(true);
        monoWrapper::mono_engine_getWorld(0)->getActiveCamera()->write(sceneFile);
    }
    else
    {
        sceneFile.Write(false);
    }
    
	sceneFile.CloseFile();

	return true;
}

bool gearSceneHierarchy::loadScene(const std::string& sceneFilePath)
{
	gxFile sceneFile;
	if(!sceneFile.OpenFile(sceneFilePath.c_str()))
	{
		return false;
	}

	clearHierarchy();

	int nRootObjects=0;
	sceneFile.Read(nRootObjects);

	unsigned int assetcrc=0;
	for(int x=0;x<nRootObjects;x++)
	{
		sceneFile.Read(assetcrc);

        if(assetcrc!=0)
        {
            auto obj = engine_getWorld(0)->loadFromCRCFile(assetcrc);
            
            matrix4x4f mat;
            sceneFile.ReadBuffer((unsigned char*)mat.getOGLMatrix(), sizeof(float)*16);
            obj->copyMatrix(mat);
            obj->transformationChangedf();
        }
        else
            engine_getWorld(0)->loadObjectsFromFile(sceneFile, 0);
	}
    
    bool anyActiveCamSaved=false;
    sceneFile.Read(anyActiveCamSaved);
    if(anyActiveCamSaved)
    {
        auto activeCam = monoWrapper::mono_engine_getWorld(0)->getActiveCamera();
        int objID = 0;
        sceneFile.Read(objID);
        if(objID==OBJECT3D_CAMERA)
        {
            activeCam->read(sceneFile);
            activeCam->transformationChangedf();
        }
    }

	sceneFile.CloseFile();

	return true;
}

void gearSceneHierarchy::onCommand(int cmd)
{
	switch(cmd)
	{
		case 0x00004000:
			{
//			object3d* emptyObject=engine_createEmptyObject3d(monoWrapper::mono_engine_getWorld(0), "EmptyObject");
			}
			break;

		case 0x00004001:
		{
			geTreeNode* selectedNode=gameObjectTreeView->getSelectedNode();
			if(selectedNode)
			{
				object3d* obj=(object3d*)selectedNode->getUserData();
				engine_createEmptyObject3d(obj, "EmptyObject");
			}
		}
			break;

		case 0x00004002:
			{
                engine_createLight(monoWrapper::mono_engine_getWorld(0), "Point Light", gxLight::LIGHT_POINT);
			}
			break;
		case 0x00004003:
			{
                engine_createLight(monoWrapper::mono_engine_getWorld(0), "Directional Light", gxLight::LIGHT_DIRECTIONAL);
			}
			break;
		case 0x00004004:
			{
				engine_createCamera(monoWrapper::mono_engine_getWorld(0), "Camera", monoWrapper::mono_engine_getWorld(0)->getRenderer());
			}
			break;
	}
}