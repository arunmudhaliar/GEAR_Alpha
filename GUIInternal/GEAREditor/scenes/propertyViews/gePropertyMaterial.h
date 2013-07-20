#ifndef GEPROPERTYMATERIAL_H
#define GEPROPERTYMATERIAL_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "geTextureThumbnailExtended.h"

class gePropertyMaterial : public geTreeNode, public MGUIObserver
{
public:
	geTextBox* m_pText_tileX;
	geTextBox* m_pText_tileY;
	gxMaterial* m_pCurrentMaterialPtr;

	gePropertyMaterial(geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxMaterial* material):
	  geTreeNode(parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 85.0f);

		//geButton* btn=new geButton();
		//btn->create(this, "button1", 40, 10);

		//gePushButton* pbtn = new gePushButton("");
		//pbtn->create(this, "", 15, 10);

		m_pCurrentMaterialPtr=material;
		geTextBox* text_material = new geTextBox("MaterialName");
		text_material->create(this, material->getMaterialName(), 50, 10, 200, 16);

		//geHorizontalSlider* slider = new geHorizontalSlider();
		//slider->create(this, "slider", 50, 35, 70);
		
		char tileX_temp_buffer[10];
		char tileY_temp_buffer[10];

		if(material->getTexture())
		{
			const float* texMat4x4=material->getTexture()->getTextureMatrix()->getMatrix();
			sprintf(tileX_temp_buffer, "%3.2f", texMat4x4[0]);
			sprintf(tileY_temp_buffer, "%3.2f", texMat4x4[5]);
		}
		else
		{
			sprintf(tileX_temp_buffer, "%3.2f", 1.0f);
			sprintf(tileY_temp_buffer, "%3.2f", 1.0f);
		}

		m_pText_tileX = new geTextBox("1.0");
		m_pText_tileX->create(this, tileX_temp_buffer, 100, 40, 50, 16);
		m_pText_tileX->setGUIObserver(this);
		m_pText_tileY = new geTextBox("1.0");
		m_pText_tileY->create(this, tileY_temp_buffer, 100, 60, 50, 16);
		m_pText_tileY->setGUIObserver(this);

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

	virtual void onTextChange(geGUIBase* textbox)
	{
		if(textbox==m_pText_tileX && m_pCurrentMaterialPtr->getTexture())
		{
			float value=geUtil::getFloat(m_pText_tileX->getName());
			const float* texMat4x4=m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->getMatrix();
			m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->setXAxis(vector3f(value, texMat4x4[1], texMat4x4[2]));
		}
		else if(textbox==m_pText_tileY && m_pCurrentMaterialPtr->getTexture())
		{
			float value=geUtil::getFloat(m_pText_tileY->getName());
			const float* texMat4x4=m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->getMatrix();
			m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->setYAxis(vector3f(texMat4x4[4], value, texMat4x4[6]));
		}
	}
};

#endif