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

void gePropertyScriptComponent::populatePropertyOfMonoScripts(monoScriptObjectInstance* monoScriptobject)
{
	geGUIBase* lastControl=NULL;

	//mono fields
	for(int x=0;x<monoScriptobject->getScriptPtr()->getMonoFieldCount();x++)
	{
        auto mvartypename = monoScriptobject->getScriptPtr()->getMonoFieldTypeName(x);
        if(strncmp(mvartypename, "MonoGEAR.gxColor", strlen("MonoGEAR.gxColor"))==0)
        {
            stWindowColumnRow* row = windowColumnControl->addRow(monoScriptobject->getScriptPtr()->getMonoFieldName(x));
            geColorControl* colorControl = new geColorControl(fontManagerGUI);
            colorControl->create(rendererGUI, this, 100, 10);
            colorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
            colorControl->setGUIObserver(this);
            windowColumnControl->addControl(row, colorControl, 16.0f);
            lastControl=colorControl;
        }
        else
        {
            stWindowColumnRow* row = windowColumnControl->addRow(monoScriptobject->getScriptPtr()->getMonoFieldName(x));
            geTextBox* variableeditbox = new geTextBox("", fontManagerGUI);
            variableeditbox->create(rendererGUI, this, mvartypename, 0, 0);
            windowColumnControl->addControl(row, variableeditbox, 16.0f);
            lastControl=variableeditbox;
        }
	}

    //mono properties
    for(auto property : monoScriptobject->getScriptPtr()->getMonoProperties())
    {
        auto propertyName = monoScriptobject->getScriptPtr()->getPropertyName(property);
        auto getType = monoScriptobject->getScriptPtr()->getPropertyGetMethodType(property);
        const char* typeName = nullptr;
        if(getType )
        {
            typeName = monoScriptobject->getScriptPtr()->getMonoPropertyTypeName(getType);
        }
        
        if(getType && strncmp(typeName, "MonoGEAR.gxColor", strlen("MonoGEAR.gxColor"))==0)
        {
            stWindowColumnRow* row = windowColumnControl->addRow(propertyName);
            geColorControl* colorControl = new geColorControl(fontManagerGUI);
            colorControl->create(rendererGUI, this, 100, 10);
            colorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
            colorControl->setGUIObserver(this);
            windowColumnControl->addControl(row, colorControl, 16.0f);
            lastControl=colorControl;
        }
        else
        {
            stWindowColumnRow* row = windowColumnControl->addRow(propertyName);
            geTextBox* propertyeditbox = new geTextBox("", fontManagerGUI);
            propertyeditbox->create(rendererGUI, this, propertyName, 0, 0);
            windowColumnControl->addControl(row, propertyeditbox, 16.0f);
            lastControl=propertyeditbox;
        }
    }
	//

	if(lastControl)
		setSize(m_cSize.x, lastControl->getPos().y+lastControl->getSize().y+15);
}

void gePropertyScriptComponent::onButtonClicked(geGUIBase* btn)
{

}