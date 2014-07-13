#include "gePropertyMaterial.h"
#include "../../EditorApp.h"

gePropertyMaterial::gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	m_pHorizontalSliderShininess=NULL;
	m_pColorControl=NULL;
	m_pWindowColumn_ShaderProperties = NULL;
	m_pCommonSeperator = NULL;
	m_pBottomSeperator = NULL;

	setSize(m_cSize.x, 115.0f);

	m_pTriInfoPtr=triinfo;
	loadClientViewFromMaterial(m_pTriInfoPtr->getMaterial());

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyMaterial::~gePropertyMaterial()
{
	//destroy submapview nodes
	for(std::vector<stSubMapView*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		stSubMapView* tvnode = *it;
		GE_DELETE(tvnode);
	}
	m_vSubMap.clear();

	destroyShaderPropertiesControls();
}

void gePropertyMaterial::onDragDrop(int x, int y, MDataObject* dropObject)
{
	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetPath();

		if (util::GE_IS_EXTENSION(relativePath, ".mat") || util::GE_IS_EXTENSION(relativePath, ".MAT"))
		{
			unsigned int crc32=AssetImporter::calcCRC32((unsigned char*)relativePath);

			gxMaterial* matchingMaterial=NULL;
			//check if the material name already exists in our list or not
			gxWorld* world=monoWrapper::mono_engine_getWorld(0);
			std::vector<gxMaterial*>* materialList = world->getMaterialList();
			for(std::vector<gxMaterial*>::iterator it = materialList->begin(); it != materialList->end(); ++it)
			{
				gxMaterial* material_in_list = *it;
				if(material_in_list->getAssetFileCRC()==crc32)
				{
					//match found, so assing and delete the new material object
					matchingMaterial=material_in_list;
					m_pTriInfoPtr->setMaterial(matchingMaterial);
					loadClientViewFromMaterial(matchingMaterial);
					return;
				}
			}

			//if material not found in world then load from meta
			char crcFile[1024];
			sprintf(crcFile, "%s/%x", world->getMetaDataFolder(), crc32);
			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				gxMaterial* material = new gxMaterial();
				material->read(file_meta);
				file_meta.CloseFile();

				HWShaderManager* hwShaderManager = engine_getHWShaderManager();
				//load surface shader
				char mainshaderfilename[1024];
				sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", material->getMainshaderName());
				material->setSurfaceShader(hwShaderManager->LoadSurfaceShader(mainshaderfilename));

				//load sub maps
				std::vector<gxSubMap*>* maplist=material->getSubMapList();
				for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it)
				{
					gxSubMap* submap = *it;
					submap->loadTextureFromMeta(*world->getTextureManager(), submap->getTextureCRC());
				}

				m_pTriInfoPtr->setMaterial(material);
				world->getMaterialList()->push_back(m_pTriInfoPtr->getMaterial());
				loadClientViewFromMaterial(material);
			}
		}
	}
}

void gePropertyMaterial::destroySubMapView()
{
	//destroy submapview nodes
	for(std::vector<stSubMapView*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		stSubMapView* submapview = *it;
		//std::vector<geGUIBase*>* childcntrol=getChildControls();
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->m_pText_tileX), m_vControls.end());
		GE_DELETE(submapview->m_pText_tileX);
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->m_pText_tileY), m_vControls.end());
		GE_DELETE(submapview->m_pText_tileY);
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->thumbnail), m_vControls.end());
		GE_DELETE(submapview->thumbnail);
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->m_pMapName), m_vControls.end());
		GE_DELETE(submapview->m_pMapName);
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->m_pTiling), m_vControls.end());
		GE_DELETE(submapview->m_pTiling);
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), submapview->m_pSeperator), m_vControls.end());
		GE_DELETE(submapview->m_pSeperator);

		GE_DELETE(submapview);
	}
	m_vSubMap.clear();
}

