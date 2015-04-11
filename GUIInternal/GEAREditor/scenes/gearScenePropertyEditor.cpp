#include "gearScenePropertyEditor.h"
#include "../EditorApp.h"

gearScenePropertyEditor::stMonoScriptTVNode::stMonoScriptTVNode()
{
	m_pMonoScriptParentNode=NULL;
	m_pSettingsMonoScript=NULL;
}

gearScenePropertyEditor::stMonoScriptTVNode::~stMonoScriptTVNode()
{
	GX_DELETE(m_pMonoScriptParentNode);
}

/////////////////////////////////////////////////////////////////////////////

gearScenePropertyEditor::gearScenePropertyEditor(geFontManager* fontmanager):
geWindow("Property Editor", fontmanager)
{
	m_pObject3dParentNode=NULL;
	m_pMaterialParent=NULL;
	m_pTransformParentNode=NULL;
	m_pAnimationParentNode=NULL;
	m_pSaveMetaDataParentNode=NULL;
	m_pLightParentNode=NULL;
	m_pAddComponentParentNode=NULL;
	m_pCameraParentNode = NULL;

	m_pPostProcessorBlurShaderNode=NULL;

	m_pObject3dPropertyNode=NULL;
	m_pTransformPropertyNode=NULL;
	m_pMaterialPropertyNode=NULL;
	m_pSaveMetaDataPropertyNode=NULL;
	m_pLightPropertyNode=NULL;
	m_pAddComponentProperty=NULL;
	m_pCameraPropertyNode=NULL;

	m_pBlurProcessorPropertyNode=NULL;

	m_pOpenOnEditorParentNode=NULL;
	m_pPropertyOpenOnEditor=NULL;

	m_pOctreeParentNode = NULL;
	m_pPropertyOctree = NULL;

	m_pLayersParentNode = NULL;
	m_pPropertyLayers = NULL;

	m_pFogParentNode = NULL;
	m_pSettingsFog = NULL;
    m_pPropertiesTreeView = new geTreeView(fontmanager);
}

gearScenePropertyEditor::~gearScenePropertyEditor()
{
	removeAllProperties();

	GE_DELETE(m_pObject3dParentNode);
	GE_DELETE(m_pTransformParentNode);
	GE_DELETE(m_pMaterialParent);
	GE_DELETE(m_pAnimationParentNode);
	GE_DELETE(m_pSaveMetaDataParentNode);
	GE_DELETE(m_pLightParentNode);
	GE_DELETE(m_pAddComponentParentNode);
	GE_DELETE(m_pPostProcessorBlurShaderNode);
	GE_DELETE(m_pCameraParentNode);
	GE_DELETE(m_pOpenOnEditorParentNode);
	GE_DELETE(m_pOctreeParentNode);
	GE_DELETE(m_pLayersParentNode);
	GE_DELETE(m_pFogParentNode);
    
    GE_DELETE(m_pPropertiesTreeView);
}

