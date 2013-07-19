#ifndef GEPROPERTYMATERIAL_H
#define GEPROPERTYMATERIAL_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "geTextureThumbnailExtended.h"

class gePropertyMaterial : public geTreeNode
{
public:
	gePropertyMaterial(geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxMaterial* material):
	  geTreeNode(parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 100.0f);

		//geButton* btn=new geButton();
		//btn->create(this, "button1", 40, 10);

		//gePushButton* pbtn = new gePushButton("");
		//pbtn->create(this, "", 15, 10);

		geTextBox* txt = new geTextBox("MaterialName");
		txt->create(this, material->getMaterialName(), 50, 10, 200, 16);

		geHorizontalSlider* slider = new geHorizontalSlider();
		slider->create(this, "slider", 50, 35, 70);

		//geHorizontalSlider* slider2 = new geHorizontalSlider();
		//slider2->create(this, "slider", 50, 55, 70);

		geColorControl* colorControl = new geColorControl();
		colorControl->create(this, 10, 10);
		vector4f diffuse=material->getDiffuseClr();
		colorControl->setControlColor(diffuse.x, diffuse.y, diffuse.z, diffuse.w);

		geTextureThumbnailExtended* thumbnail = new geTextureThumbnailExtended();
		thumbnail->create(this, material->getTexture(), 260, 10, 70, 70);
		thumbnail->setUserData(material);

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyMaterial()
	{

	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial12Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

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

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}
};

#endif