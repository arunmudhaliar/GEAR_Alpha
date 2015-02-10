#ifndef GEAR2D
#ifndef GECOLORCONTROL_H
#define GECOLORCONTROL_H

#include "geGUIBase.h"
#include "../util/geVector4.h"

class geColorControl : public geGUIBase
{
public:
	geColorControl();

public:
	virtual ~geColorControl();

	void create(rendererGL10* renderer, geGUIBase* parent, float x, float y);

	virtual void draw();
	
	void setControlColor(float r, float g, float b, float a);
	geVector4f getControlColor()	{	return m_cColor;	}

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

private:
	float m_cVBClientAreaLine[10];
	geVector4f m_cColor;
};

#endif
#endif