void gePropertyMaterial::loadSubMapView()
{
	if(!m_pCurrentMaterialPtr) return;

	//m_vSubMap
	int cntr=0;
	std::vector<stMaterialPass*>* mpasslist=m_pCurrentMaterialPtr->getShaderPassList();
	for(std::vector<stMaterialPass*>::iterator mpass_it = mpasslist->begin(); mpass_it != mpasslist->end(); ++mpass_it)
	{
		stMaterialPass* mpass = *mpass_it;
		for(std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it)
		{
			gxSubMap* map = *it;
			bool bFound=false;
			for(std::vector<stSubMapView*>::iterator sit = m_vSubMap.begin(); sit != m_vSubMap.end(); ++sit)
			{
				stSubMapView* submapview = *sit;
				if(submapview->m_pSubMapPtr==map)
				{
					bFound=true;
					break;
				}
			}

			if(bFound)
				continue;


			stSubMapView* submapview = new stSubMapView(map);

			char tileX_temp_buffer[10];
			char tileY_temp_buffer[10];

			if(map->getTexture())
			{
				const float* texMat4x4=map->getTexture()->getTextureMatrix()->getMatrix();
				sprintf(tileX_temp_buffer, "%3.2f", texMat4x4[0]);
				sprintf(tileY_temp_buffer, "%3.2f", texMat4x4[5]);
			}
			else
			{
				sprintf(tileX_temp_buffer, "%3.2f", 1.0f);
				sprintf(tileY_temp_buffer, "%3.2f", 1.0f);
			}

			submapview->m_pTiling = new geStaticTextBox("");
			submapview->m_pTiling->create(m_pRenderer, this, "Tiling", 20, 80+cntr*80, -5, geGUIManager::g_pFontArial10_80Ptr);
			submapview->m_pTiling->setGUIObserver(this);

			submapview->m_pText_tileX = new geTextBox("1.0");
			submapview->m_pText_tileX->create(m_pRenderer, this, tileX_temp_buffer, 60, 80+cntr*80, 60, 16);
			submapview->m_pText_tileX->setGUIObserver(this);
			submapview->m_pText_tileY = new geTextBox("1.0");
			submapview->m_pText_tileY->create(m_pRenderer, this, tileY_temp_buffer, 130, 80+cntr*80, 60, 16);
			submapview->m_pText_tileY->setGUIObserver(this);

			submapview->m_pMapName = new geStaticTextBox("");
			submapview->m_pMapName->create(m_pRenderer, this, map->getShaderTextureProperty()->name.c_str(), 10, 60+cntr*80, -5, geGUIManager::g_pFontArial10_80Ptr);
			submapview->m_pMapName->setGUIObserver(this);

			submapview->thumbnail = new geTextureThumbnailExtended();
			submapview->thumbnail->create(m_pRenderer, this, map->getTexture(), 260, 60+cntr*80, 70, 70);
			submapview->thumbnail->setUserData(map);

			submapview->m_pSeperator = new geSeperator();
			submapview->m_pSeperator->create(m_pRenderer, this, 10, submapview->thumbnail->getPos().y+submapview->thumbnail->getSize().y+5, submapview->thumbnail->getPos().x+submapview->thumbnail->getSize().x-10);

			m_vSubMap.push_back(submapview);
			cntr++;
		}
	}
		
	if(cntr)
		setSize(m_cSize.x, 145.0f+(cntr-1)*80);

	//range property
	std::vector<stShaderProperty_Range*>* rangeList = m_pCurrentMaterialPtr->getSurfaceShader()->getShaderPropertyList_Range();
	std::vector<stShaderProperty_Color*>* colorList = m_pCurrentMaterialPtr->getSurfaceShader()->getShaderPropertyList_Color();

	if(rangeList->size()>0 || colorList->size()>0)
	{
		//window column
		destroyShaderPropertiesControls();

		m_pWindowColumn_ShaderProperties = new geWindowColumn();
		m_pWindowColumn_ShaderProperties->create(m_pRenderer, this, getSize().y, 300.0f, 10.0f, 0.4f);

		geGUIBase* lastControl = NULL;

		for(std::vector<stShaderProperty_Range*>::iterator it = rangeList->begin(); it != rangeList->end(); ++it)
		{
			stShaderProperty_Range* range = *it;
			geHorizontalSlider* range_slider = new geHorizontalSlider();
			range_slider->create(m_pRenderer, this, "slider", 10, 85, 130);
			range_slider->setRange(range->range_min, range->range_max);
			range_slider->setSliderValueWithInRange(range->range_value);
			//range_slider->setSliderValue(1.0f);
			range_slider->setGUIObserver(this);

			char formatted_buffer[64];
			sprintf(formatted_buffer, "(%3.2f)", range->range_value);

			geStaticTextBox* pLabel = new geStaticTextBox("");
			pLabel->create(m_pRenderer, this, formatted_buffer, 20, 80+cntr*80, -5, geGUIManager::g_pFontArial10_80Ptr);
			pLabel->setGUIObserver(this);
			range_slider->setUserData(pLabel);
			pLabel->setUserData(range);

			lastControl=range_slider;
			sprintf(formatted_buffer, "%s (%3.1f, %3.1f)", range->name.c_str(), range->range_min, range->range_max);
			stWindowColumnRow* row = m_pWindowColumn_ShaderProperties->addRow(formatted_buffer);
			m_pWindowColumn_ShaderProperties->addControl(row, range_slider, range_slider->getSize().y*3);
			m_pWindowColumn_ShaderProperties->addControl(row, pLabel, range_slider->getSize().y*3);
		}

		for(std::vector<stShaderProperty_Color*>::iterator it = colorList->begin(); it != colorList->end(); ++it)
		{
			stShaderProperty_Color* color = *it;
			geColorControl* color_control = new geColorControl();
			color_control->create(m_pRenderer, this, 10, 10);
			color_control->setControlColor(color->color.x, color->color.y, color->color.z, color->color.w);
			color_control->setGUIObserver(this);
			color_control->setUserData(color);

			lastControl=color_control;
			stWindowColumnRow* row = m_pWindowColumn_ShaderProperties->addRow(color->name.c_str());
			m_pWindowColumn_ShaderProperties->addControl(row, color_control, color_control->getSize().y*2);
		}

		m_pBottomSeperator = new geSeperator();
		m_pBottomSeperator->create(m_pRenderer, this, 10, lastControl->getPos().y+lastControl->getSize().y+7, m_pWindowColumn_ShaderProperties->getSize().x-20);

		setSize(m_cSize.x, lastControl->getPos().y+lastControl->getSize().y+15);
	}
}

