#include "gearSceneHierarchy.h"
#include "../EditorApp.h"
#include "assetUserData.h"
#include "../../../GEAREngine/src/core/gxMetaStructures.h"
#include "../../../GEAREngine/src/GEAREngine.h"

gearSceneHierarchy::gearSceneHierarchy():
geWindow("Hierarchy")
{
	m_pCreateToolBarDropMenuBtnPtr = NULL;
}

gearSceneHierarchy::~gearSceneHierarchy()
{
	//destroyTVUserData(m_cGameObjectsTreeView.getRoot());
}

void gearSceneHierarchy::onCreate()
{
	engine_setEngineObserver(this);
	engine_setObject3dObserver(this);
	EditorApp::getSceneWorldEditor()->getMainWorld()->setObject3dObserver(this);

	m_pCreateToolBarDropMenuBtnPtr=new geToolBarDropMenu("Create", getToolBar());
	m_pCreateToolBarDropMenuBtnPtr->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pCreateToolBarDropMenuBtnPtr);

	m_cGameObjectsTreeView.create(this, "gameObjectsTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(88, 382, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(68, 488, 16, 16);

	EditorApp::getSceneWorldEditor()->getMainWorld()->setEditorUserData(m_cGameObjectsTreeView.getRoot());
	//m_cButton.create(this, "button1", 100, 200);
	//m_cPushButton.create(this, "check", 170, 200);
}

void gearSceneHierarchy::onDraw()
{
	m_cGameObjectsTreeView.draw();

	//m_cPushButton.draw();
	//m_cButton.draw();
}

void gearSceneHierarchy::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	EditorApp::getSceneWorldEditor()->selectedObject3D((object3d*)tvnode->getUserData());

	EditorApp::getScenePropertyEditor()->populatePropertyOfObject((object3d*)tvnode->getUserData());
}

