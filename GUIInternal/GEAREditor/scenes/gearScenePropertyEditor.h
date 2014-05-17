#ifndef GEARSCENEPROPERTYEDITOR_H
#define GEARSCENEPROPERTYEDITOR_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/core/gxMesh.h"

#include "propertyViews\gePropertyObject3d.h"
#include "propertyViews\gePropertyTransform.h"
#include "propertyViews\gePropertyMaterial.h"
#include "propertyViews\gePropertyAnimationSet.h"
#include "propertyViews\gePropertySaveMetaData.h"
#include "propertyViews\gePropertyLight.h"
#include "propertyViews\gePropertyBlurProcessor.h"
#include "propertyViews\gePropertyAddComponent.h"
#include "propertyViews\gePropertyCamera.h"
#include "propertyViews\gePropertyOpenOnEditor.h"
#include "propertyViews\gePropertyOctree.h"

class gearScenePropertyEditor : public geWindow, public MTreeViewObserver
{
public:
	gearScenePropertyEditor();
	~gearScenePropertyEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	void removeAllProperties();
	void populatePropertyOfObject(object3d* obj);
	void populatePropertyOfBlurShader(gxHWShader* blurShader);
	void populatePropertyOfOpenInEditor();
	void populatePropertyOfOctree();

	geTreeNode* getMaterialParentNode()		{	return m_pMaterialParent;		}
	geTreeNode* getAnimationParentNode()	{	return m_pAnimationParentNode;	}

protected:
	virtual void onCreate();
	virtual void onDraw();

	virtual bool onMouseMove(float x, float y, int flag);

private:
	geTreeView m_cPropertiesTreeView;
	Sprite2Dx m_cszSprites[10];

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

};

#endif