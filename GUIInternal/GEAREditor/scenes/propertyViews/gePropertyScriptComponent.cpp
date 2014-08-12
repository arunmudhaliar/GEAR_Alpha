#include "gePropertyScriptComponent.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

gePropertyScriptComponent::gePropertyScriptComponent(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	geTreeNode(renderer, parent, name, sprite, 10)
{

	m_pWindowColumn = new geWindowColumn();
	m_pWindowColumn->create(m_pRenderer, this, 10, 300.0f, 10.0f, 0.4f);

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();

	setSize(m_cSize.x, 40.0f);
}

gePropertyScriptComponent::~gePropertyScriptComponent()
{
}

void gePropertyScriptComponent::drawNode()
{
	drawRect(&m_cVBClientArea);

	//geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

void gePropertyScriptComponent::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{

}

void gePropertyScriptComponent::populatePropertyOfMonoScripts(object3d* obj, monoScriptObjectInstance* monoScript)
{
	m_pObject3dPtr=obj;

	/*
	//destroy the controls in the column
	for(std::vector<stWindowColumnRow*>::iterator it = m_pWindowColumn->getRowList()->begin(); it != m_pWindowColumn->getRowList()->end(); ++it)
	{
		stWindowColumnRow* row = *it;
		for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
		{
			geGUIBase* control = *ctrl_it;
			m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), control), m_vControls.end());
			GE_DELETE(control);
		}
		row->getControlList()->clear();
		GE_DELETE(row);
	}
	m_pWindowColumn->getRowList()->clear();
	//
	*/


	geGUIBase* lastControl=NULL;
	////window column
	////for(int x=0;x<m_pObject3dPtr->getMonoScriptInstanceCount();x++)
	//{
	//	stWindowColumnRow* firstrow = m_pWindowColumn->addRow("Script");
	//	geStaticTextBox* scripteditbox = new geStaticTextBox("");
	//	scripteditbox->create(m_pRenderer, this, monoScript->getScriptPtr()->getMonoScript().c_str(), 0, 0, -7, geGUIManager::g_pFontArial10_80Ptr);
	//	m_pWindowColumn->addControl(firstrow, scripteditbox, 16.0f);
	//	lastControl=scripteditbox;
	//}

	//mono vars
	for(int x=0;x<monoScript->getScriptPtr()->getMonoVarCount();x++)
	{
		stWindowColumnRow* row = m_pWindowColumn->addRow(monoScript->getScriptPtr()->getMonoVarName(x));
		geTextBox* variableeditbox = new geTextBox("");
		variableeditbox->create(m_pRenderer, this, "", 0, 0, 30, 15);
		m_pWindowColumn->addControl(row, variableeditbox, 16.0f);
		lastControl=variableeditbox;
	}

	//

	if(lastControl)
		setSize(m_cSize.x, lastControl->getPos().y+lastControl->getSize().y+15);
}

void gePropertyScriptComponent::onButtonClicked(geGUIBase* btn)
{

}