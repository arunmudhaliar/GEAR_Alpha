#pragma once

#include "geGUIBase.h"
#include "geWindow.h"
#include <vector>
#include <algorithm>

#define BORDER_LAYOUT_OFFSET	1
#define LEFT_LAYOUT_MARGIN	0

class geWindow;
class geLayout : public geGUIBase
{
public:
	enum ELAYOUT_DIRECTION
	{
		LAYOUT_PARENT,
		LEFT_TO_PARENT,
		RIGHT_TO_PARENT,
		TOP_TO_PARENT,
		BOTTOM_TO_PARENT
	};

	geLayout(geFontManager* fontmanager)
	{
	}
	geLayout(const char* name, geFontManager* fontmanager);
	virtual ~geLayout();

	void create(rendererGL10* renderer, geLayout* pParentLayout, float x, float y, float cx, float cy);

	void appendWindow(geWindow* window);
	geWindow* getActiveWindow()	{	return m_pActiveWindowPointer;	}
	void setActiveWindow(int index);
	void setActiveWindow(geWindow* wnd);

	void resize(double xScale, double yScale);

	geLayout* createLeft(geWindow* window, float ratio=0.5f);
	geLayout* createRight(geWindow* window, float ratio=0.5f);
	geLayout* createTop(geWindow* window, float ratio=0.5f);
	geLayout* createBottom(geWindow* window, float ratio=0.5f);
	geLayout* createAsParent(geWindow* window);

	geLayout* dropWindowOnMe(geWindow* window);
	geLayout* getParentLayout()	{	return m_pParentLayout;	}

	geLayout* getRightMostParentLayout();
	geLayout* getLeftMostParentLayout();
	geLayout* getTopMostParentLayout();
	geLayout* getBottomMostParentLayout();

	void getResizableOnLeftSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnRightSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnTopSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnBottomSide(int x, int y, std::vector<geLayout*>* vList);

	void setParentLayout(geLayout* parent)	{	m_pParentLayout=parent;	}

	bool removeChildLayout(geLayout* childLayout);
	void appendLeftChildLayout(geLayout* childLayout);
	void appendRightChildLayout(geLayout* childLayout);
	void appendTopChildLayout(geLayout* childLayout);
	void appendBottomChildLayout(geLayout* childLayout);

	void setLayoutDirection(ELAYOUT_DIRECTION eDirection)	{	m_eLayoutDirection=eDirection;	}
	ELAYOUT_DIRECTION getLayoutDirection()					{	return m_eLayoutDirection;		}

	geLayout* selectLayout(int x, int y);

	geLayout* checkResizableOnLeftSide(int x, int y);
	geLayout* checkResizableOnRightSide(int x, int y);
	geLayout* checkResizableOnTopSide(int x, int y);
	geLayout* checkResizableOnBottomSide(int x, int y);

	virtual void draw();

	void traverseMouseMoveEvent(int x, int y, int flag);

protected:
	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();
	virtual void onCancelEngagedControls();

private:
	geWindow* m_pActiveWindowPointer;
	geVector2f m_cMousePreviousPos;

	std::vector<geWindow*> m_vChildWindows;
	std::vector<geLayout*> m_vChildTopLayouts;
	std::vector<geLayout*> m_vChildBottomLayouts;
	std::vector<geLayout*> m_vChildRightLayouts;
	std::vector<geLayout*> m_vChildLeftLayouts;
	geLayout* m_pParentLayout;
	ELAYOUT_DIRECTION m_eLayoutDirection;
	float m_cVBClientAreaLine[6];
};