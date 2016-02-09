#pragma once

#include "geGUIBase.h"

class geHorizontalSlider : public geGUIBase
{
public:
	geHorizontalSlider(geFontManager* fontmanager);

public:
	virtual ~geHorizontalSlider();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx);

	virtual void draw();
	float getSliderValue()				{	return sliderValue;	}
	void setSliderValue(float value, bool bCallObserver=true);
    
	float getSliderValueWithInRange();
	void setSliderValueWithInRange(float value);

	void setRange(float min, float max);
    
    bool isGrabbed() {return is_Grabbed;}
    
protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	
	virtual void onSliderChange(float sliderValue);

private:
	stVertexBuffer vertexBufferGrabberArea;
	float vertexBufferClientAreaArray[10];
	float sliderValue;
	bool is_Grabbed;
	float mousePrevXPos	;

	float minRange;
	float maxRange;
};