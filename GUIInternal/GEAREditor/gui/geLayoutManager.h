#ifndef GELAYOUTMANAGER_H
#define GELAYOUTMANAGER_H

#include "geGUIBase.h"
#include <vector>
#include "geLayout.h"

class geLayoutManager : public geGUIBase
{
public:
	geLayoutManager();
	virtual ~geLayoutManager();

	void create(float x, float y, float cx, float cy);

	void mouseMoveWithGrabbedWindow(int x, int y, geWindow* grabbedWindow);
	void dropGrabbedWindow(int x, int y, geWindow* grabbedWindow);

	geLayout* getRootLayout()	{	return m_pRootLayout;	}

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

	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onDragLeave();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

private:
	stVertexBuffer m_cVBClientArea;
	geLayout* m_pSelectedLayout;
	geLayout* m_pRootLayout;
	geVector2f m_cPrevScale;
	geVector2i m_cMousePreviousPos;

	std::vector<geLayout*> m_vLeftResizeList;
	std::vector<geLayout*> m_vRightResizeList;

	std::vector<geLayout*> m_vTopResizeList;
	std::vector<geLayout*> m_vBottomResizeList;
};

#endif