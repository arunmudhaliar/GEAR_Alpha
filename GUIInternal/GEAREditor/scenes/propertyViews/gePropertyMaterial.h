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
			m_pMapName=NULL;
			m_pTiling=NULL;
			m_pText_tileX=NULL;
			m_pText_tileY=NULL;
			thumbnail=NULL;
			m_pSeperator=NULL;
			m_pSubMapPtr=submap;	//for checking purpose
		}

		geStaticTextBox* m_pMapName;
		geStaticTextBox* m_pTiling;
		geTextBox* m_pText_tileX;
		geTextBox* m_pText_tileY;
		geTextureThumbnailExtended* thumbnail;
		geSeperator* m_pSeperator;
		gxSubMap* m_pSubMapPtr;	//for checking purpose
	};
	std::vector<stSubMapView*> m_vSubMap;

	struct stFogView
	{
		stFogView()
		{
			reset();
		}

		void reset()
		{
			edit_fog_start=NULL;
			edit_fog_end=NULL;
			edit_fog_density=NULL;
			color_fog_color=NULL;
			menu_fog_type=NULL;
			pWindowColumn=NULL;
		}

		geTextBox* edit_fog_start;
		geTextBox* edit_fog_end;
		geTextBox* edit_fog_density;
		geColorControl* color_fog_color;
		geToolBarDropMenu* menu_fog_type;
		geWindowColumn* pWindowColumn;
	};

	gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo);
	virtual ~gePropertyMaterial();

	virtual void drawNode();
	virtual void onTextChange(geGUIBase* textbox);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onCommand(int cmd);
	virtual void onSliderChange(geGUIBase* slider);

	void loadClientViewFromMaterial(gxMaterial* material);
	void destroySubMapView();
	void loadSubMapView(bool& fog);

	stFogView m_cFogSubView;
	geColorControl* m_pColorControl;
	geToolBarDropMenu* m_pSurfaceShaderToolBarDropMenuBtnPtr;
	geHorizontalSlider* m_pHorizontalSliderShininess;
	geSeperator* m_pCommonSeperator;
};

#endif