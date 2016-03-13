#pragma once

#include "geSecondryView.h"

class geModalDialog : public geSecondryView, public MGUIObserver
{
public:
	geModalDialog(geWindow* pWindow, geVector2i pos);
	virtual ~geModalDialog();

	virtual void onCreate(float cx, float cy);
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

    static geModalDialog* createModalDialog(geWindow* pWindow, geVector2i pos);
    
protected:
	geWindow* m_pWindow;
};