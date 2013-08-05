#include "gearSceneHierarchy.h"
#include "../EditorApp.h"
#include "assetUserData.h"
#include "../../../GEAREngine/src/core/gxMetaStructures.h"

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

bool gearSceneHierarchy::onMouseMove(float x, float y, int flag)
{
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
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

void read3dFile2(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;
		if(objID==0 || objID==1)
		{
			tempObj = new object3d(objID);
		}
		else if(objID==100)
		{
			tempObj = new gxMesh();
		}

		tempObj->read(file);
		obj->appendChild(tempObj);
		read3dFile2(file, tempObj);
	}
}

void loadMaterialFromObject3d(gxWorld* world, object3d* obj3d)
{
	if(obj3d->getID()==100)
	{
		gxMesh* mesh = (gxMesh*)obj3d;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triInfo = mesh->getTriInfo(x);
			int materialCRC=triInfo->getMaterialCRCFromFileReadInfo();
			char crcFile[1024];
			sprintf(crcFile, "%s/MetaData/%x", EditorApp::getProjectHomeDirectory(), materialCRC);

			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				gxMaterial* material = new gxMaterial();
				material->read(file_meta);

				//check if the material name already exists in our list or not
				std::vector<gxMaterial*>* materialList = world->getMaterialList();
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
				world->getMaterialList()->push_back(triInfo->getMaterial());
			}
		}
	}

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadMaterialFromObject3d(world, childobj);
	}
}

void loadAnmationFromObject3d(gxWorld* world, object3d* obj3d)
{
	gxAnimation* animationController = obj3d->getAnimationController();
	if(animationController)
	{
		std::vector<gxAnimationSet*>* animationSetList=animationController->getAnimationSetList();
		for(std::vector<gxAnimationSet*>::iterator it = animationSetList->begin(); it != animationSetList->end(); ++it)
		{
			gxAnimationSet* animationSet = *it;
			world->getAnimationSetList()->push_back(animationSet);
		}
	}

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		loadAnmationFromObject3d(world, childobj);
	}
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
			object3d* obj = NULL;//monoWrapper::mono_engine_loadAndAppendFBX(EditorApp::getSceneWorldEditor()->getMainWorld(), absolutePath);
			char metaInfoFileName[256];
			sprintf(metaInfoFileName, "%s.meta",absolutePath);

			gxFile metaInfoFile;
			if(metaInfoFile.OpenFile(metaInfoFileName))
			{
				int crc=0;
				metaInfoFile.Read(crc);
				metaInfoFile.CloseFile();

				char crcFile[1024];
				sprintf(crcFile, "%s/MetaData/%x", EditorApp::getProjectHomeDirectory(), crc);

				gxFile file_meta;
				if(file_meta.OpenFile(crcFile))
				{
					stMetaHeader metaHeader;
					file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

					int objID=0;
					file_meta.Read(objID);

					object3d* tempObj=NULL;
					if(objID==0 || objID==1)
					{
						tempObj = new object3d(objID);
						tempObj->read(file_meta);
						read3dFile2(file_meta, tempObj);
						obj=tempObj;
						loadMaterialFromObject3d(EditorApp::getSceneWorldEditor()->getMainWorld(), obj);
						loadAnmationFromObject3d(EditorApp::getSceneWorldEditor()->getMainWorld(), obj);
						obj->transformationChangedf();
						EditorApp::getSceneWorldEditor()->getMainWorld()->appendChild(obj);
					}
					file_meta.CloseFile();
				}
			}

			createTVNode(rootNode, obj, droppedDataObject->getName());
		}

		rootNode->traverseSetWidth(m_cSize.x);
		m_cGameObjectsTreeView.refreshTreeView();
	}
	else if(dropObject->getSourcePtr()==this)
	{
		geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y/*-getTopMarginOffsetHeight()*/);
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
	else if(dropObject->getSourcePtr()==EditorApp::getScenePropertyEditor())
	{
		if(droppedDataObject->getParent() && droppedDataObject->getParent()==EditorApp::getScenePropertyEditor()->getAnimationParentNode())
		{
			gxAnimationSet* animSet=(gxAnimationSet*)((geTreeNode*)droppedDataObject)->getUserData();

			geTreeNode* selectedNode=m_cGameObjectsTreeView.getTVNode(x, y);
			if(selectedNode)
			{
				object3d* selectedObj=(object3d*)((assetUserData*)selectedNode->getUserData())->getAssetObjectPtr();
				gxAnimation* animationController = selectedObj->createAnimationController();	//wont create new if there is already an animatiion controller exists
				animationController->appendAnimationSet(animSet);
				selectedObj->applyAnimationSetRecursive(animationController->getAnimationSetList()->size()-1);
				animationController->setActiveAnimationSet(animationController->getAnimationSetList()->size()-1);
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