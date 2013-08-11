#ifndef GEPROPERTYANIMATIONSET_H
#define GEPROPERTYANIMATIONSET_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"

class gePropertyAnimationSet : public geTreeNode, public MGUIObserver
{
public:
	gePropertyAnimationSet(geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(parent, name, sprite, 10)
	{
		setSize(m_cSize.x, m_cSize.y);

		setNodeColor(0.21f, 0.21f, 0.21f);
		//setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyAnimationSet()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial12Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

		if(m_pSprite)
			m_pSprite->draw();
	}

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}
};

#endif