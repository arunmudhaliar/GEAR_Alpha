#ifndef GESECONDRYVIEW_H
#define GESECONDRYVIEW_H

#include "../util/geDefines.h"
#include "../renderer/rendererGL10.h"
#include <Windows.h>
#include "../../resource.h"
#include "../gui/geLayout.h"
#include "../gui/geLayoutManager.h"

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

	void setSize(geVector2f& sz);
	void setPos(geVector2f& pos);

		//mouse events
	bool mouseLButtonDown(float x, float y, int nFlag);
	void mouseLButtonUp(float x, float y, int nFlag);

	bool mouseRButtonDown(float x, float y, int nFlag);
	void mouseRButtonUp(float x, float y, int nFlag);

	bool mouseMove(float x, float y, int flag);
	void mouseWheel(int zDelta, int x, int y, int flag);

	static LRESULT CALLBACK SecondryView_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
protected:
	rendererGL10* m_pSecondryRenderer;
	geVector2f m_cPrevScale;
	char m_szName[256];
	geLayoutManager m_cLayoutManager;

	geVector2f m_cPos;
	geVector2f m_cSize;

	virtual void onCreate();
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual void onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);
};
#endif