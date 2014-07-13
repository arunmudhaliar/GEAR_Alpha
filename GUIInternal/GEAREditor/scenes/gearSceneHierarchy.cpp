#include "gearSceneHierarchy.h"
#include "../EditorApp.h"
#include "assetUserData.h"
#include "../../../GEAREngine/src/core/gxMetaStructures.h"
#include "../../../GEAREngine/src/GEAREngine.h"

gearSceneHierarchy::gearSceneHierarchy():
geWindow("Hierarchy")
{
	m_pCreateToolBarDropMenuBtnPtr = NULL;
	m_pClearBtn=NULL;
}

gearSceneHierarchy::~gearSceneHierarchy()
{
	//engine_setEngineObserver(NULL);
	//engine_setObject3dObserver(NULL);
	//gxWorld* world=engine_getWorld(0);
	//world->setObject3dObserver(NULL);
	engine_destroy();	
}

void gearSceneHierarchy::onCreate()
{
	engine_setEngineObserver(this);
	engine_setObject3dObserver(this);
	EditorApp::getSceneWorldEditor()->getMainWorld()->setObject3dObserverRecursive(this);
	EditorApp::getSceneWorldEditor()->getMainWorld()->setEngineObserver(this);

	m_pCreateToolBarDropMenuBtnPtr=new geToolBarDropMenu(m_pRenderer, "Create", getToolBar());
	m_pCreateToolBarDropMenuBtnPtr->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pCreateToolBarDropMenuBtnPtr);

	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("Create Object", 0x00004000);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("Create Object on Selected Node", 0x00004001);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("", 0, NULL, true);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("Point Light", 0x00004002);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("Directional Light", 0x00004003);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("", 0, NULL, true);
	m_pCreateToolBarDropMenuBtnPtr->appendMenuItem("Camera", 0x00004004);

	//clear btn
	m_pClearBtn=new geToolBarButton(m_pRenderer, "Clear All", getToolBar());
	m_pClearBtn->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pClearBtn);

	m_cGameObjectsTreeView.create(m_pRenderer, this, "gameObjectsTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(88, 382, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(68, 488, 16, 16);

	EditorApp::getSceneWorldEditor()->getMainWorld()->setEditorUserData(m_cGameObjectsTreeView.getRoot());

	//HACK: Since the default camera of world0 wont be added to the heirarchy when it initially created, we create it here.
	object3d* defaultCamera=EditorApp::getSceneWorldEditor()->getMainWorld()->getActiveCamera();
	object3d* parent_obj=defaultCamera->getParent();
	geTreeNode* rootNode = (geTreeNode*)parent_obj->getEditorUserData();//m_cGameObjectsTreeView.getRoot();
	createTVNode(rootNode, defaultCamera, defaultCamera->getName());
	rootNode->traverseSetWidth(m_cSize.x);
	m_cGameObjectsTreeView.refreshTreeView();
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

	m_cGameObjectsTreeView.draw();

	//m_cPushButton.draw();
	//m_cButton.draw();
}

void gearSceneHierarchy::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	EditorApp::getSceneWorldEditor()->selectedObject3D((object3d*)tvnode->getUserData());

	EditorApp::getScenePropertyEditor()->populatePropertyOfObject((object3d*)tvnode->getUserData());
}

void gearSceneHierarchy::selectObject3dInTreeView(object3d* objtoselect)
{
	selectObject3dInTreeView(m_cGameObjectsTreeView.getRoot(), objtoselect);
}

bool gearSceneHierarchy::selectObject3dInTreeView(geTreeNode* node, object3d* objtoselect)
{
	if(node->getUserData()==objtoselect)
	{
		m_cGameObjectsTreeView.selectNode(node);
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

	std::vector<geTreeNode*>* selectedNodeList=m_cGameObjectsTreeView.getSelectedNodeList();
	//geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
	geTreeNode* nodeAtMousePos=m_cGameObjectsTreeView.getTVNode(x, y);

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
		if(m_cGameObjectsTreeView.getScrollBar()->isScrollBarGrabbed())
			return true;

		std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();
		for(std::vector<geTreeNode*>::iterator it = selectedNodeList->begin(); it != selectedNodeList->end(); ++it)
		{
			geTreeNode* node = *it;
			newlist->push_back(node);
		}

		MDataObject* dataObject = new MDataObject(newlist, this);
		MDropSource* dropSource = new MDropSource();

		DWORD lpd=0;
		HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

		dataObject->Release();
		dropSource->Release();
		return true;
	}

	return true;
}

