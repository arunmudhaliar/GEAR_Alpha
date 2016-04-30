#ifndef GEPROPERTYANIMATIONSET_H
#define GEPROPERTYANIMATIONSET_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"
#include "../../../../GEAREngine/src/core/object3d.h"
#include "../../../../GEAREngine/src/animation/gxAnimationSet.h"

class gePropertyAnimationSet : public geTreeNode, public MGUIObserver
{
public:
	gePropertyAnimationSet(rendererGL10* renderer, geGUIBase* parent, object3d* ownerObj, gxAnimationSet* animSet, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, animSet->getAnimationName().c_str(), sprite, fontmanager, 10)
	{
		setAnimationButton=NULL;
		ownerObject3d=ownerObj;
		animationSet=animSet;
        REF_RETAIN(animationSet);
		setSize(m_cSize.x, m_cSize.y*1.2f);

		setAnimationButton = new geButton("", fontmanager);
		setAnimationButton->create(renderer, this, "Set", 0, 3);
		setAnimationButton->setSize(setAnimationButton->getSize().x, setAnimationButton->getSize().y);
		setAnimationButton->setGUIObserver(this);

		setNodeColor(0.21f, 0.21f, 0.21f);
		//setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertyAnimationSet()
	{
        REF_RELEASE(animationSet);
	}

	virtual void onSize(float cx, float cy, int flag)
	{
		if(setAnimationButton)
		{
			if(cx>setAnimationButton->getSize().x*5.0f)
				setAnimationButton->setPos(cx-(setAnimationButton->getSize().x+30), 3);
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
		if(btn==setAnimationButton && setAnimationButton->isButtonPressed())
		{
			gxAnimation* animationController = ownerObject3d->getAnimationController();
			if(animationController->getActiveAnimationSet()==animationSet)
			{
				if(animationController->isPlaying())
				{
					animationController->stop();
				}
				else
				{
					animationController->play(animationSet);
				}
			}
			else
			{
				ownerObject3d->applyAnimationSetRecursive(animationSet);
				animationController->play(animationSet);
			}
		}
	}

	object3d* ownerObject3d;
	geButton* setAnimationButton;
	gxAnimationSet* animationSet;
};

#endif