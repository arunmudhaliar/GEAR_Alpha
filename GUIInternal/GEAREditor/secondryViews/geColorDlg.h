#pragma once

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"
#include "../gui/geColorControl.h"

#define COLOR_DLG_MAX_RESOLUTION	38

class geColorDlg : public geSecondryView, public MGUIObserver
{
public:
	geColorDlg(geColorControl* pObserverControlPtr, geFontManager* fontManager, rendererGL10* mainRenderer);
	virtual ~geColorDlg();

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onSliderChange(geGUIBase* slider);

protected:
	geWindow* m_pWindow;
	geHorizontalSlider* rgbaHorizontalSliderArray[4];
	float colorDlgCircleRadius;
	geVector2f colorCircleVertexArray[COLOR_DLG_MAX_RESOLUTION];
	geVector2f colorPointerVertexArray[12];
	geVector3f colorCircleColorArray[COLOR_DLG_MAX_RESOLUTION];
	geVector2f colorDlgPointerPosition;
	geColorControl* colorControl;
	geColorControl* observerControlPtr;	//must not delete this pointer
    
private:
    geVector4f selectedColor;
};