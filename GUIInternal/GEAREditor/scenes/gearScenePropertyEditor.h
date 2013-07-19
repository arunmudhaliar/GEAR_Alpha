#ifndef GEARSCENEPROPERTYEDITOR_H
#define GEARSCENEPROPERTYEDITOR_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
#include "../../../GEAREngine/src/core/gxMesh.h"

#include "propertyViews\gePropertyObject3d.h"
#include "propertyViews\gePropertyTransform.h"
#include "propertyViews\gePropertyMaterial.h"

class gearScenePropertyEditor : public geWindow, public MTreeViewObserver
{
public:
	gearScenePropertyEditor();
	~gearScenePropertyEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

	void populatePropertyOfObject(object3d* obj);

protected:
	virtual void onCreate();
	virtual void onDraw();

private:
	geTreeView m_cPropertiesTreeView;
	Sprite2Dx m_cszSprites[5];

	geTreeNode* m_pObject3dParentNode;
	geTreeNode* m_pTransformParentNode;
	geTreeNode* m_pMaterialParent;

	gePropertyObject3d* m_pObject3dPropertyNode;
	gePropertyTransform* m_pTransformPropertyNode;
	gePropertyMaterial* m_pMaterialPropertyNode;
};

#endif