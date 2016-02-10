#ifndef GEPROPERTYBLURPROCESSOR_H
#define GEPROPERTYBLURPROCESSOR_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geGUIManager.h"

class gePropertyBlurProcessor : public geTreeNode, public MGUIObserver
{
public:
	gePropertyBlurProcessor(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	  geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
	{
		setSize(m_cSize.x, 30.0f);

		object3dVisibleToggle = new gePushButton("", fontmanager);
		object3dVisibleToggle->create(renderer, this, "", 15, 10);
		object3dVisibleToggle->setGUIObserver(this);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertyBlurProcessor()
	{
	}

	virtual void drawNode()
	{
		drawRect(&vertexBufferClientArea);

		geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	void populatePropertyOfBlurShader(gxHWShader* blurShader)
	{
		glslBlurShader=blurShader;
	}


	virtual void onSliderChange(geGUIBase* slider)
	{
	}

	virtual void onButtonClicked(geGUIBase* btn);

private:
	gxHWShader* glslBlurShader;
	gePushButton* object3dVisibleToggle;
};

#endif