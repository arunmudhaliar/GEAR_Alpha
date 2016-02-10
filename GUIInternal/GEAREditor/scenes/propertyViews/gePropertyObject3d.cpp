#include "gePropertyObject3d.h"
#include "../../EditorApp.h"
#include "../../OSSpecific/ConfirmationDialog.h"

gePropertyObject3d::gePropertyObject3d(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 60.0f);

	//geButton* btn=new geButton();
	//btn->create(this, "button1", 40, 10);

	object3dVisibleToggle = new gePushButton("", fontmanager);
	object3dVisibleToggle->create(renderer, this, "", 15, 10);
	object3dVisibleToggle->setGUIObserver(this);

	meshNameTextBox = new geTextBox("MeshName", fontManagerGUI);
	meshNameTextBox->create(renderer, this, "MeshName", 35, 10, 200, 16);

	object3dStaticToggle = new gePushButton("", fontmanager);
	object3dStaticToggle->create(renderer, this, "", meshNameTextBox->getPos().x+meshNameTextBox->getSize().x+10, 10);
	object3dStaticToggle->setGUIObserver(this);

	tagDropDownMenu=new geToolBarDropMenu(rendererGUI, "Tag", this, fontmanager);
	tagDropDownMenu->setGUIObserver(this);
	tagDropDownMenu->setPos(35, 35);
	tagDropDownMenu->appendMenuItem("Tag0", 0x00006000);
	tagDropDownMenu->appendMenuItem("Tag1", 0x00006001);
	tagDropDownMenu->setMenuItem(0x00006000);

	layerDropDownMenu=new geToolBarDropMenu(rendererGUI, "Layer", this, fontmanager);
	layerDropDownMenu->setGUIObserver(this);
	layerDropDownMenu->setPos(150, 35);

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyObject3d::~gePropertyObject3d()
{
}

//virtual void draw();
void gePropertyObject3d::drawNode()
{
	drawRect(&vertexBufferClientArea);

	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Static", object3dStaticToggle->getPos().x+object3dStaticToggle->getSize().x+5, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+5, m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Tag", 10, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Layer", layerDropDownMenu->getPos().x-37, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);

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

void gePropertyObject3d::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{

}

void gePropertyObject3d::populatePropertyOfObject(object3d* obj)
{
	object3dPtr=obj;
	meshNameTextBox->setName(obj->getName());
	object3dVisibleToggle->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible));
	object3dStaticToggle->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Static));

	layerDropDownMenu->clearMenu();
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();
	for(int x=0;x<MAX_LAYER;x++)
	{
		Layer* layer = layerManager->getLayer(x);
		layerDropDownMenu->appendMenuItem(layer->getLayerName(), 0x00006500+x);
	}

	if(object3dPtr->getLayer()>=0)
	{
		layerDropDownMenu->setMenuItem(0x00006500+object3dPtr->getLayer());
	}
}

void gePropertyObject3d::onButtonClicked(geGUIBase* btn)
{
	if(btn==object3dVisibleToggle)
	{
		if(object3dVisibleToggle->isCheck())
			object3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Visible, true);
		else
			object3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Visible, true);
	}
	else if(btn==object3dStaticToggle /*&& object3dStaticToggle->isButtonPressed()*/)
	{
		bool bRecursive=false;
		bool bOldCheck=object3dStaticToggle->isCheck();
		if(object3dPtr->getChildCount())
		{
			
			ConfirmationDialog::ConfirmationDialogButton result = ConfirmationDialog::ShowConfirmationDialog("Do you want to change this object and its child or not ?");
            if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_YES)
				bRecursive=true;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_NO || result==ConfirmationDialog::ConfirmationDialogButton::BTN_CANCEL)
			{
				object3dStaticToggle->setCheck(!bOldCheck);
				return;
			}

		}
		if(bOldCheck)
		{
			object3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Static, bRecursive);
			//object3dStaticToggle->buttonPressed(true);
		}
		else
		{
			object3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Static, bRecursive);
			//object3dStaticToggle->buttonNormal(true);
		}
		object3dStaticToggle->refresh();
		//TODO: We need to remove/add to octree based on this flag.
	}
}

void gePropertyObject3d::onCommand(int cmd)
{
	if(cmd>=0x00006500 && cmd<0x00006500+MAX_LAYER)
	{
		bool bRecursive=false;
		if(object3dPtr->getChildCount())
		{
			ConfirmationDialog::ConfirmationDialogButton result = ConfirmationDialog::ShowConfirmationDialog("Do you want to change this object and its child or not ?");
			if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_YES)
				bRecursive=true;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_NO)
				bRecursive=false;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_CANCEL)
				return;
		}

		object3dPtr->setLayer(cmd-0x00006500, bRecursive);
		layerDropDownMenu->setMenuItem(cmd);
	}
}