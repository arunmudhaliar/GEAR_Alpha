#ifndef GEPROPERTYOCTREE_H
#define GEPROPERTYOCTREE_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../../../GEAREngine/src/core/gxCamera.h"
#include "../../gui/geToolBarDropMenu.h"
#include "../../gui/geWindowColumn.h"
#include "../../gui/geButton.h"

class gePropertyOctree : public geTreeNode, public MGUIObserver
{
public:
	geHorizontalSlider* m_pNoOfTransformObjectPerNode;
	geHorizontalSlider* m_pNoOfLevels;
	geButton* m_pButtonApply;
	stWindowColumnRow* m_pRows[3];

	gePropertyOctree(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager);
	virtual ~gePropertyOctree();

	void getOctreeVars(int& nTransformPerNodes, int& nLevels);

	//void populatePropertyOfCamera(object3d* obj);

	virtual void drawNode();

	virtual void onSliderChange(geGUIBase* slider);
//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
//#endif
	virtual void onButtonClicked(geGUIBase* btn);
};

#endif