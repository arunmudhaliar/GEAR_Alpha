#ifndef GESTATICTEXTBOX_H
#define GESTATICTEXTBOX_H

#include "geGUIBase.h"
#include "../util/geFontManager.h"

class geStaticTextBox : public geGUIBase
{
public:
	geStaticTextBox();
	geStaticTextBox(const char* name);

public:
	virtual ~geStaticTextBox();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float yoffset, geFont* pFont);
	virtual void draw();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onSetName();

private:
	stVertexBuffer m_cVBSelectionArea;
	float m_cVBClientAreaLine[10];
	geFont* m_pFont;
	float m_fYOffset;
};

#endif