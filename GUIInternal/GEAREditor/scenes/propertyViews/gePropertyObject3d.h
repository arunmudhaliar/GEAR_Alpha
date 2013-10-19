#ifndef GEPROPERTYOBJECT3D_H
#define GEPROPERTYOBJECT3D_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"

class gePropertyObject3d : public geTreeNode, public MGUIObserver
{
public:
	gePropertyObject3d(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(renderer, parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 30.0f);

		//geButton* btn=new geButton();
		//btn->create(this, "button1", 40, 10);

		m_pPushBtn_Object3dVisible = new gePushButton("");
		m_pPushBtn_Object3dVisible->create(renderer, this, "", 15, 10);
		m_pPushBtn_Object3dVisible->setGUIObserver(this);

		m_pTextBoxMeshName = new geTextBox("MeshName");
		m_pTextBoxMeshName->create(renderer, this, "MeshName", 35, 10, 200, 16);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyObject3d()
	{
	}

	//virtual void draw();
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

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	void populatePropertyOfObject(object3d* obj)
	{
		m_pObject3dPtr=obj;
		m_pTextBoxMeshName->setName(obj->getName());

		m_pPushBtn_Object3dVisible->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible));
	}

	virtual void onButtonClicked(geGUIBase* btn)
	{
		if(btn==m_pPushBtn_Object3dVisible)
		{
			if(m_pPushBtn_Object3dVisible->isCheck())
				m_pObject3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Visible);
			else
				m_pObject3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Visible);
		}
	}

private:
	object3d* m_pObject3dPtr;
	geTextBox* m_pTextBoxMeshName;
	gePushButton* m_pPushBtn_Object3dVisible;
};

#endif