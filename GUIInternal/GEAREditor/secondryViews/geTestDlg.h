#ifndef GETESTDLG_H
#define GETESTDLG_H

#include "geSecondryView.h"
#include "../gui/geHorizontalSlider.h"

class geTestDlg : public geSecondryView, public MGUIObserver
{
public:
	geTestDlg(geFontManager* fontManager, rendererGL10* mainRenderer);
	virtual ~geTestDlg();

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

//	virtual bool onMouseMove(float x, float y, int flag);

//	virtual void onSliderChange(geGUIBase* slider);

protected:
	geWindow* m_pWindow;
};

#endif