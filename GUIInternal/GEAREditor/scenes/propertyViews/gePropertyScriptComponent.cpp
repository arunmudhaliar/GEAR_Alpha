#include "gePropertyScriptComponent.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

gePropertyScriptComponent::gePropertyScriptComponent(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{

	windowColumnControl = new geWindowColumn(fontManagerGUI);
	windowColumnControl->create(rendererGUI, this, 10, 300.0f, 10.0f, 0.4f);

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
	drawRect(&vertexBufferClientArea);

	//geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

void gePropertyScriptComponent::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{

}

void gePropertyScriptComponent::populatePropertyOfMonoScripts(object3d* obj, monoScriptObjectInstance* monoScriptobject)
{
	object3dPtr=obj;

	/*
	//destroy the controls in the column
	for(std::vector<stWindowColumnRow*>::iterator it = windowColumnControl->getRowList()->begin(); it != windowColumnControl->getRowList()->end(); ++it)
	{
		stWindowColumnRow* row = *it;
		for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
		{
			geGUIBase* control = *ctrl_it;
			childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), control), childControlList.end());
			GE_DELETE(control);
		}
		row->getControlList()->clear();
		GE_DELETE(row);
	}
	windowColumnControl->getRowList()->clear();
	//
	*/


	geGUIBase* lastControl=NULL;
	////window column
	////for(int x=0;x<object3dPtr->getMonoScriptInstanceCount();x++)
	//{
	//	stWindowColumnRow* firstrow = windowColumnControl->addRow("Script");
	//	geStaticTextBox* scripteditbox = new geStaticTextBox("");
	//	scripteditbox->create(rendererGUI, this, monoScript->getScriptPtr()->getScriptFileName().c_str(), 0, 0, -7, geFontManager::g_pFontArial10_80Ptr);
	//	windowColumnControl->addControl(firstrow, scripteditbox, 16.0f);
	//	lastControl=scripteditbox;
	//}

	//mono vars
	for(int x=0;x<monoScriptobject->getScriptPtr()->getMonoVarCount();x++)
	{
		stWindowColumnRow* row = windowColumnControl->addRow(monoScriptobject->getScriptPtr()->getMonoVarName(x));
		geTextBox* variableeditbox = new geTextBox("", fontManagerGUI);
		variableeditbox->create(rendererGUI, this, "", 0, 0, 30, 15);
		windowColumnControl->addControl(row, variableeditbox, 16.0f);
		lastControl=variableeditbox;
	}

	//

	if(lastControl)
		setSize(m_cSize.x, lastControl->getPos().y+lastControl->getSize().y+15);
}

void gePropertyScriptComponent::onButtonClicked(geGUIBase* btn)
{

}