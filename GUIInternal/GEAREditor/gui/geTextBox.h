#pragma once

#include "geGUIBase.h"

class geTextBox : public geGUIBase
{
public:
	geTextBox(geFontManager* fontmanager);
	geTextBox(const char* name, geFontManager* fontmanager);

public:
	virtual ~geTextBox();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy);
	//void create(geGUIBase* parent, const char* name, float x, float y);

	virtual void draw();
	
	static geTextBox* g_pCurrentSelectedTextBoxPtr;
	static geTextBox* g_pCurrentlyActiveTextBoxPtr;
	void clearSelection();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onCancelEngagedControls();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	void setCursorPos(int index);

	virtual void onSetName();
	float getVirtualEndBound(int& index);

private:
	stVertexBuffer m_cVBSelectionArea;
	float m_cVBClientAreaLine[10];
	float m_cVBCursorLine[4];
	bool m_bStartSelection;
	bool m_bShowSelection;
	bool m_bShowCursor;
	float m_fPrevMouseXPos;
	float m_fSelectionStartXPos;
	float m_fCursorPosInPixels;
	bool m_bControlSelected;
	int m_iCursorPos;
	int m_iStartCursorSelectionPos;
	int m_iEndCursorSelectionPos;

	int m_iStartStringCharToDisplay;
	int m_nStringCharToDisplay;
	char* m_pStartCharPtr;
};