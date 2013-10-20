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