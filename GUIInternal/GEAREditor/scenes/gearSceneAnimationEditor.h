#ifndef GEARSCENEANIMATIONEDITOR_H
#define GEARSCENEANIMATIONEDITOR_H

#include "../gui/geWindow.h"
#include "../gui/geTreeView.h"
//#include "../../../GEAREngine/src/core/gxMesh.h"
//
//#include "propertyViews/gePropertyObject3d.h"
//#include "propertyViews/gePropertyTransform.h"
//#include "propertyViews/gePropertyMaterial.h"
//#include "propertyViews/gePropertyAnimationSet.h"
//#include "propertyViews/gePropertySaveMetaData.h"
//#include "propertyViews/gePropertyLight.h"
//#include "propertyViews/gePropertyBlurProcessor.h"
//#include "propertyViews/gePropertyAddComponent.h"
//#include "propertyViews/gePropertyCamera.h"
//#include "propertyViews/gePropertyOpenOnEditor.h"
//#include "propertyViews/gePropertyOctree.h"
//#include "propertyViews/gePropertyLayers.h"
//#include "propertyViews/geSettingsFog.h"
//#include "propertyViews/gePropertyScriptComponent.h"

class gearSceneAnimationEditor : public geWindow, public MTreeViewObserver
{
public:

	gearSceneAnimationEditor(geFontManager* fontmanager);
	~gearSceneAnimationEditor();

	void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);

protected:
	virtual void onCreate();
	virtual void onDraw();

private:
	geTreeView* m_pAnimationsTreeView;
};

#endif