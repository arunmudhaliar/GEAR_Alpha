#ifndef GEGUIMANAGER_H
#define GEGUIMANAGER_H

#include <vector>
#include "geWindow.h"
#include "../util/FontManager.h"
#include "geLayoutManager.h"
#include "../core/geTextureManager.h"

class geGUIManager
{
public:
	geGUIManager();
	~geGUIManager();

	void init();
	void size(int cx, int cy);
	void update(float dt);
	void draw();

	void appendWindow(geWindow* window);

	void MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);
	void MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);

	void MouseMove(float x, float y, int flag);
	void MouseWheel(int zDelta, int x, int y, int flag);

	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDataObject* dropObject);
	void DragLeave();

	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	void DoCommand(int cmd);

	geLayoutManager* getLayoutManager()		{	return &m_cLayoutManager;	}

private:
	std::vector<geWindow*> m_vWindowObjects;

	geWindow* m_pSelectedWindow;
	geVector2i m_cMousePreviousPos;

	geLayoutManager m_cLayoutManager;
public:
	static FontManager g_cFontManager;
	static gxFont* g_pFontArial12Ptr;
	static CGETextureManager g_cTextureManager;
};

#endif