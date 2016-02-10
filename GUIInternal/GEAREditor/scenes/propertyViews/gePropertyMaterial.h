#ifndef GEPROPERTYMATERIAL_H
#define GEPROPERTYMATERIAL_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "geTextureThumbnailExtended.h"
#include "geNullMaterialExtended.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../gui/geStaticTextBox.h"
#include "../../gui/geWindowColumn.h"
#include "../../gui/geSeperator.h"

#define MATERIAL_SHININESS_SCALE_FACTOR	300.0f

class gePropertyMaterial : public geTreeNode, public MGUIObserver
{
public:
	gxMaterial* m_pCurrentMaterialPtr;
	gxTriInfo* m_pTriInfoPtr;

	struct stSubMapView
	{
		stSubMapView(gxSubMap* submap)
		{
			mapNameTextBox=NULL;
			tilingTextBox=NULL;
			textureTileOffsetXTextBox=NULL;
			textureTileOffsetYTextBox=NULL;
			thumbnail=NULL;
			seperatorControl=NULL;
			this->subMap=submap;	//for checking purpose
		}

		geStaticTextBox* mapNameTextBox;
		geStaticTextBox* tilingTextBox;
		geTextBox* textureTileOffsetXTextBox;
		geTextBox* textureTileOffsetYTextBox;
		geTextureThumbnailExtended* thumbnail;
		geSeperator* seperatorControl;
		gxSubMap* subMap;	//for checking purpose
	};
	std::vector<stSubMapView*> subMapList;

	gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo, geFontManager* fontManager);
	virtual ~gePropertyMaterial();

	virtual void drawNode();
	virtual void onTextChange(geGUIBase* textbox);
//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onCommand(int cmd);
	virtual void onSliderChange(geGUIBase* slider);

	void loadClientViewFromMaterial(gxMaterial* material);
	void destroySubMapView();
	void loadSubMapView();

private:
	void destroyShaderPropertiesControls();

	geColorControl* colorControl;
	geToolBarDropMenu* surfaceShaderToolBarDropMenuButton;
	geHorizontalSlider* shininessHorizontalSlider;
	geSeperator* commonSeperatorControl;
	geSeperator* bottomSeperatorControl;	//only be visible if there is any property other than submaps
	geWindowColumn* shaderPropertiesWindowColumnControl;
};

#endif