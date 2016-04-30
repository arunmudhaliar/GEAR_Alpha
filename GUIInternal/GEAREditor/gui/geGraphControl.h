#pragma once

#include "geGUIBase.h"
#include "geScrollBar.h"
#include "../../../GEAREngine/src/animation/gxAnimationTrack.h"

class geGraphControl : public geGUIBase, public MScrollBarObserver
{
public:
	geGraphControl(geFontManager* fontmanager);
	virtual ~geGraphControl();

	void create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy, int lowerLimit, int upperLimit);
	virtual void draw();
	
    void setTrack(IAnimationTrack* track);
    
    void showPositionValues(int coordinate);
    void showRotationValues(int coordinate);
    void showScaleValues(int coordinate);

protected:
	virtual void onSize(float cx, float cy, int flag);
    
    virtual bool onMouseLButtonDown(float x, float y, int nFlag);
    virtual bool onMouseLButtonUp(float x, float y, int nFlag);
    virtual bool onMouseRButtonDown(float x, float y, int nFlag);
    virtual void onMouseRButtonUp(float x, float y, int nFlag);
    virtual bool onMouseMButtonDown(float x, float y, int nFlag);
    virtual void onMouseMButtonUp(float x, float y, int nFlag);
    
    virtual bool onMouseMove(float x, float y, int flag);
    virtual void onMouseWheel(int zDelta, int x, int y, int flag);

    virtual void onScrollBarChange(geScrollBar* scrollbar);
    virtual void onResizeComplete();
    virtual void onCancelEngagedControls();
    
    IAnimationTrack* animationTrack;
    
private:
    void resetShowFlags();
    
    float divisions;
    int lowerLimit;
    int upperLimit;
    vector2f pivot;
    vector2f previousPivot;
    vector2f previousClickPivot;
    vector2f graphOffset;
    
    vector2f mousePos;
    vector2f mousePrevPos;
    
	float vertexBufferClientAreaArray[8];
    
    float minExtend[9];
    float maxExtend[9];
    float verticalScale[9];
    float midPoint[9];
    int showValueIndex;
    
    stVertexBuffer normalKeyVertexBuffer;
    stVertexBuffer hoverKeyVertexBuffer;
    stVertexBuffer selectedKeyVertexBuffer;
    stVertexBuffer selectionRectVertexBuffer;
    bool drawSelection;
    bool dragEnabled;
    
    stVertexBuffer timeScaleVertexBuffer;
    stVertexBuffer timeScaleShadeVertexBuffer;
    
    geScrollBar* horizontalScrollBar;
};