void gearScenePropertyEditor::onCreate()
{
	m_pPropertiesTreeView->create(m_pRenderer, this, "propertyeditorTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(68, 488, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(68, 152, 16, 16);
	m_cszSprites[2].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[2].setClip(68, 110, 16, 16);
	m_cszSprites[3].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[3].setClip(404, 362, 16, 16);
	m_cszSprites[4].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[4].setClip(151, 48, 16, 16);
	m_cszSprites[5].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[5].setClip(110, 404, 16, 16);
	m_cszSprites[6].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[6].setClip(26, 298, 16, 16);
	m_cszSprites[7].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[7].setClip(130, 404, 16, 16);
	m_cszSprites[8].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[8].setClip(110, 342, 16, 16);
	m_cszSprites[9].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[9].setClip(257, 6, 16, 16);
	m_cszSprites[10].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[10].setClip(277, 27, 16, 16);

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();

	m_pObject3dParentNode = new geTreeNode(m_pRenderer, rootNode, "Object3d", &m_cszSprites[0], 0);
	m_pObject3dPropertyNode = new gePropertyObject3d(m_pRenderer, m_pObject3dParentNode, "", NULL, m_pFontManagerPtr);
	m_pTransformParentNode = new geTreeNode(m_pRenderer, rootNode, "Transform", &m_cszSprites[1], 0);
	m_pTransformPropertyNode = new gePropertyTransform(m_pRenderer, m_pTransformParentNode, "", NULL, m_pFontManagerPtr);
	m_pMaterialParent = new geTreeNode(m_pRenderer, rootNode, "Material", &m_cszSprites[2], 0);
	m_pAnimationParentNode  = new geTreeNode(m_pRenderer, rootNode, "Animation", &m_cszSprites[3], 0);
	m_pSaveMetaDataParentNode = new geTreeNode(m_pRenderer, rootNode, "Save MetaData", &m_cszSprites[4], 0);
	m_pSaveMetaDataPropertyNode = new gePropertySaveMetaData(m_pRenderer, m_pSaveMetaDataParentNode, "", NULL, m_pFontManagerPtr);
	m_pLightParentNode = new geTreeNode(m_pRenderer, rootNode, "Light", &m_cszSprites[5], 0);
	m_pLightPropertyNode = new gePropertyLight(m_pRenderer, m_pLightParentNode, "", NULL, m_pFontManagerPtr);
	m_pAddComponentParentNode = new geTreeNode(m_pRenderer, rootNode, "Add Component", &m_cszSprites[8], 0);
	m_pAddComponentProperty = new gePropertyAddComponent(m_pRenderer, m_pAddComponentParentNode, "", NULL, m_pFontManagerPtr);

	m_pCameraParentNode = new geTreeNode(m_pRenderer, rootNode, "Camera", &m_cszSprites[7], 0);
	m_pCameraPropertyNode = new gePropertyCamera(m_pRenderer, m_pCameraParentNode, "", NULL, m_pFontManagerPtr);

	m_pPostProcessorBlurShaderNode = new geTreeNode(m_pRenderer, rootNode, "Blur Processor", &m_cszSprites[5], 0);
	m_pBlurProcessorPropertyNode = new gePropertyBlurProcessor(m_pRenderer, m_pPostProcessorBlurShaderNode, "", NULL, m_pFontManagerPtr);

	m_pOpenOnEditorParentNode = new geTreeNode(m_pRenderer, rootNode, "Script Editor", &m_cszSprites[5], 0);
	m_pPropertyOpenOnEditor = new gePropertyOpenOnEditor(m_pRenderer, m_pOpenOnEditorParentNode, "", NULL, m_pFontManagerPtr);

	m_pOctreeParentNode = new geTreeNode(m_pRenderer, rootNode, "Octree", &m_cszSprites[5], 0);
	m_pPropertyOctree = new gePropertyOctree(m_pRenderer, m_pOctreeParentNode, "", NULL, m_pFontManagerPtr);

	m_pLayersParentNode = new geTreeNode(m_pRenderer, rootNode, "Layers", &m_cszSprites[9], 0);
	m_pPropertyLayers = new gePropertyLayers(m_pRenderer, m_pLayersParentNode, "", NULL, m_pFontManagerPtr);

	m_pFogParentNode = new geTreeNode(m_pRenderer, rootNode, "Fog", &m_cszSprites[9], 0);
	m_pSettingsFog = new geSettingsFog(m_pRenderer, m_pFogParentNode, "", NULL, monoWrapper::mono_engine_getWorld(0)->getRenderer()->getFog(), m_pFontManagerPtr);

	removeAllProperties();
}

void gearScenePropertyEditor::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_pPropertiesTreeView->draw();
}

void gearScenePropertyEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearScenePropertyEditor::onMouseMove(float x, float y, int flag)
{
#if DEPRECATED
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_cPropertiesTreeView.getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(m_cPropertiesTreeView.getScrollBar()->isScrollBarGrabbed())
			return true;

		if(selectedNode->getParent()==m_pAnimationParentNode)
		{
			std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();
			newlist->push_back(selectedNode);

			MDataObject* dataObject = new MDataObject(newlist, this);
			MDropSource* dropSource = new MDropSource();

			DWORD lpd=0;
			HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

			dataObject->Release();
			dropSource->Release();
		}
		return true;
	}
#endif
	return true;
}

void gearScenePropertyEditor::removeAllProperties()
{
	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();

	rootNode->removeTVChild(m_pObject3dParentNode);
	rootNode->removeTVChild(m_pTransformParentNode);
	rootNode->removeTVChild(m_pMaterialParent);
	rootNode->removeTVChild(m_pAnimationParentNode);
	rootNode->removeTVChild(m_pSaveMetaDataParentNode);
	rootNode->removeTVChild(m_pLightParentNode);
	rootNode->removeTVChild(m_pAddComponentParentNode);
	rootNode->removeTVChild(m_pPostProcessorBlurShaderNode);
	rootNode->removeTVChild(m_pCameraParentNode);

	rootNode->removeTVChild(m_pOpenOnEditorParentNode);
	rootNode->removeTVChild(m_pOctreeParentNode);
	rootNode->removeTVChild(m_pLayersParentNode);
	rootNode->removeTVChild(m_pFogParentNode);

	for(std::vector<stMonoScriptTVNode*>::iterator it = m_vMonoScript.begin(); it != m_vMonoScript.end(); ++it)
	{
		stMonoScriptTVNode* obj = *it;
		rootNode->removeTVChild(obj->m_pMonoScriptParentNode);
		GX_DELETE(obj);
	}
	m_vMonoScript.clear();

	//material
	if(m_pMaterialParent)
		m_pMaterialParent->destroyAllTVChilds();

	//animation
	if(m_pAnimationParentNode)
		m_pAnimationParentNode->destroyAllTVChilds();
}

