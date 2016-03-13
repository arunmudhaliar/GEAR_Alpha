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

	geTreeNode* getMaterialParentNode()		{	return materialParentTreeNode;		}
	geTreeNode* getAnimationParentNode()	{	return animationParentTreeNode;	}

protected:
	virtual void onCreate(float cx, float cy);
	virtual void onDraw();

	virtual bool onMouseMove(float x, float y, int flag);

private:
	geTreeView* propertiesTreeView;
	Sprite2Dx spriteArray[15];

	geTreeNode* object3dParentTreeNode;
	geTreeNode* transformParentTreeNode;
	geTreeNode* materialParentTreeNode;
	geTreeNode* animationParentTreeNode;
	geTreeNode* saveMetaDataParentTreeNode;
	geTreeNode* lightParentTreeNode;
	geTreeNode* addComponentParentTreeNode;
	geTreeNode* cameraParentTreeNode;

	gePropertyObject3d* object3dProperty;
	gePropertyTransform* transformProperty;
	gePropertyMaterial* materialProperty;
	gePropertySaveMetaData* saveMetaDataProperty;
	gePropertyLight* lightProperty;
	gePropertyAddComponent* addComponentProperty;
	gePropertyCamera* cameraProperty;

	//open on editor
	geTreeNode* openOnEditorParentTreeNode;
	gePropertyOpenOnEditor* openOnEditorProperty;

	//post processors
	geTreeNode* postProcessorBlurShaderParentTreeNode;
	gePropertyBlurProcessor* blurPostProcessorProperty;

	//octree
	geTreeNode* octreeParentTreeNode;
	gePropertyOctree* octreeProperty;

	//layers
	geTreeNode* layersParentTreeNode;
	gePropertyLayers* layersProperty;

	//Fog
	geTreeNode* fogParentTreeNode;
	geSettingsFog* fogProperty;

	//Scripts
	std::vector<stMonoScriptTVNode*> monoScriptTreeViewNodeList;
};