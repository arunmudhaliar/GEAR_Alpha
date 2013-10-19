#ifndef GESECONDRYVIEW_H
#define GESECONDRYVIEW_H

#include "../util/geDefines.h"
#include "../renderer/rendererGL10.h"
#include <Windows.h>
#include "../../resource.h"
#include "../gui/geLayout.h"

class geSecondryView
{
public:
	geSecondryView(const char* name);
	virtual ~geSecondryView();

	void createRenderer(HWND hwnd);
	rendererGL10* getRenderer()	{	return m_pSecondryRenderer;	}

	void showView(HWND parentHWND);
	void drawView();
	void sizeView(float cx, float cy);
	void destroyView();

	static LRESULT CALLBACK SecondryView_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
protected:
	rendererGL10* m_pSecondryRenderer;
	geLayout* m_pRootLayout;
	geVector2f m_cPrevScale;
	char m_szName[256];

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();
};
#endif