void gearScenePropertyEditor::populatePropertyOfBlurShader(gxHWShader* blurShader)
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	//blur
	m_pBlurProcessorPropertyNode->populatePropertyOfBlurShader(blurShader);
	rootNode->appnendTVChild(m_pPostProcessorBlurShaderNode);

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfOpenInEditor()
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	rootNode->appnendTVChild(m_pOpenOnEditorParentNode);

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfOctree()
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	rootNode->appnendTVChild(m_pOctreeParentNode);

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfLayers()
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	rootNode->appnendTVChild(m_pLayersParentNode);

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populateSettingsOfFog()
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	rootNode->appnendTVChild(m_pFogParentNode);

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfObject(object3d* obj)
{
	removeAllProperties();

	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();
	if(obj==NULL)
	{
		m_pPropertiesTreeView->refreshTreeView();
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
	if(obj->getID()==OBJECT3D_MESH || obj->getID()==OBJECT3D_SKINNED_MESH)	//mesh
	{
		gxMesh* mesh=(gxMesh*)obj;

		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			//if(!mesh->getTriInfo(x)->getMaterial())
			//	continue;
			gePropertyMaterial* materialProperty = new gePropertyMaterial(m_pRenderer, m_pMaterialParent, "", NULL, mesh->getTriInfo(x), m_pFontManagerPtr);
		}

		rootNode->appnendTVChild(m_pMaterialParent);
	}
	else if(obj->getID()==OBJECT3D_LIGHT)
	{
		m_pLightPropertyNode->populatePropertyOfLight(obj);
		rootNode->appnendTVChild(m_pLightParentNode);
	}
	else if(obj->getID()==OBJECT3D_CAMERA)
	{
		m_pCameraPropertyNode->populatePropertyOfCamera(obj);
		rootNode->appnendTVChild(m_pCameraParentNode);
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

			gePropertyAnimationSet* pAnimationSetNode  = new gePropertyAnimationSet(m_pRenderer, m_pAnimationParentNode, obj, animationSet, &m_cszSprites[6], m_pFontManagerPtr);
			pAnimationSetNode->setUserData(animationSet);
		}
		rootNode->appnendTVChild(m_pAnimationParentNode);
	}

	m_pSaveMetaDataPropertyNode->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(m_pSaveMetaDataParentNode);
	m_pSaveMetaDataParentNode->closeNode();

	////hack
	//m_pBlurProcessorPropertyNode->populatePropertyOfBlurShader(NULL);
	//rootNode->appnendTVChild(m_pPostProcessorBlurShaderNode);
	////

	//components
	m_pAddComponentProperty->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(m_pAddComponentParentNode);
	m_pAddComponentParentNode->closeNode();

	//mono scripts
	if(obj->getMonoScriptInstanceCount())
	{
		for(int x=0;x<obj->getMonoScriptInstanceCount();x++)
		{
			monoScriptObjectInstance* monoInstance = obj->getMonoScriptInstance(x);
			stMonoScriptTVNode* monoScriptTVNode = new stMonoScriptTVNode();
			monoScriptTVNode->m_pMonoScriptParentNode = new geTreeNode(m_pRenderer, rootNode, monoInstance->getScriptPtr()->getMonoScript().c_str(), &m_cszSprites[10], 0);
			monoScriptTVNode->m_pSettingsMonoScript = new gePropertyScriptComponent(m_pRenderer, monoScriptTVNode->m_pMonoScriptParentNode, "", NULL, m_pFontManagerPtr);
			monoScriptTVNode->m_pMonoScriptParentNode->closeNode();

			monoScriptTVNode->m_pSettingsMonoScript->populatePropertyOfMonoScripts(obj, monoInstance);
			m_vMonoScript.push_back(monoScriptTVNode);
		}
	}

	m_pPropertiesTreeView->refreshTreeView();
	m_pPropertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::updateTransformPropertyOfCurrentSelectedObject()
{
	geTreeNode* rootNode=m_pPropertiesTreeView->getRoot();

	std::vector<geGUIBase*>* controlList=rootNode->getChildControls();
	if(std::find(controlList->begin(), controlList->end(), m_pTransformParentNode)!=controlList->end())
	{
		m_pTransformPropertyNode->updatePropertyView();
	}
}

