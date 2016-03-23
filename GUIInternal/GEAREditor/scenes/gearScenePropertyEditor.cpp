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
	object3dParentTreeNode=NULL;
	materialParentTreeNode=NULL;
	transformParentTreeNode=NULL;
	animationParentTreeNode=NULL;
	saveMetaDataParentTreeNode=NULL;
	lightParentTreeNode=NULL;
	addComponentParentTreeNode=NULL;
	cameraParentTreeNode = NULL;

	postProcessorBlurShaderParentTreeNode=NULL;

	object3dProperty=NULL;
	transformProperty=NULL;
	materialProperty=NULL;
	saveMetaDataProperty=NULL;
	lightProperty=NULL;
	addComponentProperty=NULL;
	cameraProperty=NULL;

	blurPostProcessorProperty=NULL;

	openOnEditorParentTreeNode=NULL;
	openOnEditorProperty=NULL;

	octreeParentTreeNode = NULL;
	octreeProperty = NULL;

	layersParentTreeNode = NULL;
	layersProperty = NULL;

	fogParentTreeNode = NULL;
	fogProperty = NULL;
    propertiesTreeView = new geTreeView(fontmanager);
}

gearScenePropertyEditor::~gearScenePropertyEditor()
{
	removeAllProperties();

	GE_DELETE(object3dParentTreeNode);
	GE_DELETE(transformParentTreeNode);
	GE_DELETE(materialParentTreeNode);
	GE_DELETE(animationParentTreeNode);
	GE_DELETE(saveMetaDataParentTreeNode);
	GE_DELETE(lightParentTreeNode);
	GE_DELETE(addComponentParentTreeNode);
	GE_DELETE(postProcessorBlurShaderParentTreeNode);
	GE_DELETE(cameraParentTreeNode);
	GE_DELETE(openOnEditorParentTreeNode);
	GE_DELETE(octreeParentTreeNode);
	GE_DELETE(layersParentTreeNode);
	GE_DELETE(fogParentTreeNode);
    
    GE_DELETE(propertiesTreeView);
}

