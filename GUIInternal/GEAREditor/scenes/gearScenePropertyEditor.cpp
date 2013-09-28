#include "gearScenePropertyEditor.h"
#include "../EditorApp.h"

gearScenePropertyEditor::gearScenePropertyEditor():
geWindow("Property Editor")
{
	m_pObject3dPropertyNode=NULL;
	m_pTransformPropertyNode=NULL;
	m_pMaterialPropertyNode=NULL;
	m_pSaveMetaDataPropertyNode=NULL;

	m_pObject3dParentNode=NULL;
	m_pMaterialParent=NULL;
	m_pTransformParentNode=NULL;
	m_pAnimationParentNode=NULL;
	m_pSaveMetaDataParentNode=NULL;
}

gearScenePropertyEditor::~gearScenePropertyEditor()
{
	geTreeNode* rootNode=m_cPropertiesTreeView.getRoot();
	rootNode->removeTVChild(m_pObject3dParentNode);
	rootNode->removeTVChild(m_pTransformParentNode);
	rootNode->removeTVChild(m_pMaterialParent);
	rootNode->removeTVChild(m_pAnimationParentNode);
	rootNode->removeTVChild(m_pSaveMetaDataParentNode);

	GE_DELETE(m_pObject3dParentNode);
	GE_DELETE(m_pTransformParentNode);
	GE_DELETE(m_pMaterialParent);
	GE_DELETE(m_pAnimationParentNode);
	GE_DELETE(m_pSaveMetaDataParentNode);
}

void gearScenePropertyEditor::onCreate()
{
	m_cPropertiesTreeView.create(this, "propertyeditorTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(68, 488, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(68, 152, 16, 16);
	m_cszSprites[2].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[2].setClip(68, 110, 16, 16);
	m_cszSprites[3].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[3].setClip(404, 362, 16, 16);

	geTreeNode* rootNode=m_cPropertiesTreeView.getRoot();

	m_pObject3dParentNode = new geTreeNode(rootNode, "Object3d", &m_cszSprites[0], 0);
	m_pObject3dPropertyNode = new gePropertyObject3d(m_pObject3dParentNode, "", NULL);
	m_pTransformParentNode = new geTreeNode(rootNode, "Transform", &m_cszSprites[1], 0);
	m_pTransformPropertyNode = new gePropertyTransform(m_pTransformParentNode, "", NULL);
	m_pMaterialParent = new geTreeNode(rootNode, "Material", &m_cszSprites[2], 0);
	m_pAnimationParentNode  = new geTreeNode(rootNode, "Animation", &m_cszSprites[3], 0);
	m_pSaveMetaDataParentNode = new geTreeNode(rootNode, "Save MetaData", &m_cszSprites[0], 0);
	m_pSaveMetaDataPropertyNode = new gePropertySaveMetaData(m_pSaveMetaDataParentNode, "", NULL);

	rootNode->removeTVChild(m_pObject3dParentNode);
	rootNode->removeTVChild(m_pTransformParentNode);
	rootNode->removeTVChild(m_pMaterialParent);
	rootNode->removeTVChild(m_pAnimationParentNode);
	rootNode->removeTVChild(m_pSaveMetaDataParentNode);
}

void gearScenePropertyEditor::onDraw()
{
	m_cPropertiesTreeView.draw();
}

void gearScenePropertyEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearScenePropertyEditor::onMouseMove(float x, float y, int flag)
{
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_cPropertiesTreeView.getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(m_cPropertiesTreeView.getScrollBar()->isScrollBarGrabbed())
			return true;

		if(selectedNode->getParent()==m_pAnimationParentNode)
		{
			MDataObject* dataObject = new MDataObject(selectedNode, this);
			MDropSource* dropSource = new MDropSource();

			DWORD lpd=0;
			HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

			dataObject->Release();
			dropSource->Release();
		}
		return true;
	}

	return true;
}

void gearScenePropertyEditor::populatePropertyOfObject(object3d* obj)
{
	geTreeNode* rootNode=m_cPropertiesTreeView.getRoot();

	rootNode->removeTVChild(m_pObject3dParentNode);
	rootNode->removeTVChild(m_pTransformParentNode);
	rootNode->removeTVChild(m_pMaterialParent);
	rootNode->removeTVChild(m_pAnimationParentNode);
	rootNode->removeTVChild(m_pSaveMetaDataParentNode);

	if(obj==NULL)
	{
		m_cPropertiesTreeView.refreshTreeView();
		return;
	}

	//object3d
	m_pObject3dPropertyNode->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(m_pObject3dParentNode);

	//transform
	m_pTransformPropertyNode->populatePropertyOfTransform(obj);
	rootNode->appnendTVChild(m_pTransformParentNode);

	//material
	if(m_pMaterialParent)
		m_pMaterialParent->destroyAllTVChilds();
	if(obj->getID()==100)	//mesh
	{
		gxMesh* mesh=(gxMesh*)obj;

		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			if(!mesh->getTriInfo(x)->getMaterial())
				continue;
			gePropertyMaterial* materialProperty = new gePropertyMaterial(m_pMaterialParent, "", NULL, mesh->getTriInfo(x)->getMaterial());
		}

		rootNode->appnendTVChild(m_pMaterialParent);
	}

	//animation
	if(m_pAnimationParentNode)
		m_pAnimationParentNode->destroyAllTVChilds();
	if(obj->getAnimationController())
	{
		
		gxAnimation* animationController=obj->getAnimationController();
		std::vector<gxAnimationSet*>* animList=animationController->getAnimationSetList();
		for(std::vector<gxAnimationSet*>::iterator it = animList->begin(); it != animList->end(); ++it)
		{
			gxAnimationSet* animationSet = *it;

			gePropertyAnimationSet* pAnimationSetNode  = new gePropertyAnimationSet(m_pAnimationParentNode, animationSet->getAnimationName(), NULL);
			pAnimationSetNode->setUserData(animationSet);
		}
		rootNode->appnendTVChild(m_pAnimationParentNode);
	}

	m_pSaveMetaDataPropertyNode->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(m_pSaveMetaDataParentNode);

	m_cPropertiesTreeView.refreshTreeView();
	m_cPropertiesTreeView.resetSelectedNodePtr();
}