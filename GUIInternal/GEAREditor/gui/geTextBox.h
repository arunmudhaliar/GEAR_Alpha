#pragma once

#include "geGUIBase.h"
#ifdef GEAR_WINDOWS
#include <SDL.h>
#elif defined(GEAR_APPLE)
#include <SDL2/SDL.h>
#else
#error SDL not included
#endif
#include <map>

class KeyMapper
{
public:
    static KeyMapper& getInstance();
    int getAsciiCode(SDL_Scancode scancode);
    
private:
    KeyMapper();
    void init();
    std::map<SDL_Scancode, int> asciiCodeMap;
};

class geTextBox : public geGUIBase
{
public:
	geTextBox(geFontManager* fontmanager);
	geTextBox(const char* name, geFontManager* fontmanager);

public:
	virtual ~geTextBox();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy);
	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y);

	virtual void draw();
	
	static geTextBox* g_pCurrentSelectedTextBoxPtr;
	static geTextBox* g_pCurrentlyActiveTextBoxPtr;
	void clearSelection();
    void setAcceptOnlyNumbers(bool val) {   acceptOnlyNumbers=val;  }
    
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
    void doRealign();
    void resetVars();

    int keyMapping(int32_t scancode, uint16_t mod);
    
    bool acceptOnlyNumbers;
	stVertexBuffer vertexBufferSelectionArea;
	float vertexBufferClientAreaArray[10];
	float vertexBufferCursorLine[4];
	bool isStartSelection;
	bool isShowSelection;
	bool isShowCursor;
    bool isClearAll;
	float previousMouseXPosition;
	float selectionStartXPosition;
	float cursorPositionInPixel;
	bool isControlSelected;
	int cursorIndexPosition;
	int startCursorSelectionIndexPosition;
	int endCursorSelectionIndexPosition;
//
	int startStringCharToDisplay;
//	int noOfStringCharToDisplay;
	char* startChar;
    char internalTextBuffer[512];
};