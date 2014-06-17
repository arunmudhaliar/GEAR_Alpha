#include "gePropertyObject3d.h"
#include "../../EditorApp.h"
#include "../../../resource.h"

gePropertyObject3d::gePropertyObject3d(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	setSize(m_cSize.x, 60.0f);

	//geButton* btn=new geButton();
	//btn->create(this, "button1", 40, 10);

	m_pPushBtn_Object3dVisible = new gePushButton("");
	m_pPushBtn_Object3dVisible->create(renderer, this, "", 15, 10);
	m_pPushBtn_Object3dVisible->setGUIObserver(this);

	m_pTextBoxMeshName = new geTextBox("MeshName");
	m_pTextBoxMeshName->create(renderer, this, "MeshName", 35, 10, 200, 16);

	m_pPushBtn_Object3dStatic = new gePushButton("");
	m_pPushBtn_Object3dStatic->create(renderer, this, "", m_pTextBoxMeshName->getPos().x+m_pTextBoxMeshName->getSize().x+10, 10);
	m_pPushBtn_Object3dStatic->setGUIObserver(this);

	m_pTagDropDownMenu=new geToolBarDropMenu(m_pRenderer, "Tag", this);
	m_pTagDropDownMenu->setGUIObserver(this);
	m_pTagDropDownMenu->setPos(35, 35);
	m_pTagDropDownMenu->appendMenuItem("Tag0", 0x00006000);
	m_pTagDropDownMenu->appendMenuItem("Tag1", 0x00006001);
	m_pTagDropDownMenu->setMenuItem(0x00006000);

	m_pLayerDropDownMenu=new geToolBarDropMenu(m_pRenderer, "Layer", this);
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
	drawRect(&m_cVBClientArea);

	geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Static", m_pPushBtn_Object3dStatic->getPos().x+m_pPushBtn_Object3dStatic->getSize().x+5, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()+5, m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Tag", 10, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Layer", m_pLayerDropDownMenu->getPos().x-37, geGUIManager::g_pFontArial10_84Ptr->getLineHeight()+33, m_cSize.x);

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
		if(!m_pPushBtn_Object3dVisible->isCheck())
			m_pObject3dPtr->setBaseFlag(object3d::eObject3dBaseFlag_Visible);
		else
			m_pObject3dPtr->reSetBaseFlag(object3d::eObject3dBaseFlag_Visible);
	}
	else if(btn==m_pPushBtn_Object3dStatic /*&& m_pPushBtn_Object3dStatic->isButtonPressed()*/)
	{
		bool bRecursive=false;
		if(m_pObject3dPtr->getChildCount())
		{
			LRESULT result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNOCANCEL_ITERATECHILDS_BOX), EditorApp::getMainWindowHandle(), reinterpret_cast<DLGPROC>(YesNoCancel_DlgBox));
			if(result==IDYES)
				bRecursive=true;
			else if(result==IDCANCEL)
			{
				if(!m_pPushBtn_Object3dStatic->isCheck())
					m_pPushBtn_Object3dStatic->buttonNormal(true);
				else
					m_pPushBtn_Object3dStatic->buttonPressed(true);
				return;
			}

		}
		if(!m_pPushBtn_Object3dStatic->isCheck())
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
		m_pObject3dPtr->setLayer(cmd-0x00006500, true);
		m_pLayerDropDownMenu->setMenuItem(cmd);
	}
}