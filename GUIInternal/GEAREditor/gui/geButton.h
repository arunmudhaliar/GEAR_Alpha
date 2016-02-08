#pragma once

#include "geGUIBase.h"

class geButtonBase : public geGUIBase
{
public:
	enum EBUTTON_STATE
	{
		BTN_STATE_NORMAL,
		BTN_STATE_PRESSED,
		BTN_STATE_CANCEL
	};

	geButtonBase(geFontManager* fontManager)
	{
        m_pFontManagerPtr=fontManager;
		m_eState=BTN_STATE_NORMAL;
	}
	geButtonBase(unsigned short uGUIID, const char* name, geFontManager* fontManager);
	virtual ~geButtonBase();

	virtual void onCancelEngagedControls();

	void buttonCancel();
	void buttonPressed(bool dontPassEventToObserver);
	void buttonNormal(bool dontPassEventToObserver);
	void buttonHover();
	void buttonUnHover();
	bool isButtonPressed();

protected:
	virtual void onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver)=0;

	virtual void onButtonClicked();

	EBUTTON_STATE m_eState;
	bool m_bMouseHover;
};

class geButton : public geButtonBase
{
public:
	geButton(geFontManager* fontManager);
	geButton(const char* name, geFontManager* fontManager);

protected:
	geButton(unsigned short uGUIID, const char* name, geFontManager* fontManager);

public:
	virtual ~geButton();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy);
	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y);

	virtual void draw();
	
protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState, bool dontPassEventToObserver);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

//private:
	float m_cVBClientAreaLine[8];
};