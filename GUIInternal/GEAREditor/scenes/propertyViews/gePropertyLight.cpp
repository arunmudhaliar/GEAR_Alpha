#include "gePropertyLight.h"
#include "../../EditorApp.h"

void gePropertyLight::onDragDrop(int x, int y, MDataObject* dropObject)
{

}

void gePropertyLight::drawNode()
{
	drawRect(&m_cVBClientArea);

	geGUIManager::g_pFontArial10_84Ptr->drawString("Diffuse", 40, 7+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Ambient", 40, 30+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Specular", 40, 57+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

void gePropertyLight::populatePropertyOfLight(object3d* obj)
{
	m_pLightPtr = (gxLight*)obj;
	vector4f diff(m_pLightPtr->getDiffuseColor());
	vector4f ambient(m_pLightPtr->getAmbientColor());
	vector4f specular(m_pLightPtr->getSpecularColor());

	m_pColorControl->setControlColor(diff.x, diff.y, diff.z, diff.w);
	m_pColorControlAmbient->setControlColor(ambient.x, ambient.y, ambient.z, ambient.w);
	m_pColorControlSpecular->setControlColor(specular.x, specular.y, specular.z, specular.w);
}

void gePropertyLight::onTextChange(geGUIBase* textbox)
{
}

void gePropertyLight::onColorChange(geGUIBase* colorControl)
{
	vector4f diff(m_pLightPtr->getDiffuseColor());
	vector4f ambient(m_pLightPtr->getAmbientColor());
	vector4f specular(m_pLightPtr->getSpecularColor());

	if(colorControl==m_pColorControl)
	{
		geVector4f newClr(m_pColorControl->getControlColor());
		m_pLightPtr->setDiffuseColor(vector4f(newClr.x, newClr.y, newClr.z, newClr.w));
	}
	else if(colorControl==m_pColorControlAmbient)
	{
		geVector4f newClr(m_pColorControlAmbient->getControlColor());
		m_pLightPtr->setAmbientColor(vector4f(newClr.x, newClr.y, newClr.z, newClr.w));
	}
	else if(colorControl==m_pColorControlSpecular)
	{
		geVector4f newClr(m_pColorControlAmbient->getControlColor());
		m_pLightPtr->setSpecularColor(vector4f(newClr.x, newClr.y, newClr.z, newClr.w));
	}
}

void gePropertyLight::onSliderChange(geGUIBase* slider)
{
}

//bool calculatePosOfMyChild(geGUIBase* compareme, geGUIBase* parent, int& x, int& y);
void gePropertyLight::onButtonClicked(geGUIBase* btn)
{
	if(m_pLightTypeToolBarDropMenuBtnPtr==btn)
	{
		//HMENU hPopupMenu = CreatePopupMenu();
		//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004003, "Directional Light");
		//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004002, "Point Light");
		//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		////geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
		//int disableFlag = 0;//(selectedNode)?0:MF_DISABLED;
		//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | disableFlag, 0x00004001, "Create Object on Selected Node");
		//InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004000, "Create Object");

		////
		//geGUIBase* baseGUI=btn;
		//geGUIBase* rootTVNode=btn;
		//geVector2f absPos;
		//while(baseGUI)
		//{
		//	rootTVNode=baseGUI;
		//	baseGUI=baseGUI->getParent();
		//}

		//if(rootTVNode && rootTVNode->getGUIID()==GEGUI_TREEVIEW_NODE)
		//{
		//	geGUIBase* treeView = ((geTreeNode*)rootTVNode)->getParentTreeView();
		//	int x, y;
		//	x=y=0;
		//	calculatePosOfMyChild(btn, rootTVNode, x, y);

		//	POINT pt;
		//	pt.x=x+treeView->getPositionOnScreen().x;
		//	pt.y=y-treeView->getPositionOnScreen().y;
		//	ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
		//	TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);

		//}
	}
}

//bool calculatePosOfMyChild(geGUIBase* compareme, geGUIBase* parent, int& x, int& y)
//{
//	if(parent)
//	{
//		if(parent->getGUIID()==GEGUI_TREEVIEW_NODE)
//		{
//			y+=parent->getSize().y;
//
//			if(((geTreeNode*)parent)->isOpenNode())
//			{
//				std::vector<geGUIBase*>* childList=parent->getChildControls();
//				for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
//				{
//					geGUIBase* tvnode = *it;
//					if(compareme==tvnode)
//					{
//						x+=compareme->getPos().x-((geTreeNode*)parent)->getXOffset();
//						y+=compareme->getSize().y;
//						y-=parent->getSize().y;
//
//						compareme=compareme;
//						//calculate and return
//						return true;
//					}
//					if(calculatePosOfMyChild(compareme, tvnode, x, y))
//						return true;
//				}
//				x+=((geTreeNode*)parent)->getXOffset();
//			}
//		}
//	}
//
//	return false;
//}