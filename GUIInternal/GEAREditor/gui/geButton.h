#ifndef GEBUTTON_H
#define GEBUTTON_H

#include "geGUIBase.h"

class geButtonBase : public geGUIBase
{
public:
	enum EBUTTON_STATE
	{
		BTN_STATE_NORMAL,
		BTN_STATE_PRESSED
	};

	geButtonBase()
	{
		m_eState=BTN_STATE_NORMAL;
	}
	geButtonBase(unsigned short uGUIID, const char* name);
	virtual ~geButtonBase();

	virtual void onCancelEngagedControls();

	void buttonPressed();
	void buttonNormal();
	void buttonHover();
	void buttonUnHover();
	bool isButtonPressed();

protected:
	virtual void onButtonStateChanged(EBUTTON_STATE eFromState)=0;

	virtual void onButtonClicked();

	EBUTTON_STATE m_eState;
	bool m_bMouseHover;
};

class geButton : public geButtonBase
{
public:
	geButton();
	geButton(const char* name);

protected:
	geButton(unsigned short uGUIID, const char* name);

public:
	virtual ~geButton();

	void create(geGUIBase* parent, const char* name, float x, float y, float cx, float cy);
	void create(geGUIBase* parent, const char* name, float x, float y);

	virtual void draw();
	
protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual void onMouseMove(float x, float y, int flag);

	virtual void onButtonStateChanged(EBUTTON_STATE eFromState);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

//private:
	stVertexBuffer m_cVBClientArea;
	float m_cVBClientAreaLine[8];
};

#endif