bool gearSceneHierarchy::onMouseMove(float x, float y, int flag)
{
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
	geTreeNode* nodeAtMousePos=m_cGameObjectsTreeView.getTVNode(x, y);
	if((flag&MK_LBUTTON) && selectedNode && selectedNode==nodeAtMousePos)
	{
		if(m_cGameObjectsTreeView.getScrollBar()->isScrollBarGrabbed())
			return true;
		MDataObject* dataObject = new MDataObject(selectedNode, this);
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
	geGUIBase* droppedDataObject = dropObject->getActualData();

	if(dropObject->getSourcePtr()==EditorApp::getSceneFileView())
	{
		const char* absolutePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

		if (util::GE_IS_EXTENSION(absolutePath, ".fbx") || util::GE_IS_EXTENSION(absolutePath, ".FBX") ||
			util::GE_IS_EXTENSION(absolutePath, ".prefab") || util::GE_IS_EXTENSION(absolutePath, ".PREFAB"))
		{
			object3d* obj = monoWrapper::mono_engine_loadAndAppendFBX(EditorApp::getSceneWorldEditor()->getMainWorld(), absolutePath);
			//if(obj)
			//{
			//	createTVNode(rootNode, obj, droppedDataObject->getName());
			//}
		}
	}
	else if(dropObject->getSourcePtr()==this)
	{
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y/*-getTopMarginOffsetHeight()*/);
		if(selectedNode && !droppedDataObject->isNodeExistsInTree(selectedNode))
		{
			object3d* selectedObj=(object3d*)selectedNode->getUserData();
			object3d* droppedObj=(object3d*)droppedDataObject->getUserData();
		
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
			{
				selectedObj->appendChild(droppedObj);

				//geTreeNode* dtv=(geTreeNode*)droppedDataObject;
				//geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
				//dtv_parent->removeTVChild(dtv);

				//((geTreeNode*)droppedDataObject)->setXOffset(20.0f);
				//selectedNode->appnendTVChild((geTreeNode*)droppedDataObject);
				//((geTreeNode*)droppedDataObject)->setNodeSprite(&m_cszSprites[0]);
				//rootNode->traverseSetWidth(m_cSize.x);
				//m_cGameObjectsTreeView.refreshTreeView();
			}
		}
		else if(selectedNode==NULL)
		{
			//add to the root node
			object3d* droppedObj=(object3d*)droppedDataObject->getUserData();
		
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
			{
				monoWrapper::mono_engine_appendObject3dToRoot(monoWrapper::mono_engine_getWorld(0), droppedObj);
				////selectedObj->appendChild(droppedObj);

				//geTreeNode* dtv=(geTreeNode*)droppedDataObject;
				//geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
				//dtv_parent->removeTVChild(dtv);

				//((geTreeNode*)droppedDataObject)->setXOffset(0.0f);
				//m_cGameObjectsTreeView.getRoot()->appnendTVChild((geTreeNode*)droppedDataObject);
				//((geTreeNode*)droppedDataObject)->setNodeSprite(&m_cszSprites[1]);
				//rootNode->traverseSetWidth(m_cSize.x);
				//m_cGameObjectsTreeView.refreshTreeView();
			}

		}
	}
	else if(dropObject->getSourcePtr()==EditorApp::getScenePropertyEditor())
	{
		if(droppedDataObject->getParent() && droppedDataObject->getParent()==EditorApp::getScenePropertyEditor()->getAnimationParentNode())
		{
			gxAnimationSet* animSet=(gxAnimationSet*)droppedDataObject->getUserData();
			geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y);
			if(selectedNode)
			{
				object3d* selectedObj=(object3d*)selectedNode->getUserData();
				gxAnimation* animationController = selectedObj->createAnimationController();	//wont create new if there is already an animatiion controller exists
				animationController->appendAnimationSet(animSet);
				selectedObj->applyAnimationSetRecursive(animationController->getAnimationSetList()->size()-1);
				//animationController->play(animationController->getAnimationSetList()->size()-1);
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

	geTreeNode* newtvNode = new geTreeNode(parentNode, name, sprite);
	//assetUserData* userData = new assetUserData(2, "", obj);
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

	//assetUserData* userdata=(assetUserData*)((geTreeNode*)parent)->getUserData();
	//GE_DELETE(userdata);
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		destroyTVUserData(tvnode);
	}
}

void gearSceneHierarchy::onAppendToWorld(gxWorld* world, object3d* obj)
{
	//geTreeNode* rootNode = m_cGameObjectsTreeView.getRoot();
	//createTVNode(rootNode, obj, obj->getName());
	//rootNode->traverseSetWidth(m_cSize.x);
	//m_cGameObjectsTreeView.refreshTreeView();
}

void gearSceneHierarchy::onRemoveFromWorld(gxWorld* world, object3d* obj)
{

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
				//geTreeNode* parentNode = (geTreeNode*)selectedNode->getParent();
				//parentNode->removeTVChild(selectedNode);
				//destroyTVUserData(selectedNode);
				//GE_DELETE(selectedNode);
				//m_cGameObjectsTreeView.resetSelectedNodePtr();

				GE_DELETE(obj);
				EditorApp::getSceneWorldEditor()->selectedObject3D(NULL);
				EditorApp::getScenePropertyEditor()->populatePropertyOfObject(NULL);
			}
		}
	}

	return geWindow::onKeyDown(charValue, flag);
}

bool gearSceneHierarchy::onKeyUp(int charValue, int flag)
{
	return geWindow::onKeyUp(charValue, flag);
}

void gearSceneHierarchy::onButtonClicked(geGUIBase* btn)
{
	if(m_pCreateToolBarDropMenuBtnPtr==btn)
	{
		HMENU hPopupMenu = CreatePopupMenu();
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004003, "Directional Light");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004002, "Point Light");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
		int disableFlag = (selectedNode)?0:MF_DISABLED;
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | disableFlag, 0x00004001, "Create Object on Selected Node");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004000, "Create Object");

		POINT pt;
		pt.x=btn->getPositionOnScreen().x;
		pt.y=-btn->getPositionOnScreen().y;
		ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
		TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);
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
	}
}