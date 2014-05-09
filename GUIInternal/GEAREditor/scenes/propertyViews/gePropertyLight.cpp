#include "gePropertyLight.h"
#include "../../EditorApp.h"

gePropertyLight::gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	setSize(m_cSize.x, 130.0f);

	//diffuse
	m_pColorControl = new geColorControl();
	m_pColorControl->create(renderer, this, 10, 10);
	m_pColorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControl->setGUIObserver(this);

	m_pLightTypeToolBarDropMenuBtnPtr=new geToolBarDropMenu(m_pRenderer, "LightType", this);
	m_pLightTypeToolBarDropMenuBtnPtr->setGUIObserver(this);
	m_pLightTypeToolBarDropMenuBtnPtr->setPos(10, 35);

	m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Directional Light", 0x00005003);
	m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Point Light", 0x00005002);
	//m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Spot Light", 0x00005003);

	//ambient
	m_pColorControlAmbient = new geColorControl();
	m_pColorControlAmbient->create(renderer, this, 100, 10);
	m_pColorControlAmbient->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControlAmbient->setGUIObserver(this);


	//specular
	m_pColorControlSpecular = new geColorControl();
	m_pColorControlSpecular->create(renderer, this, 190, 10);
	m_pColorControlSpecular->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControlSpecular->setGUIObserver(this);


	//attenuations
	m_pHorizontalSlider_ConstantAttenuation = new geHorizontalSlider();
	m_pHorizontalSlider_ConstantAttenuation->create(m_pRenderer, this, "slider", 10, 65, 130);
	m_pHorizontalSlider_ConstantAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_ConstantAttenuation->setGUIObserver(this);

	m_pHorizontalSlider_LinearAttenuation = new geHorizontalSlider();
	m_pHorizontalSlider_LinearAttenuation->create(m_pRenderer, this, "slider", 10, 85, 130);
	m_pHorizontalSlider_LinearAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_LinearAttenuation->setGUIObserver(this);

	m_pHorizontalSlider_QuadraticAttenuation = new geHorizontalSlider();
	m_pHorizontalSlider_QuadraticAttenuation->create(m_pRenderer, this, "slider", 10, 105, 130);
	m_pHorizontalSlider_QuadraticAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_QuadraticAttenuation->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn();
	pWindowColumn->create(m_pRenderer, this, 40, 300.0f, 10.0f, 0.4f);
	stWindowColumnRow* row = pWindowColumn->addRow("Type");
	pWindowColumn->addControl(row, m_pLightTypeToolBarDropMenuBtnPtr, 30.0f);
	row = pWindowColumn->addRow("Constant Attenuation");
	pWindowColumn->addControl(row, m_pHorizontalSlider_ConstantAttenuation, 20.0f);
	row = pWindowColumn->addRow("Linear Attenuation");
	pWindowColumn->addControl(row, m_pHorizontalSlider_LinearAttenuation, 20.0f);
	row = pWindowColumn->addRow("Quadratic Attenuation");
	pWindowColumn->addControl(row, m_pHorizontalSlider_QuadraticAttenuation, 20.0f);
	//

	m_pLightPtr=NULL;

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyLight::~gePropertyLight()
{

}

void gePropertyLight::onDragDrop(int x, int y, MDataObject* dropObject)
{

}

void gePropertyLight::drawNode()
{
	drawRect(&m_cVBClientArea);

	geGUIManager::g_pFontArial10_84Ptr->drawString("Diffuse", m_pColorControl->getPos().x+30, 7+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Ambient", m_pColorControlAmbient->getPos().x+30, 7+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geGUIManager::g_pFontArial10_84Ptr->drawString("Specular", m_pColorControlSpecular->getPos().x+30, 7+geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

	//m_pWindowColumn->draw();

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

	m_pHorizontalSlider_ConstantAttenuation->setSliderValue(m_pLightPtr->getConstantAttenuation(), false);
	m_pHorizontalSlider_LinearAttenuation->setSliderValue(m_pLightPtr->getLinearAttenuation()*100.0f, false);
	m_pHorizontalSlider_QuadraticAttenuation->setSliderValue(m_pLightPtr->getQuadraticAttenuation()*1000.0f, false);

	if(m_pLightPtr->getLightType()==gxLight::LIGHT_DIRECTIONAL)
		m_pLightTypeToolBarDropMenuBtnPtr->setMenuItem("Directional Light");
	else if(m_pLightPtr->getLightType()==gxLight::LIGHT_POINT)
		m_pLightTypeToolBarDropMenuBtnPtr->setMenuItem("Point Light");
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
		geVector4f newClr(m_pColorControlSpecular->getControlColor());
		m_pLightPtr->setSpecularColor(vector4f(newClr.x, newClr.y, newClr.z, newClr.w));
	}
}

void gePropertyLight::onSliderChange(geGUIBase* slider)
{
	if(m_pHorizontalSlider_ConstantAttenuation==slider)
	{
		m_pLightPtr->setConstantAttenuation(m_pHorizontalSlider_ConstantAttenuation->getSliderValue());
	}
	else if(m_pHorizontalSlider_LinearAttenuation==slider)
	{
		m_pLightPtr->setLinearAttenuation(m_pHorizontalSlider_LinearAttenuation->getSliderValue()*0.01f);
	}
	else if(m_pHorizontalSlider_QuadraticAttenuation==slider)
	{
		m_pLightPtr->setQuadraticAttenuation(m_pHorizontalSlider_QuadraticAttenuation->getSliderValue()*0.001f);
	}
}

void gePropertyLight::onButtonClicked(geGUIBase* btn)
{
}

void gePropertyLight::onCommand(int cmd)
{
	if(cmd==0x00005003)
	{
		m_pLightPtr->setLightType(gxLight::LIGHT_DIRECTIONAL);
		m_pLightTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
	else if(cmd==0x00005002)
	{
		m_pLightPtr->setLightType(gxLight::LIGHT_POINT);
		m_pLightTypeToolBarDropMenuBtnPtr->setMenuItem(cmd);
	}
}