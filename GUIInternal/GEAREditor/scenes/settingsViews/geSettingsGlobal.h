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
	geSettingsGlobal(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
	{
		setSize(m_cSize.x, 40.0f);

		vsyncToggle = new gePushButton("", fontmanager);
		vsyncToggle->create(renderer, this, "", 15, 10);
		vsyncToggle->setGUIObserver(this);
		vsyncToggle->setCheck(true);

		//window column
		geWindowColumn* pWindowColumn = new geWindowColumn(fontManagerGUI);
		pWindowColumn->create(rendererGUI, this, 10, 300.0f, 10.0f, 0.35f);
		stWindowColumnRow* row = pWindowColumn->addRow("VSync");
		pWindowColumn->addControl(row, vsyncToggle);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~geSettingsGlobal()
	{
	}

	virtual void onButtonClicked(geGUIBase* btn)
	{
		if(btn==vsyncToggle)
		{
			if(vsyncToggle->isCheck())
				setVSync(1);
			else
				setVSync(0);
		}
	}

	void setVSync(int interval=1)
	{
#ifdef GEAR_WINDOWS
		const char *extensions = (const char*)glGetString( GL_EXTENSIONS );
		if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
			return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
		else
		{
			//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

			if( wglSwapIntervalEXT )
				wglSwapIntervalEXT(interval);
		}
#endif
	}

	virtual void drawNode()
	{
		drawRect(&vertexBufferClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		if(childControlList.size() && hasAtleastOneTreeNodeChild)
		{
			if(is_OpenNode)
				drawTriangle(&vertexBufferToggleButtonArray[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&vertexBufferToggleButtonArray[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}

		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			tvnode->draw();
		}

		if(sprite)
			sprite->draw();
	}

private:
	gePushButton* vsyncToggle;
};

#endif