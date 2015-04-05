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
	Camera* m_pCameraPtr;

	geToolBarDropMenu* m_pCameraTypeToolBarDropMenuBtnPtr;
	geHorizontalSlider* m_pHorizontalSlider_FOV;
	geHorizontalSlider* m_pHorizontalSlider_Near;
	geHorizontalSlider* m_pHorizontalSlider_Far;
	geToolBarDropMenu* m_pCameraCullingToolBarDropMenuBtnPtr;

	geButton* m_pButtonApplyMainCamera;

	gePropertyCamera(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager);
	virtual ~gePropertyCamera();


	void populatePropertyOfCamera(object3d* obj);

	virtual void drawNode();

	virtual void onTextChange(geGUIBase* textbox);
	virtual void onSliderChange(geGUIBase* slider);
	virtual void onColorChange(geGUIBase* colorControl);
//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual void onButtonClicked(geGUIBase* btn);

	virtual void onCommand(int cmd);
};

#endif