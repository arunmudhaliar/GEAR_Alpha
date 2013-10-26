#include "gePropertyBlurProcessor.h"
#include "../../EditorApp.h"

bool calculatePosOfMyChild2(geGUIBase* compareme, geGUIBase* parent, int& x, int& y);

void gePropertyBlurProcessor::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pPushBtn_Object3dVisible)
	{
		HMENU hPopupMenu = CreatePopupMenu();
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004003, "Directional Light");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004002, "Point Light");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
		//geTreeNode* selectedNode=m_cGameObjectsTreeView.getSelectedNode();
		int disableFlag = 0;//(selectedNode)?0:MF_DISABLED;
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING | disableFlag, 0x00004001, "Create Object on Selected Node");
		InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 0x00004000, "Create Object");

		//
		geGUIBase* baseGUI=btn;
		geGUIBase* rootTVNode=btn;
		geVector2f absPos;
		while(baseGUI)
		{
			rootTVNode=baseGUI;
			baseGUI=baseGUI->getParent();
		}

		if(rootTVNode && rootTVNode->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			geGUIBase* treeView = ((geTreeNode*)rootTVNode)->getParentTreeView();
			int x, y;
			x=y=0;
			calculatePosOfMyChild2(btn, rootTVNode, x, y);

			POINT pt;
			pt.x=x+treeView->getPositionOnScreen().x;
			pt.y=y-treeView->getPositionOnScreen().y;
			ClientToScreen(EditorApp::getMainWindowHandle(), &pt);
			TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, EditorApp::getMainWindowHandle(), NULL);
		}
	}
}

bool calculatePosOfMyChild2(geGUIBase* compareme, geGUIBase* parent, int& x, int& y)
{
	if(parent)
	{
		if(parent->getGUIID()==GEGUI_TREEVIEW_NODE)
		{
			y+=parent->getSize().y;

			if(((geTreeNode*)parent)->isOpenNode())
			{
				std::vector<geGUIBase*>* childList=parent->getChildControls();
				for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
				{
					geGUIBase* tvnode = *it;
					if(compareme==tvnode)
					{
						x+=compareme->getPos().x-((geTreeNode*)parent)->getXOffset();
						y+=compareme->getSize().y;
						y-=parent->getSize().y;

						compareme=compareme;
						//calculate and return
						return true;
					}
					if(calculatePosOfMyChild2(compareme, tvnode, x, y))
						return true;
				}
				x+=((geTreeNode*)parent)->getXOffset();
			}
		}
	}

	return false;
}