void gearSceneHierarchy::onDragEnter(int x, int y)
{
}

void gearSceneHierarchy::onDragDrop(int x, int y, MDataObject* dropObject)
{
	geTreeNode* rootNode = m_cGameObjectsTreeView.getRoot();
	geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y/*-getTopMarginOffsetHeight()*/);

	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		if(dropObject->getSourcePtr()==EditorApp::getSceneFileView())
		{
			const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetPath();

			if (util::GE_IS_EXTENSION(relativePath, ".fbx") || util::GE_IS_EXTENSION(relativePath, ".FBX") ||
				util::GE_IS_EXTENSION(relativePath, ".prefab") || util::GE_IS_EXTENSION(relativePath, ".PREFAB"))
			{
				object3d* obj = engine_loadAndAppendMesh(EditorApp::getSceneWorldEditor()->getMainWorld(), relativePath);
			}
		}
		else if(dropObject->getSourcePtr()==this)
		{
			if(selectedNode && !droppedDataObject->isNodeExistsInTree(selectedNode))
			{
				object3d* selectedObj=(object3d*)selectedNode->getUserData();
				object3d* droppedObj=(object3d*)droppedDataObject->getUserData();
				
				matrix4x4f inselectedNodesSpace = selectedObj->getInverse() * *droppedObj->getWorldMatrix();
				droppedObj->copy(inselectedNodesSpace);

				if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
				{
					selectedObj->appendChild(droppedObj);
				}
			}
			else if(selectedNode==NULL)
			{
				//add to the root node
				object3d* droppedObj=(object3d*)droppedDataObject->getUserData();
		
				if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
				{
					droppedObj->copy(*droppedObj->getWorldMatrix());	//thinking world is at the center; if not we need to get it in to the world space
					//monoWrapper::mono_engine_appendObject3dToRoot(monoWrapper::mono_engine_getWorld(0), droppedObj);
					monoWrapper::mono_engine_getWorld(0)->appendChild(droppedObj);
				}
			}
		}
		else if(dropObject->getSourcePtr()==EditorApp::getScenePropertyEditor())
		{
			if(droppedDataObject->getParent() && droppedDataObject->getParent()==EditorApp::getScenePropertyEditor()->getAnimationParentNode())
			{
				gxAnimationSet* animSet=(gxAnimationSet*)droppedDataObject->getUserData();
				//geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y);
				if(selectedNode)
				{
					object3d* selectedObj=(object3d*)selectedNode->getUserData();
					gxAnimation* animationController = selectedObj->createAnimationController();	//wont create new if there is already an animatiion controller exists
					animationController->appendAnimationSet(animSet);
					selectedObj->applyAnimationSetRecursive(animationController->getAnimationSetList()->size()-1);
					animationController->play(animationController->getAnimationSetList()->size()-1);
				}
			}
		}
	}
}

void gearSceneHierarchy::onDragLeave()
{
}

void gearSceneHierarchy::createTVNode(geTreeNode* parentNode, object3d* obj, const char* filename)
{
	if(obj==NULL || parentNode==NULL) return;

	Sprite2Dx* sprite=NULL;
	const char* name=NULL;

	if(obj->getParent() && obj->getParent()->getParent()==NULL)
	{
		name=filename;
		sprite=&m_cszSprites[1];
	}
	else
	{
		name=obj->getName();
		sprite=&m_cszSprites[0];
	}

	geTreeNode* newtvNode = new geTreeNode(m_pRenderer, parentNode, name, sprite);
	newtvNode->setUserData(obj);
	newtvNode->closeNode();
	obj->setEditorUserData(newtvNode);

	std::vector<object3d*>* list=obj->getChildList();
	for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		object3d* childobj = *it;
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
	recreateOctree();
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
	EditorApp::getPropertyOctree()->getOctreeVars(nTransformPerNodes, nLevels);
	world->createOctree(nTransformPerNodes, nLevels);
}

void gearSceneHierarchy::onConsoleLogFromMono(const char* msg, int msgtype)
{
	EditorApp::getSceneConsole()->appendConsoleMsg(msg, msgtype);
}

void gearSceneHierarchy::onObject3dChildAppend(object3d* child)
{
	object3d* parent_obj=child->getParent();
	geTreeNode* rootNode = (geTreeNode*)parent_obj->getEditorUserData();//m_cGameObjectsTreeView.getRoot();
	createTVNode(rootNode, child, child->getName());
	rootNode->traverseSetWidth(m_cSize.x);
	m_cGameObjectsTreeView.refreshTreeView();
	monoWrapper::mono_object3d_onObject3dChildAppend(parent_obj, child);
}

