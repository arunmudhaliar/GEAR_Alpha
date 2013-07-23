#ifndef GEHORIZONTALSLIDER_H
#define GEHORIZONTALSLIDER_H

#include "geGUIBase.h"

class geHorizontalSlider : public geGUIBase
{
public:
	geHorizontalSlider();

public:
	virtual ~geHorizontalSlider();

	void create(geGUIBase* parent, const char* name, float x, float y, float cx);

	virtual void draw();
	float getSliderValue()				{	return m_fSliderPos;	}
	void setSliderValue(float value);

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	
	virtual void onSliderChange(float sliderValue);

private:
	stVertexBuffer m_cVBClientArea;
	stVertexBuffer m_cVBGrabberArea;
	float m_cVBClientAreaLine[10];
	float m_fSliderPos;
	bool m_bGrabbed;
	float m_fMousePrevXPos	;
};

#endif