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

		m_pPushBtn_Object3dVisible = new gePushButton("", fontmanager);
		m_pPushBtn_Object3dVisible->create(renderer, this, "", 15, 10);
		m_pPushBtn_Object3dVisible->setGUIObserver(this);

		//m_pHorizontalSlider_BlurSize = new geHorizontalSlider();
		//m_pHorizontalSlider_BlurSize->create(renderer, this, "slider", 30, 15, 120.0f);
		//m_pHorizontalSlider_BlurSize->setSliderValue(1.0f);
		//m_pHorizontalSlider_BlurSize->setGUIObserver(this);

		//geToolBarDropMenu* pLightTypeToolBarDropMenuBtnPtr=new geToolBarDropMenu(rendererGUI, "LightType", this);
		//pLightTypeToolBarDropMenuBtnPtr->setGUIObserver(this);
		//pLightTypeToolBarDropMenuBtnPtr->setPos(200, 20);


		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
		applyPrimaryColorToVBClientArea();
	}

	virtual ~gePropertyBlurProcessor()
	{
	}

	//virtual void draw();
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
		m_pBlurShaderPtr=blurShader;
		//m_pTextBoxMeshName->setName(obj->getName());

		//m_pPushBtn_Object3dVisible->setCheck(obj->isBaseFlag(object3d::eObject3dBaseFlag_Visible));
	}


	virtual void onSliderChange(geGUIBase* slider)
	{
		//if(m_pHorizontalSlider_BlurSize==slider)
		{

		}
	}

	virtual void onButtonClicked(geGUIBase* btn);

private:
	gxHWShader* m_pBlurShaderPtr;
	//geTextBox* m_pTextBoxMeshName;
	gePushButton* m_pPushBtn_Object3dVisible;
	//geHorizontalSlider* m_pHorizontalSlider_BlurSize;

};

#endif