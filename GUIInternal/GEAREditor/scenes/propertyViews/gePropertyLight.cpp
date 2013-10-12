#include "gePropertyLight.h"
#include "../../EditorApp.h"

void gePropertyLight::onDragDrop(int x, int y, MDataObject* dropObject)
{

}

void gePropertyLight::drawNode()
{
	drawRect(&m_cVBClientArea);

	geGUIManager::g_pFontArial12Ptr->drawString("Diffuse", 10, 50+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial12Ptr->drawString("Ambient", 120, 50+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial12Ptr->drawString("Specular", 230, 50+geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

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
	m_pHorizontalSlider_LightColor[0]->setSliderValue(diff.x);
	m_pHorizontalSlider_LightColor[1]->setSliderValue(diff.y);
	m_pHorizontalSlider_LightColor[2]->setSliderValue(diff.z);
}

void gePropertyLight::onTextChange(geGUIBase* textbox)
{
}

void gePropertyLight::onSliderChange(geGUIBase* slider)
{
	vector4f diff(m_pLightPtr->getDiffuseColor());
	vector4f ambient(m_pLightPtr->getAmbientColor());
	vector4f specular(m_pLightPtr->getSpecularColor());

	if(slider==m_pHorizontalSlider_LightColor[0])
	{
		m_pLightPtr->setDiffuseColor(vector4f(m_pHorizontalSlider_LightColor[0]->getSliderValue(), diff.y, diff.z, diff.w));
	}
	else if(slider==m_pHorizontalSlider_LightColor[1])
	{
		m_pLightPtr->setDiffuseColor(vector4f(diff.x, m_pHorizontalSlider_LightColor[1]->getSliderValue(), diff.z, diff.w));
	}
	else if(slider==m_pHorizontalSlider_LightColor[2])
	{
		m_pLightPtr->setDiffuseColor(vector4f(diff.x, diff.y, m_pHorizontalSlider_LightColor[2]->getSliderValue(), diff.w));
	}

	if(slider==m_pHorizontalSlider_LightAmbientColor[0])
	{
		m_pLightPtr->setAmbientColor(vector4f(m_pHorizontalSlider_LightAmbientColor[0]->getSliderValue(), ambient.y, ambient.z, ambient.w));
	}
	else if(slider==m_pHorizontalSlider_LightAmbientColor[1])
	{
		m_pLightPtr->setAmbientColor(vector4f(ambient.x, m_pHorizontalSlider_LightAmbientColor[1]->getSliderValue(), ambient.z, ambient.w));
	}
	else if(slider==m_pHorizontalSlider_LightAmbientColor[2])
	{
		m_pLightPtr->setAmbientColor(vector4f(ambient.x, ambient.y, m_pHorizontalSlider_LightAmbientColor[2]->getSliderValue(), ambient.w));
	}

	if(slider==m_pHorizontalSlider_LightSpecularColor[0])
	{
		m_pLightPtr->setSpecularColor(vector4f(m_pHorizontalSlider_LightSpecularColor[0]->getSliderValue(), specular.y, specular.z, specular.w));
	}
	else if(slider==m_pHorizontalSlider_LightSpecularColor[1])
	{
		m_pLightPtr->setSpecularColor(vector4f(specular.x, m_pHorizontalSlider_LightSpecularColor[1]->getSliderValue(), specular.z, specular.w));
	}
	else if(slider==m_pHorizontalSlider_LightSpecularColor[2])
	{
		m_pLightPtr->setSpecularColor(vector4f(specular.x, specular.y, m_pHorizontalSlider_LightSpecularColor[2]->getSliderValue(), specular.w));
	}

	diff = m_pLightPtr->getDiffuseColor();
	ambient=m_pLightPtr->getAmbientColor();
	specular=m_pLightPtr->getSpecularColor();

	m_pColorControl->setControlColor(diff.x, diff.y, diff.z, diff.w);
	m_pColorControlAmbient->setControlColor(ambient.x, ambient.y, ambient.z, ambient.w);
	m_pColorControlSpecular->setControlColor(specular.x, specular.y, specular.z, specular.w);
}