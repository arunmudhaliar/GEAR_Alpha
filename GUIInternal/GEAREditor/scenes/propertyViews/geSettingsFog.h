#ifndef GESETTINGSFOG_H
#define GESETTINGSFOG_H

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

class geSettingsFog : public geTreeNode, public MGUIObserver
{
public:

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

		geHorizontalSlider* edit_fog_start;
		geHorizontalSlider* edit_fog_end;
		geHorizontalSlider* edit_fog_density;
		geColorControl* color_fog_color;
		geToolBarDropMenu* menu_fog_type;
		geWindowColumn* pWindowColumn;
	};

	geSettingsFog(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, stFog* fog_struct, geFontManager* fontmanager);
	virtual ~geSettingsFog();

	virtual void drawNode();
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onCommand(int cmd);
	virtual void onSliderChange(geGUIBase* slider);

	stFog* m_pFog_struct;	//must not delete this pointer
	stFogView m_cFogSubView;
	gePushButton* m_pPushBtn_Fog;
};

#endif