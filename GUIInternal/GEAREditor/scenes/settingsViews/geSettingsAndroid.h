#ifndef GESETTINGSANDROID_H
#define GESETTINGSANDROID_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../gui/geWindowColumn.h"

class geSettingsAndroid : public geTreeNode, public MGUIObserver
{
public:
	geSettingsAndroid(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
	{
		setSize(m_cSize.x, 100.0f);

		char* buffer;
		//ant home
		buffer=getenv("ANT_HOME");
		if(buffer==NULL)
		{
            antRootPathTextBox = new geTextBox("ANT_HOME environment variable not set.", fontManagerGUI);
            antRootPathTextBox->create(renderer, this, "ANT_HOME environment variable not set.", 0, 0, 300, 16);
		}
		else
		{
			antRootPathTextBox = new geTextBox(buffer, fontManagerGUI);
			antRootPathTextBox->create(renderer, this, buffer, 0, 0, 300, 16);
		}

		//android root
		buffer=getenv("ANDROID_ROOT");
		if(buffer==NULL)
		{

            androidSDKPathTextBox = new geTextBox("ANDROID_ROOT environment variable not set.", fontManagerGUI);
            androidSDKPathTextBox->create(renderer, this, "ANDROID_ROOT environment variable not set.", 0, 0, 300, 16);
		}
		else
		{
			androidSDKPathTextBox = new geTextBox(buffer, fontManagerGUI);
			androidSDKPathTextBox->create(renderer, this, buffer, 0, 0, 300, 16);
		}
        
		//package name
		androidBundleIdentifierTextBox = new geTextBox("com.example.app", fontManagerGUI);
		androidBundleIdentifierTextBox->create(renderer, this, "com.example.app", 0, 0, 300, 16);

		//bundle version
		appVersionTextBox = new geTextBox("1.0.0", fontManagerGUI);
		appVersionTextBox->create(renderer, this, "1.0.0", 0, 0, 300, 16);

		//bundle version code
		appVersionCodeTextBox = new geTextBox("1", fontManagerGUI);
		appVersionCodeTextBox->create(renderer, this, "1", 0, 0, 300, 16);

		//window column
		geWindowColumn* pWindowColumn = new geWindowColumn(fontManagerGUI);
		pWindowColumn->create(rendererGUI, this, 10, 300.0f, 10.0f, 0.35f);
		stWindowColumnRow* row = pWindowColumn->addRow("ANT_HOME");
		pWindowColumn->addControl(row, antRootPathTextBox);
		row = pWindowColumn->addRow("ANDROID_ROOT");
		pWindowColumn->addControl(row, androidSDKPathTextBox);
		row = pWindowColumn->addRow("Bundle identifier");
		pWindowColumn->addControl(row, androidBundleIdentifierTextBox);
		row = pWindowColumn->addRow("Bundle version");
		pWindowColumn->addControl(row, appVersionTextBox);
		row = pWindowColumn->addRow("Bundle version code");
		pWindowColumn->addControl(row, appVersionCodeTextBox);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~geSettingsAndroid()
	{
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
	geTextBox* antRootPathTextBox;
	geTextBox* androidSDKPathTextBox;
	geTextBox* androidBundleIdentifierTextBox;
	geTextBox* appVersionTextBox;
	geTextBox* appVersionCodeTextBox;
};

#endif