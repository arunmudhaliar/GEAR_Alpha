#pragma once

#include "geGUIBase.h"
#include "../util/geFontManager.h"

class geStaticTextBox : public geGUIBase
{
public:
	geStaticTextBox(geFontManager* fontmanager);
	geStaticTextBox(const char* name, geFontManager* fontmanager);

public:
	virtual ~geStaticTextBox();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float yoffset, geFont* pFont);
	virtual void draw();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onSetName();

private:
	stVertexBuffer vertexBufferSelectionArea;
	float vertexBufferClientAreaArray[10];
	geFont* fontGUI;
	float fontYOffset;
};