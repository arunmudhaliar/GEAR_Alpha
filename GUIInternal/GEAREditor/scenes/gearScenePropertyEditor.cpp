#include "gearScenePropertyEditor.h"
#include "../EditorApp.h"

gearScenePropertyEditor::gearScenePropertyEditor():
geWindow("Property Editor")
{
	m_pObject3dPropertyNode=NULL;
	m_pTransformPropertyNode=NULL;
	m_pMaterialPropertyNode=NULL;

	m_pObject3dParentNode=NULL;
	m_pMaterialParent=NULL;
	m_pTransformParentNode=NULL;
}

gearScenePropertyEditor::~gearScenePropertyEditor()
{
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

	geTreeNode* rootNode=m_cPropertiesTreeView.getRoot();

	m_pObject3dParentNode = new geTreeNode(rootNode, "Object3d", &m_cszSprites[0], 0);
	m_pObject3dPropertyNode = new gePropertyObject3d(m_pObject3dParentNode, "", NULL);
	m_pTransformParentNode = new geTreeNode(rootNode, "Transform", &m_cszSprites[1], 0);
	m_pTransformPropertyNode = new gePropertyTransform(m_pTransformParentNode, "", NULL);
	m_pMaterialParent = new geTreeNode(rootNode, "Material", &m_cszSprites[2], 0);

	rootNode->removeChild(m_pObject3dParentNode);
	rootNode->removeChild(m_pTransformParentNode);
	rootNode->removeChild(m_pMaterialParent);
}

void gearScenePropertyEditor::onDraw()
{
	m_cPropertiesTreeView.draw();
}

void gearScenePropertyEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

void gearScenePropertyEditor::populatePropertyOfObject(object3d* obj)
{
	geTreeNode* rootNode=m_cPropertiesTreeView.getRoot();

	rootNode->removeChild(m_pObject3dParentNode);
	rootNode->removeChild(m_pTransformParentNode);
	rootNode->removeChild(m_pMaterialParent);

	if(obj==NULL)
	{
		m_cPropertiesTreeView.refreshTreeView();
		return;
	}

	m_pObject3dPropertyNode->populatePropertyOfObject(obj);
	rootNode->appnendChild(m_pObject3dParentNode);

	m_pTransformPropertyNode->populatePropertyOfTransform(obj);
	rootNode->appnendChild(m_pTransformParentNode);

	if(m_pMaterialParent)
		m_pMaterialParent->destroyAllChilds();
	if(obj->getID()==100)	//mesh
	{
		gxMesh* mesh=(gxMesh*)obj;

		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gePropertyMaterial* materialProperty = new gePropertyMaterial(m_pMaterialParent, "", NULL, mesh->getTriInfo(x)->getMaterial());
		}

		rootNode->appnendChild(m_pMaterialParent);
	}
	m_cPropertiesTreeView.refreshTreeView();
}