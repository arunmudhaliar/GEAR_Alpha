#ifndef GEPROPERTYLIGHT_H
#define GEPROPERTYLIGHT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../../../GEAREngine/src/script/gxLight.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../gui/geWindowColumn.h"


class gePropertyLight : public geTreeNode, public MGUIObserver
{
public:
	gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager);
	virtual ~gePropertyLight();

	void populatePropertyOfLight(object3d* obj);
	virtual void drawNode();
	virtual void onTextChange(geGUIBase* textbox);
	virtual void onSliderChange(geGUIBase* slider);
	virtual void onColorChange(geGUIBase* colorControl);
//#if !defined(GEAR_APPLE) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual void onButtonClicked(geGUIBase* btn);
	virtual void onCommand(int cmd);
    
    gxLight* previewLight;
    geColorControl* colorControl;
    geColorControl* ambientColorControl;
    geColorControl* specularColorControl;
    geToolBarDropMenu* lightTypeToolBarDropMenuButton;
    geHorizontalSlider* constantAttenuationHorizontalSlider;
    geHorizontalSlider* linearAttenuationHorizontalSlider;
    geHorizontalSlider* quadraticAttenuationHorizontalSlider;
};

#endif