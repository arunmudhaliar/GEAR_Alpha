#include "gearSceneHierarchy.h"
#include "../EditorApp.h"
#include "assetUserData.h"

gearSceneHierarchy::gearSceneHierarchy():
geWindow("Hierarchy")
{
	//m_pSceneWorldEditorPtr = NULL;
}

gearSceneHierarchy::~gearSceneHierarchy()
{
	destroyTVUserData(m_cGameObjectsTreeView.getRoot());
}

void gearSceneHierarchy::onCreate()
{
	m_cGameObjectsTreeView.create(this, "gameObjectsTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(88, 382, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(68, 488, 16, 16);

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
	EditorApp::getSceneWorldEditor()->selectedObject3D((object3d*)((assetUserData*)tvnode->getUserData())->getAssetObjectPtr());

	EditorApp::getScenePropertyEditor()->populatePropertyOfObject((object3d*)((assetUserData*)tvnode->getUserData())->getAssetObjectPtr());
}

void gearSceneHierarchy::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
		return;

	geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(m_cGameObjectsTreeView.getScrollBar()->isScrollBarGrabbed())
			return;
		MDataObject* dataObject = new MDataObject(selectedNode, this);
		MDropSource* dropSource = new MDropSource();

		DWORD lpd=0;
		HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

		dataObject->Release();
		dropSource->Release();
		return;
	}

	geWindow::onMouseMove(x, y, flag);
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

		if (util::GE_IS_EXTENSION(absolutePath, ".fbx") || util::GE_IS_EXTENSION(absolutePath, ".FBX"))
		{
			object3d* obj = monoWrapper::mono_engine_loadAndAppendFBX(EditorApp::getSceneWorldEditor()->getMainWorld(), absolutePath);
			createTVNode(rootNode, obj, droppedDataObject->getName());
		}

		rootNode->traverseSetWidth(m_cSize.x);
		m_cGameObjectsTreeView.refreshTreeView();
	}
	else if(dropObject->getSourcePtr()==this)
	{
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y-getTopMarginOffsetHeight());
		if(selectedNode && !droppedDataObject->isNodeExistsInTree(selectedNode))
		{
			object3d* selectedObj=(object3d*)((assetUserData*)selectedNode->getUserData())->getAssetObjectPtr();
			object3d* droppedObj=(object3d*)((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetObjectPtr();
		
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
			{
				selectedObj->appendChild(droppedObj);

				geTreeNode* dtv=(geTreeNode*)droppedDataObject;
				geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
				dtv_parent->removeChild(dtv);

				((geTreeNode*)droppedDataObject)->setXOffset(20.0f);
				selectedNode->appnendChild((geTreeNode*)droppedDataObject);
				((geTreeNode*)droppedDataObject)->setNodeSprite(&m_cszSprites[0]);
				rootNode->traverseSetWidth(m_cSize.x);
				m_cGameObjectsTreeView.refreshTreeView();
			}
		}
		else if(selectedNode==NULL)
		{
			//add to the root node
			object3d* droppedObj=(object3d*)((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetObjectPtr();
		
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), droppedObj))
			{
				monoWrapper::mono_engine_appendObject3dToRoot(monoWrapper::mono_engine_getWorld(0), droppedObj);
				//selectedObj->appendChild(droppedObj);

				geTreeNode* dtv=(geTreeNode*)droppedDataObject;
				geTreeNode* dtv_parent=(geTreeNode*)dtv->getParent();
				dtv_parent->removeChild(dtv);

				((geTreeNode*)droppedDataObject)->setXOffset(0.0f);
				m_cGameObjectsTreeView.getRoot()->appnendChild((geTreeNode*)droppedDataObject);
				((geTreeNode*)droppedDataObject)->setNodeSprite(&m_cszSprites[1]);
				rootNode->traverseSetWidth(m_cSize.x);
				m_cGameObjectsTreeView.refreshTreeView();
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
	assetUserData* userData = new assetUserData(2, "", obj);
	newtvNode->setUserData(userData);

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

	assetUserData* userdata=(assetUserData*)((geTreeNode*)parent)->getUserData();
	GE_DELETE(userdata);
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		destroyTVUserData(tvnode);
	}
}

bool gearSceneHierarchy::onKeyDown(int charValue, int flag)
{
	if(charValue==46)	//DEL key
	{
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
		if(selectedNode)
		{
			object3d* obj=(object3d*)((assetUserData*)selectedNode->getUserData())->getAssetObjectPtr();
			if(monoWrapper::mono_engine_removeObject3d(monoWrapper::mono_engine_getWorld(0), obj))
			{
				geTreeNode* parentNode = (geTreeNode*)selectedNode->getParent();
				parentNode->removeChild(selectedNode);
				destroyTVUserData(selectedNode);
				GE_DELETE(selectedNode);
				m_cGameObjectsTreeView.resetSelectedNodePtr();

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