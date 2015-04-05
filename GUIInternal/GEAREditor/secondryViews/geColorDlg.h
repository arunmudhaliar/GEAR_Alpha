#ifndef GECOLORDLG_H
#define GECOLORDLG_H

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

	geHorizontalSlider* m_pHorizontalSlider_RGBA[4];

	float m_fCircleRadius;
	geVector2f m_pszColorCircleVertices[COLOR_DLG_MAX_RESOLUTION];
	geVector2f m_pszColorPointerVertices[12];
	geVector3f m_pszColorCircleColor[COLOR_DLG_MAX_RESOLUTION];
	geVector2f m_cPointerPos;
	geVector2f m_cRGBPointerPos[3];
	geColorControl* m_pColorControl;

	geColorControl* m_pObserverControlPtr;	//must not delete this pointer
};

#endif