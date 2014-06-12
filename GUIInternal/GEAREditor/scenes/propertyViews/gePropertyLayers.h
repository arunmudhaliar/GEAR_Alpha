#ifndef GEPROPERTYLAYERS_H
#define GEPROPERTYLAYERS_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../gui/geWindowColumn.h"

class gePropertyLayers : public geTreeNode, public MGUIObserver
{
public:
	gePropertyLayers(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite);
	virtual ~gePropertyLayers();
	virtual void drawNode();
	virtual void onTextChange(geGUIBase* textbox);

private:
	geTextBox* m_pTextBoxLayers[32];	//32 layers are there
};

#endif