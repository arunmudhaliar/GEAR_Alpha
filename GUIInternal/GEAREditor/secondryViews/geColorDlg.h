#ifndef GECOLORDLG_H
#define GECOLORDLG_H

#include "geSecondryView.h"

class geColorDlg : public geSecondryView
{
public:
	geColorDlg();
	virtual ~geColorDlg();

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

protected:
	geWindow* m_pWindow;
};

#endif