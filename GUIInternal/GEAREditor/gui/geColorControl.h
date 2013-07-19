#ifndef GECOLORCONTROL_H
#define GECOLORCONTROL_H

#include "geGUIBase.h"

class geColorControl : public geGUIBase
{
public:
	geColorControl();

public:
	virtual ~geColorControl();

	void create(geGUIBase* parent, float x, float y);

	virtual void draw();
	
	void setControlColor(float r, float g, float b, float a);

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual void onMouseMove(float x, float y, int flag);

private:
	stVertexBuffer m_cVBClientArea;
	float m_cVBClientAreaLine[10];
};

#endif