void gePropertyMaterial::destroyShaderPropertiesControls()
{
	if(m_pWindowColumn_ShaderProperties==NULL) return;

	//destroy the controls in the column
	for(std::vector<stWindowColumnRow*>::iterator it = m_pWindowColumn_ShaderProperties->getRowList()->begin(); it != m_pWindowColumn_ShaderProperties->getRowList()->end(); ++it)
	{
		stWindowColumnRow* row = *it;
		for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
		{
			geGUIBase* control = *ctrl_it;
			m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), control), m_vControls.end());
			GE_DELETE(control);
		}
	}
	//

	m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_pWindowColumn_ShaderProperties), m_vControls.end());
	GE_DELETE(m_pWindowColumn_ShaderProperties);

	m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_pBottomSeperator), m_vControls.end());
	GE_DELETE(m_pBottomSeperator);
}

void gePropertyMaterial::loadClientViewFromMaterial(gxMaterial* material)
{
	destroyShaderPropertiesControls();
	destroySubMapView();

	//destroy the child views
	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		GE_DELETE(tvnode);
	}
	m_vControls.clear();
	//

	m_pCurrentMaterialPtr=material;
	if(m_pCurrentMaterialPtr)
	{
		geTextBox* text_material = new geTextBox("MaterialName");
		text_material->create(m_pRenderer, this, m_pCurrentMaterialPtr->getMaterialName(), 50, 10, 130, 16);
		
		m_pColorControl = new geColorControl();
		m_pColorControl->create(m_pRenderer, this, 10, 10);
		vector4f diffuse=m_pCurrentMaterialPtr->getDiffuseClr();
		m_pColorControl->setControlColor(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_pColorControl->setGUIObserver(this);

		m_pSurfaceShaderToolBarDropMenuBtnPtr=new geToolBarDropMenu(m_pRenderer, "Empty", this);
		m_pSurfaceShaderToolBarDropMenuBtnPtr->setGUIObserver(this);
		m_pSurfaceShaderToolBarDropMenuBtnPtr->setPos(text_material->getPos().x+text_material->getSize().x+15, 10);

		HWShaderManager* hWshaderManager=engine_getHWShaderManager();
		std::vector<gxSurfaceShader*>* surfaceshaderlist=hWshaderManager->getSurfaceShaderList();
		unsigned int base=0x00006000;
		for(std::vector<gxSurfaceShader*>::iterator it = surfaceshaderlist->begin(); it != surfaceshaderlist->end(); ++it, base++)
		{
			gxSurfaceShader* surfaceshader = *it;
			m_pSurfaceShaderToolBarDropMenuBtnPtr->appendMenuItem(surfaceshader->getName(), base);
		}
		m_pSurfaceShaderToolBarDropMenuBtnPtr->setMenuItem(m_pCurrentMaterialPtr->getMainshaderName());

		//shininess
		m_pHorizontalSliderShininess = new geHorizontalSlider();
		m_pHorizontalSliderShininess->create(m_pRenderer, this, "slider", 140, 40, 100);
		m_pHorizontalSliderShininess->setSliderValue(m_pCurrentMaterialPtr->getShininess()/MATERIAL_SHININESS_SCALE_FACTOR);
		m_pHorizontalSliderShininess->setGUIObserver(this);

		m_pCommonSeperator = new geSeperator();
		m_pCommonSeperator->create(m_pRenderer, this, 10, m_pHorizontalSliderShininess->getPos().y+m_pHorizontalSliderShininess->getSize().y+7, m_pHorizontalSliderShininess->getPos().x+m_pHorizontalSliderShininess->getSize().x-10);

		loadSubMapView();
	}
}

