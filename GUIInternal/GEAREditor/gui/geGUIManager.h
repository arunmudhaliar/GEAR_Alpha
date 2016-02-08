#pragma once

#include <vector>
#include "geWindow.h"
#include "../util/geFontManager.h"
#include "geLayoutManager.h"
#include "../core/geTextureManager.h"

class geGUIManager
{
public:
	geGUIManager(geFontManager* fontmanager);
	~geGUIManager();

	void init(rendererGL10* renderer);
	void reset();

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

//#if !defined(__APPLE__) //disable Drag-Drop
	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDropData* dropObject);
	void DragLeave();
//#endif
    
	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	void DoCommand(int cmd);

	geLayoutManager* getLayoutManager()		{	return m_pLayoutManager;	}

private:
	std::vector<geWindow*> m_vWindowObjects;

	geWindow* m_pSelectedWindow;
	geVector2i m_cMousePreviousPos;

	geLayoutManager* m_pLayoutManager;
public:
	static CGETextureManager g_cTextureManager;
};