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
    void removeWindow(geWindow* window);

	geWindow* getActiveWindow()	{	return activeWindow;	}
	void setActiveWindow(int index);
	void setActiveWindow(geWindow* wnd);

	void resize(double xScale, double yScale);

	geLayout* createLeft(geWindow* window, float ratio=0.5f);
	geLayout* createRight(geWindow* window, float ratio=0.5f);
	geLayout* createTop(geWindow* window, float ratio=0.5f);
	geLayout* createBottom(geWindow* window, float ratio=0.5f);
	geLayout* createAsParent(geWindow* window);

	geLayout* dropWindowOnMe(geWindow* window);
	geLayout* getParentLayout()	{	return parentLayout;	}

	geLayout* getRightMostParentLayout();
	geLayout* getLeftMostParentLayout();
	geLayout* getTopMostParentLayout();
	geLayout* getBottomMostParentLayout();

	void getResizableOnLeftSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnRightSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnTopSide(int x, int y, std::vector<geLayout*>* vList);
	void getResizableOnBottomSide(int x, int y, std::vector<geLayout*>* vList);

	void setParentLayout(geLayout* parent)	{	parentLayout=parent;	}

	bool removeChildLayout(geLayout* childLayout);
	void appendLeftChildLayout(geLayout* childLayout);
	void appendRightChildLayout(geLayout* childLayout);
	void appendTopChildLayout(geLayout* childLayout);
	void appendBottomChildLayout(geLayout* childLayout);

	void setLayoutDirection(ELAYOUT_DIRECTION eDirection)	{	layoutDirection=eDirection;	}
	ELAYOUT_DIRECTION getLayoutDirection()					{	return layoutDirection;		}

	geLayout* selectLayout(int x, int y);

	geLayout* checkResizableOnLeftSide(int x, int y);
	geLayout* checkResizableOnRightSide(int x, int y);
	geLayout* checkResizableOnTopSide(int x, int y);
	geLayout* checkResizableOnBottomSide(int x, int y);

	virtual void draw();

	void traverseMouseMoveEvent(int x, int y, int flag);

    void unPinLayout()  {   is_Pinned = false;  }
    void pinLayout()    {   is_Pinned = true;   }
    bool isPinned()     {   return is_Pinned;   }
    geVector2f getUnPinRectSize()    {   return geVector2f(unPinSprite.getClipWidth(), unPinSprite.getClipHeight());   }
    bool isOverlapWithPinRect(float x, float y);
    bool doUnPin(float x, float y, geLayout* rootLayout, bool& deleteLayout);   //this is used for un-pinning
    void doUnPlug(geLayout* rootLayout);    //used for grabbing
    
    bool canGrab(float x, float y);
    geLayout* doGrabOverlapAreaCheck(geLayout* grabbed, float x, float y, int& whichArea);  //0-left, 1-right, 2-top, 3-bottom
    
    static void reAdjustLayoutOnPlug(geLayout* parent, geLayout* grabbed, float ratio, int whichArea);
    
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
    void moveLayoutTo(std::vector<geLayout*>& fillingLayouts, float delta, ELAYOUT_DIRECTION direction);
    void clearAllList();
    std::vector<geLayout*>& getImmediateChildLayoutList()   {   return immediateChildLayoutList;    }
    void checkToFitLayout(geLayout* layout, ELAYOUT_DIRECTION direction, std::vector<geLayout*>& fillingLayouts);
    
    
	geWindow* activeWindow;
	geVector2f mousePreviousPos;

	std::vector<geWindow*> childWindowList;
	std::vector<geLayout*> childTopLayoutList;
	std::vector<geLayout*> childBottomLayoutList;
	std::vector<geLayout*> childRightLayoutList;
	std::vector<geLayout*> childLeftLayoutList;
    std::vector<geLayout*> immediateChildLayoutList;

	geLayout* parentLayout;
	ELAYOUT_DIRECTION layoutDirection;
	float vertexBufferClientAreaArray[6];
    bool is_Pinned;
    Sprite2Dx unPinSprite;
    bool is_AreaGrabbed[4];
    stVertexBuffer grabberAreaRects[4];
};