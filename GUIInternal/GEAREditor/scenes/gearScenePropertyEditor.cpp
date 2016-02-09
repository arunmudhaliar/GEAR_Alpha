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
	m_pPropertiesTreeView->create(rendererGUI, this, "propertyeditorTV", this);

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

	m_pObject3dParentNode = new geTreeNode(rendererGUI, rootNode, "Object3d", &m_cszSprites[0], 0);
	m_pObject3dPropertyNode = new gePropertyObject3d(rendererGUI, m_pObject3dParentNode, "", NULL, fontManagerGUI);
	m_pTransformParentNode = new geTreeNode(rendererGUI, rootNode, "Transform", &m_cszSprites[1], 0);
	m_pTransformPropertyNode = new gePropertyTransform(rendererGUI, m_pTransformParentNode, "", NULL, fontManagerGUI);
	m_pMaterialParent = new geTreeNode(rendererGUI, rootNode, "Material", &m_cszSprites[2], 0);
	m_pAnimationParentNode  = new geTreeNode(rendererGUI, rootNode, "Animation", &m_cszSprites[3], 0);
	m_pSaveMetaDataParentNode = new geTreeNode(rendererGUI, rootNode, "Save MetaData", &m_cszSprites[4], 0);
	m_pSaveMetaDataPropertyNode = new gePropertySaveMetaData(rendererGUI, m_pSaveMetaDataParentNode, "", NULL, fontManagerGUI);
	m_pLightParentNode = new geTreeNode(rendererGUI, rootNode, "Light", &m_cszSprites[5], 0);
	m_pLightPropertyNode = new gePropertyLight(rendererGUI, m_pLightParentNode, "", NULL, fontManagerGUI);
	m_pAddComponentParentNode = new geTreeNode(rendererGUI, rootNode, "Add Component", &m_cszSprites[8], 0);
	m_pAddComponentProperty = new gePropertyAddComponent(rendererGUI, m_pAddComponentParentNode, "", NULL, fontManagerGUI);

	m_pCameraParentNode = new geTreeNode(rendererGUI, rootNode, "Camera", &m_cszSprites[7], 0);
	m_pCameraPropertyNode = new gePropertyCamera(rendererGUI, m_pCameraParentNode, "", NULL, fontManagerGUI);

	m_pPostProcessorBlurShaderNode = new geTreeNode(rendererGUI, rootNode, "Blur Processor", &m_cszSprites[5], 0);
	m_pBlurProcessorPropertyNode = new gePropertyBlurProcessor(rendererGUI, m_pPostProcessorBlurShaderNode, "", NULL, fontManagerGUI);

	m_pOpenOnEditorParentNode = new geTreeNode(rendererGUI, rootNode, "Script Editor", &m_cszSprites[5], 0);
	m_pPropertyOpenOnEditor = new gePropertyOpenOnEditor(rendererGUI, m_pOpenOnEditorParentNode, "", NULL, fontManagerGUI);

	m_pOctreeParentNode = new geTreeNode(rendererGUI, rootNode, "Octree", &m_cszSprites[5], 0);
	m_pPropertyOctree = new gePropertyOctree(rendererGUI, m_pOctreeParentNode, "", NULL, fontManagerGUI);

	m_pLayersParentNode = new geTreeNode(rendererGUI, rootNode, "Layers", &m_cszSprites[9], 0);
	m_pPropertyLayers = new gePropertyLayers(rendererGUI, m_pLayersParentNode, "", NULL, fontManagerGUI);

	m_pFogParentNode = new geTreeNode(rendererGUI, rootNode, "Fog", &m_cszSprites[9], 0);
	m_pSettingsFog = new geSettingsFog(rendererGUI, m_pFogParentNode, "", NULL, monoWrapper::mono_engine_getWorld(0)->getRenderer()->getFog(), fontManagerGUI);

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
//#if DEPRECATED
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_pPropertiesTreeView->getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(m_pPropertiesTreeView->getScrollBar()->isScrollBarGrabbed())
			return true;

		if(selectedNode->getParent()==m_pAnimationParentNode)
		{
			std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();
			newlist->push_back(selectedNode);

			MDropData* dataObject = new MDropData(newlist, this);
			doDragDropSynchronous(dataObject);

			//MDataObject* dataObject = new MDataObject(newlist, this);
			//MDropSource* dropSource = new MDropSource();

			//DWORD lpd=0;
			//HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

			//dataObject->Release();
			//dropSource->Release();
		}
	}
//#endif
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
			gePropertyMaterial* materialProperty = new gePropertyMaterial(rendererGUI, m_pMaterialParent, "", NULL, mesh->getTriInfo(x), fontManagerGUI);
            UNUSED(materialProperty);
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

			gePropertyAnimationSet* pAnimationSetNode  = new gePropertyAnimationSet(rendererGUI, m_pAnimationParentNode, obj, animationSet, &m_cszSprites[6], fontManagerGUI);
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
			monoScriptTVNode->m_pMonoScriptParentNode = new geTreeNode(rendererGUI, rootNode, monoInstance->getScriptPtr()->getScriptFileName().c_str(), &m_cszSprites[10], 0);
			monoScriptTVNode->m_pSettingsMonoScript = new gePropertyScriptComponent(rendererGUI, monoScriptTVNode->m_pMonoScriptParentNode, "", NULL, fontManagerGUI);
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