void gearScenePropertyEditor::onCreate(float cx, float cy)
{
	propertiesTreeView->create(rendererGUI, this, "propertyeditorTV", this);

	spriteArray[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[0].setClip(68, 488, 16, 16);
	spriteArray[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[1].setClip(68, 152, 16, 16);
	spriteArray[2].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[2].setClip(68, 110, 16, 16);
	spriteArray[3].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[3].setClip(404, 362, 16, 16);
	spriteArray[4].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[4].setClip(151, 48, 16, 16);
	spriteArray[5].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[5].setClip(110, 404, 16, 16);
	spriteArray[6].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[6].setClip(26, 298, 16, 16);
	spriteArray[7].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[7].setClip(130, 404, 16, 16);
	spriteArray[8].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[8].setClip(110, 342, 16, 16);
	spriteArray[9].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[9].setClip(257, 6, 16, 16);
	spriteArray[10].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	spriteArray[10].setClip(277, 27, 16, 16);

	geTreeNode* rootNode=propertiesTreeView->getRoot();

	object3dParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Object3d", &spriteArray[0], 0);
	object3dProperty = new gePropertyObject3d(rendererGUI, object3dParentTreeNode, "", NULL, fontManagerGUI);
	transformParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Transform", &spriteArray[1], 0);
	transformProperty = new gePropertyTransform(rendererGUI, transformParentTreeNode, "", NULL, fontManagerGUI);
	materialParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Material", &spriteArray[2], 0);
	animationParentTreeNode  = new geTreeNode(rendererGUI, rootNode, "Animation", &spriteArray[3], 0);
	saveMetaDataParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Save MetaData", &spriteArray[4], 0);
	saveMetaDataProperty = new gePropertySaveMetaData(rendererGUI, saveMetaDataParentTreeNode, "", NULL, fontManagerGUI);
	lightParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Light", &spriteArray[5], 0);
	lightProperty = new gePropertyLight(rendererGUI, lightParentTreeNode, "", NULL, fontManagerGUI);
	addComponentParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Add Component", &spriteArray[8], 0);
	addComponentProperty = new gePropertyAddComponent(rendererGUI, addComponentParentTreeNode, "", NULL, fontManagerGUI);

	cameraParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Camera", &spriteArray[7], 0);
	cameraProperty = new gePropertyCamera(rendererGUI, cameraParentTreeNode, "", NULL, fontManagerGUI);

	postProcessorBlurShaderParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Blur Processor", &spriteArray[5], 0);
	blurPostProcessorProperty = new gePropertyBlurProcessor(rendererGUI, postProcessorBlurShaderParentTreeNode, "", NULL, fontManagerGUI);

	openOnEditorParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Script Editor", &spriteArray[5], 0);
	openOnEditorProperty = new gePropertyOpenOnEditor(rendererGUI, openOnEditorParentTreeNode, "", NULL, fontManagerGUI);

	octreeParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Octree", &spriteArray[5], 0);
	octreeProperty = new gePropertyOctree(rendererGUI, octreeParentTreeNode, "", NULL, fontManagerGUI);

	layersParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Layers", &spriteArray[9], 0);
	layersProperty = new gePropertyLayers(rendererGUI, layersParentTreeNode, "", NULL, fontManagerGUI);

	fogParentTreeNode = new geTreeNode(rendererGUI, rootNode, "Fog", &spriteArray[9], 0);
	fogProperty = new geSettingsFog(rendererGUI, fogParentTreeNode, "", NULL, monoWrapper::mono_engine_getWorld(0)->getRenderer()->getFog(), fontManagerGUI);

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

	propertiesTreeView->draw();
}

void gearScenePropertyEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearScenePropertyEditor::onMouseMove(float x, float y, int flag)
{
	geTreeNode* selectedNode=propertiesTreeView->getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(propertiesTreeView->getScrollBar()->isScrollBarGrabbed())
			return true;

		if(selectedNode->getParent()==animationParentTreeNode)
		{
			std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();
			newlist->push_back(selectedNode);

			MDropData* dataObject = new MDropData(newlist, this, SDL_GetWindowID(getRenderer()->getWindow()));
			doDragDropSynchronous(dataObject);
		}
	}
	return true;
}

void gearScenePropertyEditor::removeAllProperties()
{
	geTreeNode* rootNode=propertiesTreeView->getRoot();

	rootNode->removeTVChild(object3dParentTreeNode);
	rootNode->removeTVChild(transformParentTreeNode);
	rootNode->removeTVChild(materialParentTreeNode);
	rootNode->removeTVChild(animationParentTreeNode);
	rootNode->removeTVChild(saveMetaDataParentTreeNode);
	rootNode->removeTVChild(lightParentTreeNode);
	rootNode->removeTVChild(addComponentParentTreeNode);
	rootNode->removeTVChild(postProcessorBlurShaderParentTreeNode);
	rootNode->removeTVChild(cameraParentTreeNode);

	rootNode->removeTVChild(openOnEditorParentTreeNode);
	rootNode->removeTVChild(octreeParentTreeNode);
	rootNode->removeTVChild(layersParentTreeNode);
	rootNode->removeTVChild(fogParentTreeNode);

	for(std::vector<stMonoScriptTVNode*>::iterator it = monoScriptTreeViewNodeList.begin(); it != monoScriptTreeViewNodeList.end(); ++it)
	{
		stMonoScriptTVNode* scriptNode = *it;
		rootNode->removeTVChild(scriptNode->m_pMonoScriptParentNode);
		GX_DELETE(scriptNode);
	}
	monoScriptTreeViewNodeList.clear();

	//material
	if(materialParentTreeNode)
		materialParentTreeNode->destroyAllTVChilds();

	//animation
	if(animationParentTreeNode)
		animationParentTreeNode->destroyAllTVChilds();
}

void gearScenePropertyEditor::populatePropertyOfBlurShader(gxHWShader* blurShader)
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	//blur
	blurPostProcessorProperty->populatePropertyOfBlurShader(blurShader);
	rootNode->appnendTVChild(postProcessorBlurShaderParentTreeNode);

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfOpenInEditor()
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	rootNode->appnendTVChild(openOnEditorParentTreeNode);

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfOctree()
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	rootNode->appnendTVChild(octreeParentTreeNode);

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfLayers()
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	rootNode->appnendTVChild(layersParentTreeNode);

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populateSettingsOfFog()
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	rootNode->appnendTVChild(fogParentTreeNode);

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::populatePropertyOfObject(object3d* obj)
{
	removeAllProperties();

	geTreeNode* rootNode=propertiesTreeView->getRoot();
	if(obj==NULL)
	{
		propertiesTreeView->refreshTreeView();
        propertiesTreeView->resetSelectedNodePtr();
		return;
	}

	//object3d
	object3dProperty->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(object3dParentTreeNode);

	//transform
	transformProperty->populatePropertyOfTransform(obj);
	rootNode->appnendTVChild(transformParentTreeNode);

	//material
	if(materialParentTreeNode)
		materialParentTreeNode->destroyAllTVChilds();
	if(obj->getID()==OBJECT3D_MESH || obj->getID()==OBJECT3D_SKINNED_MESH)	//mesh
	{
		gxMesh* mesh=(gxMesh*)obj;

		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			//if(!mesh->getTriInfo(x)->getMaterial())
			//	continue;
			gePropertyMaterial* materialProperty = new gePropertyMaterial(rendererGUI, materialParentTreeNode, "", NULL, mesh->getTriInfo(x), fontManagerGUI);
            UNUSED(materialProperty);
		}

		rootNode->appnendTVChild(materialParentTreeNode);
	}
	else if(obj->getID()==OBJECT3D_LIGHT)
	{
		lightProperty->populatePropertyOfLight(obj);
		rootNode->appnendTVChild(lightParentTreeNode);
	}
	else if(obj->getID()==OBJECT3D_CAMERA)
	{
		cameraProperty->populatePropertyOfCamera(obj);
		rootNode->appnendTVChild(cameraParentTreeNode);
	}

	//animation
	if(animationParentTreeNode)
		animationParentTreeNode->destroyAllTVChilds();
	if(obj->getAnimationController())
	{
		
		gxAnimation* animationController=obj->getAnimationController();
		const std::vector<gxAnimationSet*>* animList=animationController->getAnimationSetList();
        for (auto animationSet : *animList)
		{
			gePropertyAnimationSet* pAnimationSetNode  = new gePropertyAnimationSet(rendererGUI, animationParentTreeNode, obj, animationSet, &spriteArray[6], fontManagerGUI);
			pAnimationSetNode->setUserData(animationSet);
		}
		rootNode->appnendTVChild(animationParentTreeNode);
	}

	saveMetaDataProperty->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(saveMetaDataParentTreeNode);
	saveMetaDataParentTreeNode->closeNode();

	////hack
	//blurPostProcessorProperty->populatePropertyOfBlurShader(NULL);
	//rootNode->appnendTVChild(postProcessorBlurShaderParentTreeNode);
	////

	//components
	addComponentProperty->populatePropertyOfObject(obj);
	rootNode->appnendTVChild(addComponentParentTreeNode);
	addComponentParentTreeNode->closeNode();

	//mono scripts
	if(obj->getMonoScriptInstanceCount())
	{
		for(int x=0;x<obj->getMonoScriptInstanceCount();x++)
		{
			monoScriptObjectInstance* monoInstance = obj->getMonoScriptInstance(x);
			stMonoScriptTVNode* monoScriptTVNode = new stMonoScriptTVNode();
			monoScriptTVNode->m_pMonoScriptParentNode = new geTreeNode(rendererGUI, rootNode, monoInstance->getScriptPtr()->getScriptFileName().c_str(), &spriteArray[10], 0);
			monoScriptTVNode->m_pSettingsMonoScript = new gePropertyScriptComponent(rendererGUI, monoScriptTVNode->m_pMonoScriptParentNode, "", NULL, fontManagerGUI);
			monoScriptTVNode->m_pMonoScriptParentNode->closeNode();

			monoScriptTVNode->m_pSettingsMonoScript->populatePropertyOfMonoScripts(obj, monoInstance);
			monoScriptTreeViewNodeList.push_back(monoScriptTVNode);
		}
	}

	propertiesTreeView->refreshTreeView();
	propertiesTreeView->resetSelectedNodePtr();
}

void gearScenePropertyEditor::updateTransformPropertyOfCurrentSelectedObject()
{
	geTreeNode* rootNode=propertiesTreeView->getRoot();

	std::vector<geGUIBase*>* controlList=rootNode->getChildControls();
	if(std::find(controlList->begin(), controlList->end(), transformParentTreeNode)!=controlList->end())
	{
		transformProperty->updatePropertyView();
	}
}

