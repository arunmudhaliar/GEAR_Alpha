#ifndef GEPROPERTYSAVEMETADATA_H
#define GEPROPERTYSAVEMETADATA_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../AssetImporter.h"

class gePropertySaveMetaData : public geTreeNode, public MGUIObserver
{
public:
	gePropertySaveMetaData(geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 40.0f);

		m_pButtonSave = new geButton("");
		m_pButtonSave->create(this, "Save Meta", 15, 10);
		m_pButtonSave->setGUIObserver(this);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertySaveMetaData()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial12Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

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

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	void populatePropertyOfObject(object3d* obj)
	{
		m_pObject3dPtr=obj;
	}

	virtual void onButtonClicked(geGUIBase* btn);
	void saveMaterialRecursiveToMeta(object3d* obj);

private:
	object3d* m_pObject3dPtr;
	geTextBox* m_pTextBoxMeshName;
	geButton* m_pButtonSave;
};

#endif