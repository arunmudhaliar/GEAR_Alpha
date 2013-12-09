#ifndef GESTATICTEXTBOX_H
#define GESTATICTEXTBOX_H

#include "geGUIBase.h"
#include "../util/FontManager.h"

class geStaticTextBox : public geGUIBase
{
public:
	geStaticTextBox();
	geStaticTextBox(const char* name);

public:
	virtual ~geStaticTextBox();

	void create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float yoffset, gxFont* pFont);
	virtual void draw();

protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

	virtual void onSetName();

private:
	stVertexBuffer m_cVBClientArea;
	stVertexBuffer m_cVBSelectionArea;
	float m_cVBClientAreaLine[10];
	gxFont* m_pFont;
	float m_fYOffset;
};

#endif