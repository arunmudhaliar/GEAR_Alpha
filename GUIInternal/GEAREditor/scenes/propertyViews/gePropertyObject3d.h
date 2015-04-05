#ifndef GEPROPERTYOBJECT3D_H
#define GEPROPERTYOBJECT3D_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../gui/geToolBarDropMenu.h"

class gePropertyObject3d : public geTreeNode, public MGUIObserver
{
public:
	gePropertyObject3d(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager);

	virtual ~gePropertyObject3d();

	//virtual void draw();
	virtual void drawNode();
	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	virtual void onButtonClicked(geGUIBase* btn);
	virtual void onCommand(int cmd);

	void populatePropertyOfObject(object3d* obj);

private:
	object3d* m_pObject3dPtr;
	geTextBox* m_pTextBoxMeshName;
	gePushButton* m_pPushBtn_Object3dVisible;
	gePushButton* m_pPushBtn_Object3dStatic;
	geToolBarDropMenu* m_pTagDropDownMenu;
	geToolBarDropMenu* m_pLayerDropDownMenu;
};

#endif