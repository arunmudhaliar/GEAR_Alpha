#pragma once

#include "geGUIBase.h"
#include <vector>
#include "geLayout.h"

class geLayoutManager : public geGUIBase
{
public:
	geLayoutManager(geFontManager* fontmanager);
	virtual ~geLayoutManager();

	void create(rendererGL10* renderer, float x, float y, float cx, float cy);

	void mouseMoveWithGrabbedWindow(int x, int y, geWindow* grabbedWindow);
	void dropGrabbedWindow(int x, int y, geWindow* grabbedWindow);

	geLayout* getRootLayout()	{	return rootLayout;	}

	void expandOrContractLeftAndRightLayoutsOnMouseDrag(int x, int y);
	void expandOrContractTopAndBottomLayoutsOnMouseDrag(int x, int y);

	virtual void draw();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);

	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
	virtual void onDragLeave();
//#endif

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	virtual void onCommand(int cmd);

private:
	geLayout* selectedLayout;
	geLayout* rootLayout;
	geVector2f previousScale;
	geVector2i mousePreviousPos;

	std::vector<geLayout*> leftResizeLayoutList;
	std::vector<geLayout*> rightResizeLayoutList;

	std::vector<geLayout*> topResizeLayoutList;
	std::vector<geLayout*> bottomResizeLayoutList;
};