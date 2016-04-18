#ifndef GEPROPERTYSCRIPTCOMPONENT_H
#define GEPROPERTYSCRIPTCOMPONENT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geStaticTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../AssetImporter.h"
#include "../../gui/geWindowColumn.h"

class gePropertyScriptComponent : public geTreeNode, public MGUIObserver
{
public:
	gePropertyScriptComponent(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager);
	virtual ~gePropertyScriptComponent();

	virtual void drawNode();
	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	virtual void onButtonClicked(geGUIBase* btn);

	void populatePropertyOfMonoScripts(monoScriptObjectInstance* monoScriptobject);

private:
	geWindowColumn* windowColumnControl;
};

#endif