#include "gePropertyLight.h"
#include "../../EditorApp.h"

gePropertyLight::gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 130.0f);

	//diffuse
	m_pColorControl = new geColorControl(fontmanager);
	m_pColorControl->create(renderer, this, 10, 10);
	m_pColorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControl->setGUIObserver(this);

	m_pLightTypeToolBarDropMenuBtnPtr=new geToolBarDropMenu(rendererGUI, "LightType", this, fontmanager);
	m_pLightTypeToolBarDropMenuBtnPtr->setGUIObserver(this);
	m_pLightTypeToolBarDropMenuBtnPtr->setPos(10, 35);

	m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Directional Light", 0x00005003);
	m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Point Light", 0x00005002);
	//m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Spot Light", 0x00005003);

	//ambient
	m_pColorControlAmbient = new geColorControl(fontmanager);
	m_pColorControlAmbient->create(renderer, this, 100, 10);
	m_pColorControlAmbient->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControlAmbient->setGUIObserver(this);


	//specular
	m_pColorControlSpecular = new geColorControl(fontmanager);
	m_pColorControlSpecular->create(renderer, this, 190, 10);
	m_pColorControlSpecular->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_pColorControlSpecular->setGUIObserver(this);


	//attenuations
	m_pHorizontalSlider_ConstantAttenuation = new geHorizontalSlider(fontManagerGUI);
	m_pHorizontalSlider_ConstantAttenuation->create(rendererGUI, this, "slider", 10, 65, 130);
	m_pHorizontalSlider_ConstantAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_ConstantAttenuation->setGUIObserver(this);

	m_pHorizontalSlider_LinearAttenuation = new geHorizontalSlider(fontManagerGUI);
	m_pHorizontalSlider_LinearAttenuation->create(rendererGUI, this, "slider", 10, 85, 130);
	m_pHorizontalSlider_LinearAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_LinearAttenuation->setGUIObserver(this);

	m_pHorizontalSlider_QuadraticAttenuation = new geHorizontalSlider(fontManagerGUI);
	m_pHorizontalSlider_QuadraticAttenuation->create(rendererGUI, this, "slider", 10, 105, 130);
	m_pHorizontalSlider_QuadraticAttenuation->setSliderValue(1.0f);
	m_pHorizontalSlider_QuadraticAttenuation->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManagerGUI);
	pWindowColumn->create(rendererGUI, this, 40, 300.0f, 10.0f, 0.4f);
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

//#if !defined(__APPLE__) //disable Drag-Drop
void gePropertyLight::onDragDrop(int x, int y, MDropData* dropObject)
{

}
//#endif

void gePropertyLight::drawNode()
{
	drawRect(&vertexBufferClientArea);

	geFontManager::g_pFontArial10_84Ptr->drawString("Diffuse", m_pColorControl->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Ambient", m_pColorControlAmbient->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Specular", m_pColorControlSpecular->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

	//m_pWindowColumn->draw();

	if(sprite)
		sprite->draw();
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
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		m_pLightPtr->setDiffuseColor(tmp);
	}
	else if(colorControl==m_pColorControlAmbient)
	{
		geVector4f newClr(m_pColorControlAmbient->getControlColor());
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		m_pLightPtr->setAmbientColor(tmp);
	}
	else if(colorControl==m_pColorControlSpecular)
	{
		geVector4f newClr(m_pColorControlSpecular->getControlColor());
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		m_pLightPtr->setSpecularColor(tmp);
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