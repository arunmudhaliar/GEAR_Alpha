#pragma once

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/core/gxMesh.h"

#include "propertyViews/gePropertyObject3d.h"
#include "propertyViews/gePropertyTransform.h"
#include "propertyViews/gePropertyMaterial.h"
#include "propertyViews/gePropertyAnimationSet.h"
#include "propertyViews/gePropertySaveMetaData.h"
#include "propertyViews/gePropertyLight.h"
#include "propertyViews/gePropertyBlurProcessor.h"
#include "propertyViews/gePropertyAddComponent.h"
#include "propertyViews/gePropertyCamera.h"
#include "propertyViews/gePropertyOpenOnEditor.h"
#include "propertyViews/gePropertyOctree.h"
#include "propertyViews/gePropertyLayers.h"
#include "propertyViews/geSettingsFog.h"
#include "propertyViews/gePropertyScriptComponent.h"

class gearScenePropertyEditor : public geWindow, public MTreeViewObserver
{
public:
	class stMonoScriptTVNode
	{
	public:
		stMonoScriptTVNode();
		~stMonoScriptTVNode();

		geTreeNode* m_pMonoScriptParentNode;
		gePropertyScriptComponent* m_pSettingsMonoScript;
	};

	gearScenePropertyEditor(geFontManager* fontmanager);
	~gearScenePropertyEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	void removeAllProperties();
	void populatePropertyOfObject(object3d* obj);
	void populatePropertyOfBlurShader(gxHWShader* blurShader);
	void populatePropertyOfOpenInEditor();
	void populatePropertyOfOctree();
	void populatePropertyOfLayers();
	void populateSettingsOfFog();

	void updateTransformPropertyOfCurrentSelectedObject();

	geTreeNode* getMaterialParentNode()		{	return m_pMaterialParent;		}
	geTreeNode* getAnimationParentNode()	{	return m_pAnimationParentNode;	}

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseMove(float x, float y, int flag);

private:
	geTreeView* m_pPropertiesTreeView;
	Sprite2Dx m_cszSprites[15];

	geTreeNode* m_pObject3dParentNode;
	geTreeNode* m_pTransformParentNode;
	geTreeNode* m_pMaterialParent;
	geTreeNode* m_pAnimationParentNode;
	geTreeNode* m_pSaveMetaDataParentNode;
	geTreeNode* m_pLightParentNode;
	geTreeNode* m_pAddComponentParentNode;
	geTreeNode* m_pCameraParentNode;

	gePropertyObject3d* m_pObject3dPropertyNode;
	gePropertyTransform* m_pTransformPropertyNode;
	gePropertyMaterial* m_pMaterialPropertyNode;
	gePropertySaveMetaData* m_pSaveMetaDataPropertyNode;
	gePropertyLight* m_pLightPropertyNode;
	gePropertyAddComponent* m_pAddComponentProperty;
	gePropertyCamera* m_pCameraPropertyNode;

	//open on editor
	geTreeNode* m_pOpenOnEditorParentNode;
	gePropertyOpenOnEditor* m_pPropertyOpenOnEditor;

	//post processors
	geTreeNode* m_pPostProcessorBlurShaderNode;
	gePropertyBlurProcessor* m_pBlurProcessorPropertyNode;

	//octree
	geTreeNode* m_pOctreeParentNode;
	gePropertyOctree* m_pPropertyOctree;

	//layers
	geTreeNode* m_pLayersParentNode;
	gePropertyLayers* m_pPropertyLayers;

	//Fog
	geTreeNode* m_pFogParentNode;
	geSettingsFog* m_pSettingsFog;

	//Scripts
	std::vector<stMonoScriptTVNode*> m_vMonoScript;
};