void gearSceneHierarchy::onObject3dChildRemove(object3d* child)
{
	object3d* parent_obj=child->getParent();
	geTreeNode* dtv=(geTreeNode*)child->getEditorUserData();
	geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
	dtv_parent->removeTVChild(dtv);
	destroyTVUserData(dtv);
	GE_DELETE(dtv);
	m_cGameObjectsTreeView.resetSelectedNodePtr();
	m_cGameObjectsTreeView.refreshTreeView();
	monoWrapper::mono_object3d_onObject3dChildRemove(parent_obj, child);
}

void gearSceneHierarchy::onObject3dDestroy(object3d* obj)
{
}

bool gearSceneHierarchy::onKeyDown(int charValue, int flag)
{
	if(charValue==46)	//DEL key
	{
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
		if(selectedNode)
		{
			object3d* obj=(object3d*)selectedNode->getUserData();
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), obj))
			{
				GE_DELETE(obj);
				EditorApp::getSceneWorldEditor()->selectedObject3D(NULL);
				EditorApp::getScenePropertyEditor()->populatePropertyOfObject(NULL);
			}
		}
	}
	else if(charValue==70)	//F key
	{
		EditorApp::getSceneWorldEditor()->startFollowCam();
	}

	return geWindow::onKeyDown(charValue, flag);
}

bool gearSceneHierarchy::onKeyUp(int charValue, int flag)
{
	return geWindow::onKeyUp(charValue, flag);
}

void gearSceneHierarchy::onButtonClicked(geGUIBase* btn)
{
	if(m_pClearBtn==btn)
	{
		if(m_pClearBtn->isButtonPressed())
		{
			//if(MessageBox(EditorApp::getMainWindowHandle(),"Are you sure to reset the world.","Warning",MB_YESNO|MB_ICONWARNING)==IDYES)
			{
				EditorApp::getSceneWorldEditor()->stopSimulation();
				m_cGameObjectsTreeView.clearAndDestroyAll();
				m_cGameObjectsTreeView.resetSelectedNodePtr();
				EditorApp::getSceneWorldEditor()->getMainWorld()->setEditorUserData(m_cGameObjectsTreeView.getRoot());
				monoWrapper::mono_engine_getWorld(0)->resetWorld();
				EditorApp::getScenePreview()->reinitPreviewWorld();

				EditorApp::getSceneWorldEditor()->selectedObject3D(NULL);
				EditorApp::getScenePropertyEditor()->populatePropertyOfObject(NULL);
				geTreeNode* selectedProjectFile = EditorApp::getSceneProject()->getSelectedNode();
				EditorApp::getSceneFileView()->populateFileView();
				//if(selectedProjectFile)
				//{
				//	EditorApp::getSceneFileView()->populateFiles(((assetUserData*)selectedProjectFile->getUserData())->getAssetAbsolutePath());
				//}

				//EditorApp::getScenePreview()->selectedObject3D(NULL);	//for safety
			}
			m_pClearBtn->buttonNormal(true);
		}
	}
}

void gearSceneHierarchy::onSliderChange(geGUIBase* slider)
{
}

geGUIBase* gearSceneHierarchy::getSelectedTreeNode()
{
	geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
	return selectedNode;
}

void gearSceneHierarchy::onCommand(int cmd)
{
	switch(cmd)
	{
		case 0x00004000:
			{
			object3d* emptyObject=engine_createEmptyObject3d(monoWrapper::mono_engine_getWorld(0), "EmptyObject");
			}
			break;

		case 0x00004001:
		{
			geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
			if(selectedNode)
			{
				object3d* obj=(object3d*)selectedNode->getUserData();
				object3d* emptyObject=engine_createEmptyObject3d(obj, "EmptyObject");
			}
		}
			break;

		case 0x00004002:
			{
			object3d* light=engine_createLight(monoWrapper::mono_engine_getWorld(0), "Point Light", gxLight::LIGHT_POINT);
			}
			break;
		case 0x00004003:
			{
			object3d* light=engine_createLight(monoWrapper::mono_engine_getWorld(0), "Directional Light", gxLight::LIGHT_DIRECTIONAL);
			}
			break;
		case 0x00004004:
			{
				object3d* camera=engine_createCamera(monoWrapper::mono_engine_getWorld(0), "Camera", monoWrapper::mono_engine_getWorld(0)->getRenderer());
			}
			break;
	}
}