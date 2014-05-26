#ifndef GESEPERATOR_H
#define GESEPERATOR_H

#include "geGUIBase.h"

class geSeperator : public geGUIBase
{
public:
	enum ESEPERATOR_STYPE
	{
		STYLE_FLAT,
		STYLE_3D
	};

	geSeperator();
	virtual ~geSeperator();

	void create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx);
	virtual void draw();
	
	void setStyle(ESEPERATOR_STYPE style);

protected:
	virtual void onSize(float cx, float cy, int flag);
	float m_cVBClientAreaLine[8];
	ESEPERATOR_STYPE m_eStyle;
};

#endif