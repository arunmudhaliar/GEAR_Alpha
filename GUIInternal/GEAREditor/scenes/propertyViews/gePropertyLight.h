#ifndef GEPROPERTYLIGHT_H
#define GEPROPERTYLIGHT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../../../GEAREngine/src/core/gxLight.h"
#include "../../gui/geToolBarDropMenu.h"

class gePropertyLight : public geTreeNode, public MGUIObserver
{
public:
	gxLight* m_pLightPtr;
	geColorControl* m_pColorControl;
	geColorControl* m_pColorControlAmbient;
	geColorControl* m_pColorControlSpecular;
	geToolBarDropMenu* m_pLightTypeToolBarDropMenuBtnPtr;
	geHorizontalSlider* m_pHorizontalSlider_ConstantAttenuation;
	geHorizontalSlider* m_pHorizontalSlider_LinearAttenuation;
	geHorizontalSlider* m_pHorizontalSlider_QuadraticAttenuation;

	gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
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
		m_pLightTypeToolBarDropMenuBtnPtr->appendMenuItem("Spot Light", 0x00005003);

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


		m_pLightPtr=NULL;

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyLight()
	{

	}


	void populatePropertyOfLight(object3d* obj);

	virtual void drawNode();

	virtual void onTextChange(geGUIBase* textbox);
	virtual void onSliderChange(geGUIBase* slider);
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onButtonClicked(geGUIBase* btn);

	virtual void onCommand(int cmd);
};

#endif