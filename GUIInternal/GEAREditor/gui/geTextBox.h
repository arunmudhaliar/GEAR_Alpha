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
	stVertexBuffer vertexBufferSelectionArea;
	float vertexBufferClientAreaArray[10];
	float vertexBufferCursorLine[4];
	bool isStartSelection;
	bool isShowSelection;
	bool isShowCursor;
	float previousMouseXPosition;
	float selectionStartXPosition;
	float cursorPositionInPixel;
	bool isControlSelected;
	int cursorPosition;
	int startCursorSelectionPosition;
	int endCursorSelectionPosition;

	int startStringCharToDisplay;
	int noOfStringCharToDisplay;
	char* startChar;
};