#ifndef GEPROPERTYOPENONEDITOR_H
#define GEPROPERTYOPENONEDITOR_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../gui/geWindowColumn.h"


class gePropertyOpenOnEditor : public geTreeNode, public MGUIObserver
{
public:
	gePropertyOpenOnEditor(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager);
	virtual ~gePropertyOpenOnEditor();

	virtual void drawNode();
	virtual void onButtonClicked(geGUIBase* btn);
    
    geButton* openOnEditorButton;
};

#endif