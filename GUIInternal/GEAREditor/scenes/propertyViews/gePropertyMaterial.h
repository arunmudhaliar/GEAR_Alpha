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

class gePropertyMaterial : public geTreeNode, public MGUIObserver
{
public:
	gxMaterial* m_pCurrentMaterialPtr;
	gxTriInfo* m_pTriInfoPtr;

	struct stSubMapView
	{
		geStaticTextBox* m_pMapName;
		geStaticTextBox* m_pTiling;
		geTextBox* m_pText_tileX;
		geTextBox* m_pText_tileY;
		geTextureThumbnailExtended* thumbnail;
	};
	std::vector<stSubMapView*> m_vSubMap;

	gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo):
	  geTreeNode(renderer, parent, name, sprite, 10)
	{
		m_pColorControl=NULL;
		setSize(m_cSize.x, 115.0f);

		m_pTriInfoPtr=triinfo;
		loadClientViewFromMaterial(m_pTriInfoPtr->getMaterial());

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	void loadClientViewFromMaterial(gxMaterial* material);

	virtual ~gePropertyMaterial()
	{
		//destroy submapview nodes
		for(std::vector<stSubMapView*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
		{
			stSubMapView* tvnode = *it;
			GE_DELETE(tvnode);
		}
		m_vSubMap.clear();
	}

	virtual void drawNode();

	virtual void onTextChange(geGUIBase* textbox)
	{
		//if(textbox==m_pText_tileX && m_pCurrentMaterialPtr->getTexture())
		//{
		//	float value=geUtil::getFloat(m_pText_tileX->getName());
		//	const float* texMat4x4=m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->getMatrix();
		//	m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->setXAxis(vector3f(value, texMat4x4[1], texMat4x4[2]));
		//}
		//else if(textbox==m_pText_tileY && m_pCurrentMaterialPtr->getTexture())
		//{
		//	float value=geUtil::getFloat(m_pText_tileY->getName());
		//	const float* texMat4x4=m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->getMatrix();
		//	m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->setYAxis(vector3f(texMat4x4[4], value, texMat4x4[6]));
		//}
	}

	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onCommand(int cmd);

	void destroySubMapView();
	void loadSubMapView();

	geColorControl* m_pColorControl;
	geToolBarDropMenu* m_pSurfaceShaderToolBarDropMenuBtnPtr;
};

#endif