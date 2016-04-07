#include "gePropertyLight.h"
#include "../../EditorApp.h"

gePropertyLight::gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 130.0f);

	//diffuse
	colorControl = new geColorControl(fontmanager);
	colorControl->create(renderer, this, 10, 10);
	colorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	colorControl->setGUIObserver(this);

	lightTypeToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "LightType", this, fontmanager);
	lightTypeToolBarDropMenuButton->setGUIObserver(this);
	lightTypeToolBarDropMenuButton->setPos(10, 35);

	lightTypeToolBarDropMenuButton->appendMenuItem("Directional Light", 0x00005003);
	lightTypeToolBarDropMenuButton->appendMenuItem("Point Light", 0x00005002);
	//lightTypeToolBarDropMenuButton->appendMenuItem("Spot Light", 0x00005003);

	//ambient
	ambientColorControl = new geColorControl(fontmanager);
	ambientColorControl->create(renderer, this, 100, 10);
	ambientColorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	ambientColorControl->setGUIObserver(this);


	//specular
	specularColorControl = new geColorControl(fontmanager);
	specularColorControl->create(renderer, this, 190, 10);
	specularColorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
	specularColorControl->setGUIObserver(this);


	//attenuations
	constantAttenuationHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	constantAttenuationHorizontalSlider->create(rendererGUI, this, 10, 65, 130);
	constantAttenuationHorizontalSlider->setSliderValue(1.0f);
	constantAttenuationHorizontalSlider->setGUIObserver(this);

	linearAttenuationHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	linearAttenuationHorizontalSlider->create(rendererGUI, this, 10, 85, 130);
	linearAttenuationHorizontalSlider->setSliderValue(1.0f);
	linearAttenuationHorizontalSlider->setGUIObserver(this);

	quadraticAttenuationHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
	quadraticAttenuationHorizontalSlider->create(rendererGUI, this, 10, 105, 130);
	quadraticAttenuationHorizontalSlider->setSliderValue(1.0f);
	quadraticAttenuationHorizontalSlider->setGUIObserver(this);

	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManagerGUI);
	pWindowColumn->create(rendererGUI, this, 40, 300.0f, 10.0f, 0.4f);
	stWindowColumnRow* row = pWindowColumn->addRow("Type");
	pWindowColumn->addControl(row, lightTypeToolBarDropMenuButton, 30.0f);
	row = pWindowColumn->addRow("Constant Attenuation");
	pWindowColumn->addControl(row, constantAttenuationHorizontalSlider, 20.0f);
	row = pWindowColumn->addRow("Linear Attenuation");
	pWindowColumn->addControl(row, linearAttenuationHorizontalSlider, 20.0f);
	row = pWindowColumn->addRow("Quadratic Attenuation");
	pWindowColumn->addControl(row, quadraticAttenuationHorizontalSlider, 20.0f);
	//

	previewLight=NULL;

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyLight::~gePropertyLight()
{

}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
void gePropertyLight::onDragDrop(int x, int y, MDropData* dropObject)
{

}
//#endif

void gePropertyLight::drawNode()
{
	drawRect(&vertexBufferClientArea);

	geFontManager::g_pFontArial10_84Ptr->drawString("Diffuse", colorControl->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Ambient", ambientColorControl->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);
	geFontManager::g_pFontArial10_84Ptr->drawString("Specular", specularColorControl->getPos().x+30, 7+geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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

	//windowColumnControl->draw();

	if(sprite)
		sprite->draw();
}

void gePropertyLight::populatePropertyOfLight(object3d* obj)
{
	previewLight = obj->getMonoScriptInstance<gxLight*>();
	vector4f diff(previewLight->getDiffuseColor());
	vector4f ambient(previewLight->getAmbientColor());
	vector4f specular(previewLight->getSpecularColor());

	colorControl->setControlColor(diff.x, diff.y, diff.z, diff.w);
	ambientColorControl->setControlColor(ambient.x, ambient.y, ambient.z, ambient.w);
	specularColorControl->setControlColor(specular.x, specular.y, specular.z, specular.w);

	constantAttenuationHorizontalSlider->setSliderValue(previewLight->getConstantAttenuation(), false);
	linearAttenuationHorizontalSlider->setSliderValue(previewLight->getLinearAttenuation()*100.0f, false);
	quadraticAttenuationHorizontalSlider->setSliderValue(previewLight->getQuadraticAttenuation()*1000.0f, false);

	if(previewLight->getLightType()==gxLight::LIGHT_DIRECTIONAL)
		lightTypeToolBarDropMenuButton->setMenuItem("Directional Light");
	else if(previewLight->getLightType()==gxLight::LIGHT_POINT)
		lightTypeToolBarDropMenuButton->setMenuItem("Point Light");
}

void gePropertyLight::onTextChange(geGUIBase* textbox)
{
}

void gePropertyLight::onColorChange(geGUIBase* colorControl)
{
	vector4f diff(previewLight->getDiffuseColor());
	vector4f ambient(previewLight->getAmbientColor());
	vector4f specular(previewLight->getSpecularColor());

	if(colorControl==this->colorControl)
	{
		geVector4f newClr(this->colorControl->getControlColor());
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		previewLight->setDiffuseColor(tmp);
	}
	else if(colorControl==ambientColorControl)
	{
		geVector4f newClr(ambientColorControl->getControlColor());
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		previewLight->setAmbientColor(tmp);
	}
	else if(colorControl==specularColorControl)
	{
		geVector4f newClr(specularColorControl->getControlColor());
        vector4f tmp(newClr.x, newClr.y, newClr.z, newClr.w);
		previewLight->setSpecularColor(tmp);
	}
}

void gePropertyLight::onSliderChange(geGUIBase* slider)
{
	if(constantAttenuationHorizontalSlider==slider)
	{
		previewLight->setConstantAttenuation(constantAttenuationHorizontalSlider->getSliderValue());
	}
	else if(linearAttenuationHorizontalSlider==slider)
	{
		previewLight->setLinearAttenuation(linearAttenuationHorizontalSlider->getSliderValue()*0.01f);
	}
	else if(quadraticAttenuationHorizontalSlider==slider)
	{
		previewLight->setQuadraticAttenuation(quadraticAttenuationHorizontalSlider->getSliderValue()*0.001f);
	}
}

void gePropertyLight::onButtonClicked(geGUIBase* btn)
{
}

void gePropertyLight::onCommand(int cmd)
{
	if(cmd==0x00005003)
	{
		previewLight->setLightType(gxLight::LIGHT_DIRECTIONAL);
		lightTypeToolBarDropMenuButton->setMenuItem(cmd);
	}
	else if(cmd==0x00005002)
	{
		previewLight->setLightType(gxLight::LIGHT_POINT);
		lightTypeToolBarDropMenuButton->setMenuItem(cmd);
	}
}