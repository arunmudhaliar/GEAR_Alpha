#ifndef GEPROPERTYANIMATIONSET_H
#define GEPROPERTYANIMATIONSET_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../../../GEAREngine/src/core/object3d.h"
#include "../../../../GEAREngine/src/core/gxAnimationSet.h"

class gePropertyAnimationSet : public geTreeNode, public MGUIObserver
{
public:
	gePropertyAnimationSet(rendererGL10* renderer, geGUIBase* parent, object3d* ownerObj, gxAnimationSet* animSet, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, animSet->getAnimationName(), sprite, fontmanager, 10)
	{
		m_pButtonSetAnimation=NULL;
		m_pOwnerObj=ownerObj;
		m_pAnimSetPtr=animSet;
		setSize(m_cSize.x, m_cSize.y*1.2f);

		m_pButtonSetAnimation = new geButton("", fontmanager);
		m_pButtonSetAnimation->create(renderer, this, "Set", 0, 3);
		m_pButtonSetAnimation->setSize(m_pButtonSetAnimation->getSize().x, m_pButtonSetAnimation->getSize().y);
		m_pButtonSetAnimation->setGUIObserver(this);

		setNodeColor(0.21f, 0.21f, 0.21f);
		//setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertyAnimationSet()
	{
	}

	virtual void onSize(float cx, float cy, int flag)
	{
		if(m_pButtonSetAnimation)
		{
			if(cx>m_pButtonSetAnimation->getSize().x*5.0f)
				m_pButtonSetAnimation->setPos(cx-(m_pButtonSetAnimation->getSize().x+30), 3);
		}

		geTreeNode::onSize(cx, cy, flag);
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&vertexBufferClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			tvnode->draw();
		}

		if(sprite)
			sprite->draw();
	}

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	virtual void onButtonClicked(geGUIBase* btn)
	{
		if(btn==m_pButtonSetAnimation && m_pButtonSetAnimation->isButtonPressed())
		{
			gxAnimation* animationController = m_pOwnerObj->createAnimationController();
			if(animationController->getActiveAnimationSet()==m_pAnimSetPtr)
			{
				if(animationController->isPlaying())
				{
					animationController->stop();
				}
				else
				{
					animationController->play(m_pAnimSetPtr);
				}
			}
			else
			{
				m_pOwnerObj->applyAnimationSetRecursive(m_pAnimSetPtr);
				animationController->play(m_pAnimSetPtr);
			}
		}
	}

	object3d* m_pOwnerObj;
	geButton* m_pButtonSetAnimation;
	gxAnimationSet* m_pAnimSetPtr;
};

#endif