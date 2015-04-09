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
#ifdef _WIN32
		setSize(m_cSize.x, 100.0f);

		//m_pPushBtn_Object3dVisible = new gePushButton("");
		//m_pPushBtn_Object3dVisible->create(renderer, this, "", 15, 10);
		//m_pPushBtn_Object3dVisible->setGUIObserver(this);

		char buffer[FILENAME_MAX];
		//ant home
		memset(buffer, 0, sizeof(buffer));
		int ret=GetEnvironmentVariable("ANT_HOME", buffer, sizeof(buffer));
		if(ret==0)
		{
			if(GetLastError()==ERROR_ENVVAR_NOT_FOUND)
			{
				m_pTextBoxAntRoot = new geTextBox("ANT_HOME environment variable not set.");
				m_pTextBoxAntRoot->create(renderer, this, "ANT_HOME environment variable not set.", 0, 0, 300, 16);
			}
		}
		else
		{
			m_pTextBoxAntRoot = new geTextBox(buffer);
			m_pTextBoxAntRoot->create(renderer, this, buffer, 0, 0, 300, 16);
		}

		//android root
		memset(buffer, 0, sizeof(buffer));
		ret=GetEnvironmentVariable("ANDROID_ROOT", buffer, sizeof(buffer));
		if(ret==0)
		{
			if(GetLastError()==ERROR_ENVVAR_NOT_FOUND)
			{
				m_pTextBoxAndroidRoot = new geTextBox("ANDROID_ROOT environment variable not set.");
				m_pTextBoxAndroidRoot->create(renderer, this, "ANDROID_ROOT environment variable not set.", 0, 0, 300, 16);
			}
		}
		else
		{
			m_pTextBoxAndroidRoot = new geTextBox(buffer);
			m_pTextBoxAndroidRoot->create(renderer, this, buffer, 0, 0, 300, 16);
		}
		//package name
		m_pTextBoxBundleIdentifier = new geTextBox("com.example.app");
		m_pTextBoxBundleIdentifier->create(renderer, this, "com.example.app", 0, 0, 300, 16);

		//bundle version
		m_pTextBoxBundleVersion = new geTextBox("1.0.0");
		m_pTextBoxBundleVersion->create(renderer, this, "1.0.0", 0, 0, 300, 16);

		//bundle version code
		m_pTextBoxBundleVersionCode = new geTextBox("1");
		m_pTextBoxBundleVersionCode->create(renderer, this, "1", 0, 0, 300, 16);

		//window column
		geWindowColumn* pWindowColumn = new geWindowColumn();
		pWindowColumn->create(m_pRenderer, this, 10, 300.0f, 10.0f, 0.35f);
		stWindowColumnRow* row = pWindowColumn->addRow("ANT_HOME");
		pWindowColumn->addControl(row, m_pTextBoxAntRoot);
		row = pWindowColumn->addRow("ANDROID_ROOT");
		pWindowColumn->addControl(row, m_pTextBoxAndroidRoot);
		row = pWindowColumn->addRow("Bundle identifier");
		pWindowColumn->addControl(row, m_pTextBoxBundleIdentifier);
		row = pWindowColumn->addRow("Bundle version");
		pWindowColumn->addControl(row, m_pTextBoxBundleVersion);
		row = pWindowColumn->addRow("Bundle version code");
		pWindowColumn->addControl(row, m_pTextBoxBundleVersionCode);

		//row = pWindowColumn->addRow("Linear Attenuation");
		//pWindowColumn->addControl(row, m_pHorizontalSlider_LinearAttenuation, 20.0f);
		//row = pWindowColumn->addRow("Quadratic Attenuation");
		//pWindowColumn->addControl(row, m_pHorizontalSlider_QuadraticAttenuation, 20.0f);
		//

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
#endif
	}

	virtual ~geSettingsAndroid()
	{
	}

	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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
	geTextBox* m_pTextBoxAntRoot;
	geTextBox* m_pTextBoxAndroidRoot;
	geTextBox* m_pTextBoxBundleIdentifier;
	geTextBox* m_pTextBoxBundleVersion;
	geTextBox* m_pTextBoxBundleVersionCode;

	//gePushButton* m_pPushBtn_Object3dVisible;
};

#endif