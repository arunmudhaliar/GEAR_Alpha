#ifndef GEPROPERTYADDCOMPONENT_H
#define GEPROPERTYADDCOMPONENT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../AssetImporter.h"
#include "../../gui/geToolBarDropMenu.h"

class gePropertyAddComponent : public geTreeNode, public MGUIObserver
{
public:
	gePropertyAddComponent(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(renderer, parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 40.0f);

		m_pAddComponentToolBarDropMenuBtnPtr=new geToolBarDropMenu(m_pRenderer, "Components", this);
		m_pAddComponentToolBarDropMenuBtnPtr->setGUIObserver(this);
		m_pAddComponentToolBarDropMenuBtnPtr->setPos(15, 10);

		int base=0x00005200;
		geToolBarDropMenu::stDropMenuItem* mitem = m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Physics", base++);
		m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("RigidBody", base++, mitem);
		m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Box Collider", base++, mitem);
		m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Sphere Collider", base++, mitem);
		m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Capsule Collider", base++, mitem);
		m_pAddComponentToolBarDropMenuBtnPtr->appendMenuItem("Mesh Collider", base++, mitem);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyAddComponent()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		geGUIManager::g_pFontArial10_80Ptr->drawString("Add components to the object3d.", m_pAddComponentToolBarDropMenuBtnPtr->getPos().x+m_pAddComponentToolBarDropMenuBtnPtr->getSize().x+20, geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);

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
	virtual void onCommand(int cmd);

private:
	object3d* m_pObject3dPtr;
	geToolBarDropMenu* m_pAddComponentToolBarDropMenuBtnPtr;
};

#endif