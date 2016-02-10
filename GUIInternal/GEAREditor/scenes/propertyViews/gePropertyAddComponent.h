#ifndef GEPROPERTYADDCOMPONENT_H
#define GEPROPERTYADDCOMPONENT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../AssetImporter.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../OSSpecific/MenuCPPInterface.h"

class gePropertyAddComponent : public geTreeNode, public MGUIObserver
{
public:
	gePropertyAddComponent(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
	{
		setSize(m_cSize.x, 40.0f);

		addComponentToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "Components", this, fontmanager);
		addComponentToolBarDropMenuButton->setGUIObserver(this);
		addComponentToolBarDropMenuButton->setPos(15, 10);

		int base=0x00005200;
		stDropMenuItem* mitem = addComponentToolBarDropMenuButton->appendMenuItem("Physics", base++);
		addComponentToolBarDropMenuButton->appendMenuItem("RigidBody", base++, mitem);
		addComponentToolBarDropMenuButton->appendMenuItem("Box Collider", base++, mitem);
		addComponentToolBarDropMenuButton->appendMenuItem("Sphere Collider", base++, mitem);
		addComponentToolBarDropMenuButton->appendMenuItem("Capsule Collider", base++, mitem);
		addComponentToolBarDropMenuButton->appendMenuItem("Mesh Collider", base++, mitem);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertyAddComponent()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&vertexBufferClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		geFontManager::g_pFontArial10_80Ptr->drawString("Add components to the object3d.", addComponentToolBarDropMenuButton->getPos().x+addComponentToolBarDropMenuButton->getSize().x+20, geFontManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);

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
	virtual void onCommand(int cmd);

private:
	object3d* object3dPtr;
	geToolBarDropMenu* addComponentToolBarDropMenuButton;
};

#endif