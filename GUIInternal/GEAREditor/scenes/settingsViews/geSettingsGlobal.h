#ifndef GESETTINGSGLOBAL_H
#define GESETTINGSGLOBAL_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../gui/geWindowColumn.h"

class geSettingsGlobal : public geTreeNode, public MGUIObserver
{
public:
	geSettingsGlobal(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(renderer, parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 40.0f);

		m_pPushBtnVsync = new gePushButton("");
		m_pPushBtnVsync->create(renderer, this, "", 15, 10);
		m_pPushBtnVsync->setGUIObserver(this);
		m_pPushBtnVsync->setCheck(true);

		//window column
		geWindowColumn* pWindowColumn = new geWindowColumn();
		pWindowColumn->create(m_pRenderer, this, 10, 300.0f, 10.0f, 0.35f);
		stWindowColumnRow* row = pWindowColumn->addRow("VSync");
		pWindowColumn->addControl(row, m_pPushBtnVsync);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~geSettingsGlobal()
	{
	}

	virtual void onButtonClicked(geGUIBase* btn)
	{
		if(btn==m_pPushBtnVsync)
		{
			if(m_pPushBtnVsync->isCheck())
				setVSync(1);
			else
				setVSync(0);
		}
	}

	void setVSync(int interval=1)
	{
		const char *extensions = (const char*)glGetString( GL_EXTENSIONS );
		if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
			return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
		else
		{
			//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

			if( wglSwapIntervalEXT )
				wglSwapIntervalEXT(interval);
		}
	}

	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
		{
			if(m_bNodeOpen)
				drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}

		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			tvnode->draw();
		}

		if(m_pSprite)
			m_pSprite->draw();
	}

private:
	gePushButton* m_pPushBtnVsync;
};

#endif