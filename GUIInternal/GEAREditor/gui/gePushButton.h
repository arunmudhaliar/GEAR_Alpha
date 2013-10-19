#ifndef GEPUSHBUTTON_H
#define GEPUSHBUTTON_H

#include "geButton.h"

class gePushButton : public geButtonBase
{
public:
	gePushButton();
	gePushButton(const char* name);

protected:
	gePushButton(unsigned short uGUIID, const char* name);

public:
	virtual ~gePushButton();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y);
	virtual void draw();
	
	void setCheck(bool flag)	{	m_bCheck=flag;		}
	bool isCheck()				{	return m_bCheck;	}

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onButtonClicked();

	stVertexBuffer m_cVBClientArea;
	stVertexBuffer m_cVBCheckMark;
	float m_cVBClientAreaLine[8];
	bool m_bCheck;
};
#endif