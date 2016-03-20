#ifndef GEPROPERTYCAMERA_H
#define GEPROPERTYCAMERA_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../../../GEAREngine/src/core/Camera.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../gui/geWindowColumn.h"
#include "../../gui/geButton.h"

class gePropertyCamera : public geTreeNode, public MGUIObserver
{
public:
	gePropertyCamera(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager);
	virtual ~gePropertyCamera();


	void populatePropertyOfCamera(object3d* obj);

	virtual void drawNode();

	virtual void onTextChange(geGUIBase* textbox);
	virtual void onSliderChange(geGUIBase* slider);
	virtual void onColorChange(geGUIBase* colorControl);
//#if !defined(GEAR_APPLE) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual void onButtonClicked(geGUIBase* btn);
	virtual void onCommand(int cmd);
    
    Camera* camera;
    geToolBarDropMenu* cameraTypeToolBarDropMenuButton;
    geHorizontalSlider* fovHorizontalSlider;
    geHorizontalSlider* nearHorizontalSlider;
    geHorizontalSlider* farHorizontalSlider;
    geToolBarDropMenu* cameraCullingToolBarDropMenuButton;
    
    geButton* applyMainCameraButton;
};

#endif