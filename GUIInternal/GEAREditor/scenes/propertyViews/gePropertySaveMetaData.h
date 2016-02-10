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
	gePropertySaveMetaData(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
	{
		setSize(m_cSize.x, 40.0f);

		saveMetaDataButton = new geButton("", fontmanager);
		saveMetaDataButton->create(renderer, this, "Save Meta", 15, 10);
		saveMetaDataButton->setGUIObserver(this);
		saveMetaDataButton->setClientAreaPrimaryActiveForeColor(0.6f, 0.43f, 0.25f, 1.0f);
		saveMetaDataButton->setClientAreaSecondryActiveForeColor(0.6f, 0.43f, 0.25f, 1.0f);
		saveMetaDataButton->applyPrimaryColorToVBClientArea(EGRADIENT_VERTICAL_UP, 0.3f);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertySaveMetaData()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&vertexBufferClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		geFontManager::g_pFontArial10_80Ptr->drawString("Update meta file with the current change.", saveMetaDataButton->getPos().x+saveMetaDataButton->getSize().x+20, geFontManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);

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

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	void populatePropertyOfObject(object3d* obj)
	{
		object3dPtr=obj;
	}

	virtual void onButtonClicked(geGUIBase* btn);
	void saveMaterialRecursiveToMeta(object3d* obj);

private:
	object3d* object3dPtr;
	geTextBox* meshNameTextBox;
	geButton* saveMetaDataButton;
};

#endif