#include "gePropertyObject3d.h"
#include "../../EditorApp.h"
#include "../../OSSpecific/ConfirmationDialog.h"

gePropertyObject3d::gePropertyObject3d(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 60.0f);

	//geButton* btn=new geButton();
	//btn->create(this, "button1", 40, 10);

	m_pPushBtn_Object3dVisible = new gePushButton("", fontmanager);
	m_pPushBtn_Object3dVisible->create(renderer, this, "", 15, 10);
	m_pPushBtn_Object3dVisible->setGUIObserver(this);

	m_pTextBoxMeshName = new geTextBox("MeshName", fontManagerGUI);
	m_pTextBoxMeshName->create(renderer, this, "MeshName", 35, 10, 200, 16);

	m_pPushBtn_Object3dStatic = new gePushButton("", fontmanager);
	m_pPushBtn_Object3dStatic->create(renderer, this, "", m_pTextBoxMeshName->getPos().x+m_pTextBoxMeshName->getSize().x+10, 10);
	m_pPushBtn_Object3dStatic->setGUIObserver(this);

	m_pTagDropDownMenu=new geToolBarDropMenu(rendererGUI, "Tag", this, fontmanager);
	m_pTagDropDownMenu->setGUIObserver(this);
	m_pTagDropDownMenu->setPos(35, 35);
	m_pTagDropDownMenu->appendMenuItem("Tag0", 0x00006000);
	m_pTagDropDownMenu->appendMenuItem("Tag1", 0x00006001);
	m_pTagDropDownMenu->setMenuItem(0x00006000);

	m_pLayerDropDownMenu=new geToolBarDropMenu(rendererGUI, "Layer", this, fontmanager);
	m_pLayerDropDownMenu->setGUIObserver(this);
	m_pLayerDropDownMenu->setPos(150, 35);

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
	geFontManager::g_pFontArial10_84Ptr->drawString("Static", m_pPushBtn_Object3dStatic->getPos().x+m_pPushBtn_Object3dStatic->getSize().x+5, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+5, m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Tag", 10, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Layer", m_pLayerDropDownMenu->getPos().x-37, geFontManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);

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
	m_pObject3dPtr=obj;
	m_pTextBoxMeshName->setName(obj->getName());
	m_pPushBtn_Object3dVisible->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible));
	m_pPushBtn_Object3dStatic->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Static));

	m_pLayerDropDownMenu->clearMenu();
	LayerManager* layerManager = monoWrapper::mono_engine_getWorld(0)->getLayerManager();
	for(int x=0;x<MAX_LAYER;x++)
	{
		Layer* layer = layerManager->getLayer(x);
		m_pLayerDropDownMenu->appendMenuItem(layer->getLayerName(), 0x00006500+x);
	}

	if(m_pObject3dPtr->getLayer()>=0)
	{
		m_pLayerDropDownMenu->setMenuItem(0x00006500+m_pObject3dPtr->getLayer());
	}
}

void gePropertyObject3d::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pPushBtn_Object3dVisible)
	{
		if(m_pPushBtn_Object3dVisible->isCheck())
			m_pObject3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Visible, true);
		else
			m_pObject3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Visible, true);
	}
	else if(btn==m_pPushBtn_Object3dStatic /*&& m_pPushBtn_Object3dStatic->isButtonPressed()*/)
	{
		bool bRecursive=false;
		bool bOldCheck=m_pPushBtn_Object3dStatic->isCheck();
		if(m_pObject3dPtr->getChildCount())
		{
			
			ConfirmationDialog::ConfirmationDialogButton result = ConfirmationDialog::ShowConfirmationDialog("Do you want to change this object and its child or not ?");
            if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_YES)
				bRecursive=true;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_NO || result==ConfirmationDialog::ConfirmationDialogButton::BTN_CANCEL)
			{
				m_pPushBtn_Object3dStatic->setCheck(!bOldCheck);
				return;
			}

		}
		if(bOldCheck)
		{
			m_pObject3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Static, bRecursive);
			//m_pPushBtn_Object3dStatic->buttonPressed(true);
		}
		else
		{
			m_pObject3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Static, bRecursive);
			//m_pPushBtn_Object3dStatic->buttonNormal(true);
		}
		m_pPushBtn_Object3dStatic->refresh();
		//TODO: We need to remove/add to octree based on this flag.
	}
}

void gePropertyObject3d::onCommand(int cmd)
{
	if(cmd>=0x00006500 && cmd<0x00006500+MAX_LAYER)
	{
		bool bRecursive=false;
		if(m_pObject3dPtr->getChildCount())
		{
			ConfirmationDialog::ConfirmationDialogButton result = ConfirmationDialog::ShowConfirmationDialog("Do you want to change this object and its child or not ?");
			if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_YES)
				bRecursive=true;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_NO)
				bRecursive=false;
			else if(result==ConfirmationDialog::ConfirmationDialogButton::BTN_CANCEL)
				return;
		}

		m_pObject3dPtr->setLayer(cmd-0x00006500, bRecursive);
		m_pLayerDropDownMenu->setMenuItem(cmd);
	}
}