#ifndef GEPUSHBUTTON_H
#define GEPUSHBUTTON_H

#include "geButton.h"

class gePushButton : public geButtonBase
{
public:
	gePushButton(geFontManager* fontManager);
	gePushButton(const char* name, geFontManager* fontManager);

protected:
	gePushButton(unsigned short uGUIID, const char* name, geFontManager* fontManager);

public:
	virtual ~gePushButton();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y);
	virtual void draw();
	
	void setCheck(bool flag)	{	m_bCheck=flag;		}
	bool isCheck()				{	return m_bCheck;	}
	void refresh();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onButtonClicked();

	stVertexBuffer m_cVBCheckMark;
	float m_cVBClientAreaLine[8];
	bool m_bCheck;
};
#endif