void gePropertyMaterial::drawNode()
{
	drawRect(&m_cVBClientArea);

	if(m_pCurrentMaterialPtr)
	{
		//geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		char buffer[64];
		sprintf(buffer, "Shininess (%3.2f/%3.1f)", m_pCurrentMaterialPtr->getShininess(), MATERIAL_SHININESS_SCALE_FACTOR);
		geGUIManager::g_pFontArial10_80Ptr->drawString(buffer, 10, 45, m_cSize.x);

		if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
		{
			if(m_bNodeOpen)
				drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}
	}
	else
		geGUIManager::g_pFontArial10_80Ptr->drawString("Drag-drop a material.", 35, geGUIManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(m_pSprite)
		m_pSprite->draw();
}

void gePropertyMaterial::onColorChange(geGUIBase* colorControl)
{
	if(colorControl==m_pColorControl)
	{
		m_pCurrentMaterialPtr->setDiffuseClr(vector4f(
			m_pColorControl->getControlColor().x,
			m_pColorControl->getControlColor().y,
			m_pColorControl->getControlColor().z,
			m_pColorControl->getControlColor().z
			));
	}


	//iterate through the shader properties control
	if(m_pWindowColumn_ShaderProperties)
	{
		//destroy the controls in the column
		for(std::vector<stWindowColumnRow*>::iterator it = m_pWindowColumn_ShaderProperties->getRowList()->begin(); it != m_pWindowColumn_ShaderProperties->getRowList()->end(); ++it)
		{
			stWindowColumnRow* row = *it;
			for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
			{
				geGUIBase* control = *ctrl_it;
				if(colorControl==control)
				{
					geColorControl* derived_control = (geColorControl*)colorControl;
					stShaderProperty_Color* color_property = (stShaderProperty_Color*)colorControl->getUserData();
					geVector4f clr_value(derived_control->getControlColor());
					color_property->color.set(clr_value.x, clr_value.y, clr_value.z, clr_value.w);

					//pass value to gpu
					color_property->sendToGLSL(true);
					//
					return;
				}
			}
		}
	}
	//
}

void gePropertyMaterial::onCommand(int cmd)
{
	if(cmd>=0x00006000 && cmd<0x00006000+m_pSurfaceShaderToolBarDropMenuBtnPtr->getMenuItemCount())
	{
		if(m_pCurrentMaterialPtr)
		{
			HWShaderManager* hWshaderManager=engine_getHWShaderManager();
			std::vector<gxSurfaceShader*>* surfaceshaderlist=hWshaderManager->getSurfaceShaderList();
			unsigned int base=0x00006000;
			for(std::vector<gxSurfaceShader*>::iterator it = surfaceshaderlist->begin(); it != surfaceshaderlist->end(); ++it, base++)
			{
				gxSurfaceShader* surfaceshader = *it;
				if(base==cmd)
				{
					m_pCurrentMaterialPtr->setMainShaderName(surfaceshader->getName());
					m_pCurrentMaterialPtr->setSurfaceShader(surfaceshader);
					m_pSurfaceShaderToolBarDropMenuBtnPtr->setMenuItem(surfaceshader->getName());
					destroyShaderPropertiesControls();
					destroySubMapView();
					loadSubMapView();
					notifyParent(700);	//invalidate view msg
					break;
				}
			}
		}
	}
}

void gePropertyMaterial::onTextChange(geGUIBase* textbox)
{
}

void gePropertyMaterial::onSliderChange(geGUIBase* slider)
{

	if(m_pHorizontalSliderShininess==slider)
	{
		m_pCurrentMaterialPtr->setShininess(m_pHorizontalSliderShininess->getSliderValue()*MATERIAL_SHININESS_SCALE_FACTOR);
	}

	//iterate through the shader properties control
	if(m_pWindowColumn_ShaderProperties)
	{
		//destroy the controls in the column
		for(std::vector<stWindowColumnRow*>::iterator it = m_pWindowColumn_ShaderProperties->getRowList()->begin(); it != m_pWindowColumn_ShaderProperties->getRowList()->end(); ++it)
		{
			stWindowColumnRow* row = *it;
			for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
			{
				geGUIBase* control = *ctrl_it;
				if(slider==control)
				{
					geHorizontalSlider* derived_control = (geHorizontalSlider*)slider;
					geStaticTextBox* label = (geStaticTextBox*)slider->getUserData();
					stShaderProperty_Range* range = (stShaderProperty_Range*)label->getUserData();
					range->range_value=derived_control->getSliderValueWithInRange();
					
					char formatted_buffer[64];
					sprintf(formatted_buffer, "(%3.2f)", range->range_value);
					label->setName(formatted_buffer);

					//pass value to gpu
					range->sendToGLSL(true);
					//
					return;
				}
			}
		}
	}
	//


}