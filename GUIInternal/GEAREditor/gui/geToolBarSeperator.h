#ifndef GETOOLBARSEPERATOR_H
#define GETOOLBARSEPERATOR_H

#include "geToolBar.h"

class geToolBarSeperator : public geGUIBase
{
public:
	geToolBarSeperator(geGUIBase* parent, float width):
		geGUIBase(GEGUI_TOOLBAR_SEPERATOR, "ToolBar Seperator")
	{
		createBase(parent);

		setSize(width, parent->getSize().y);

		setColor(&m_cVBClientArea, 0.2, 0.2, 0.2, 1.0f, EGRADIENT_VERTICAL_UP, 0.45f);
		m_bImageLoaded=false;
	}

	virtual ~geToolBarSeperator()
	{
	}

	virtual void draw()
	{
		glPushMatrix();
		glTranslatef(m_cPos.x, m_cPos.y, 0);
		drawRect(&m_cVBClientArea);
		drawLine(m_cVBClientAreaLine, 0.1, 0.1, 0.1, 1.0f, 2, false);
		glPopMatrix();
	}

protected:
	virtual void onSize(float cx, float cy, int flag)
	{
		const float title_vertLst[8] =
		{
			cx-1,	0,
			-1,	0,
			cx-1,	cy,
			-1,	cy,
		};

		memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

		const float clientarea_linevertLst[4] =
		{
			cx-1,	0,
			cx-1,	cy,
		};
		memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	}

private:
	stVertexBuffer m_cVBClientArea;
	float m_cVBClientAreaLine[4];
	Sprite2Dx m_cSprite;
	bool m_bImageLoaded;
};

#endif