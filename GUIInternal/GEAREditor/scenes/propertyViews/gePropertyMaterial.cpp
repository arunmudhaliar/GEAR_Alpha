#include "gePropertyMaterial.h"
#include "../../EditorApp.h"

gePropertyMaterial::gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	shininessHorizontalSlider=NULL;
	colorControl=NULL;
	shaderPropertiesWindowColumnControl = NULL;
	commonSeperatorControl = NULL;
	bottomSeperatorControl = NULL;

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
	for(std::vector<stSubMapView*>::iterator it = subMapList.begin(); it != subMapList.end(); ++it)
	{
		stSubMapView* tvnode = *it;
		GE_DELETE(tvnode);
	}
	subMapList.clear();

	destroyShaderPropertiesControls();
}

//#if !defined(GEAR_APPLE) //disable Drag-Drop
void gePropertyMaterial::onDragDrop(int x, int y, MDropData* dropObject)
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
			char crcFile[FILENAME_MAX];
			sprintf(crcFile, "%s/%x", world->getMetaDataFolder(), crc32);
			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
                //read the meta header
				stMetaHeader metaHeader;
                metaHeader.readMetaHeader(file_meta);

                //read the material
				gxMaterial* material = new gxMaterial();
				material->read(file_meta);
				file_meta.CloseFile();

				HWShaderManager* hwShaderManager = engine_getHWShaderManager();
				//load surface shader
				char mainshaderfilename[FILENAME_MAX];
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
//#endif

void gePropertyMaterial::destroySubMapView()
{
	//destroy submapview nodes
	for(std::vector<stSubMapView*>::iterator it = subMapList.begin(); it != subMapList.end(); ++it)
	{
		stSubMapView* submapview = *it;
		//std::vector<geGUIBase*>* childcntrol=getChildControls();
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->textureTileOffsetXTextBox), childControlList.end());
		GE_DELETE(submapview->textureTileOffsetXTextBox);
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->textureTileOffsetYTextBox), childControlList.end());
		GE_DELETE(submapview->textureTileOffsetYTextBox);
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->thumbnail), childControlList.end());
		GE_DELETE(submapview->thumbnail);
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->mapNameTextBox), childControlList.end());
		GE_DELETE(submapview->mapNameTextBox);
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->tilingTextBox), childControlList.end());
		GE_DELETE(submapview->tilingTextBox);
		childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), submapview->seperatorControl), childControlList.end());
		GE_DELETE(submapview->seperatorControl);

		GE_DELETE(submapview);
	}
	subMapList.clear();
}

void gePropertyMaterial::loadSubMapView()
{
	if(!m_pCurrentMaterialPtr) return;

	//subMapList
	int cntr=0;
	std::vector<stMaterialPass*>* mpasslist=m_pCurrentMaterialPtr->getShaderPassList();
	for(std::vector<stMaterialPass*>::iterator mpass_it = mpasslist->begin(); mpass_it != mpasslist->end(); ++mpass_it)
	{
		stMaterialPass* mpass = *mpass_it;
		for(std::vector<gxSubMap*>::iterator it = mpass->vUsedSubMap.begin(); it != mpass->vUsedSubMap.end(); ++it)
		{
			gxSubMap* map = *it;
			bool bFound=false;
			for(std::vector<stSubMapView*>::iterator sit = subMapList.begin(); sit != subMapList.end(); ++sit)
			{
				stSubMapView* submapview = *sit;
				if(submapview->subMap==map)
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

			submapview->tilingTextBox = new geStaticTextBox("", fontManagerGUI);
			submapview->tilingTextBox->create(rendererGUI, this, "Tiling", 20, 80+cntr*80, -5, geFontManager::g_pFontArial10_80Ptr);
			submapview->tilingTextBox->setGUIObserver(this);

			submapview->textureTileOffsetXTextBox = new geTextBox("1.0", fontManagerGUI);
			submapview->textureTileOffsetXTextBox->create(rendererGUI, this, tileX_temp_buffer, 60, 80+cntr*80, 60, 16);
			submapview->textureTileOffsetXTextBox->setGUIObserver(this);
			submapview->textureTileOffsetYTextBox = new geTextBox("1.0", fontManagerGUI);
			submapview->textureTileOffsetYTextBox->create(rendererGUI, this, tileY_temp_buffer, 130, 80+cntr*80, 60, 16);
			submapview->textureTileOffsetYTextBox->setGUIObserver(this);

			submapview->mapNameTextBox = new geStaticTextBox("", fontManagerGUI);
			submapview->mapNameTextBox->create(rendererGUI, this, map->getShaderTextureProperty()->name.c_str(), 10, 60+cntr*80, -5, geFontManager::g_pFontArial10_80Ptr);
			submapview->mapNameTextBox->setGUIObserver(this);

			submapview->thumbnail = new geTextureThumbnailExtended(fontManagerGUI);
			submapview->thumbnail->create(rendererGUI, this, map->getTexture(), 260, 60+cntr*80, 70, 70);
			submapview->thumbnail->setUserData(map);

			submapview->seperatorControl = new geSeperator(fontManagerGUI);
			submapview->seperatorControl->create(rendererGUI, this, 10, submapview->thumbnail->getPos().y+submapview->thumbnail->getSize().y+5, submapview->thumbnail->getPos().x+submapview->thumbnail->getSize().x-10);

			subMapList.push_back(submapview);
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

		shaderPropertiesWindowColumnControl = new geWindowColumn(fontManagerGUI);
		shaderPropertiesWindowColumnControl->create(rendererGUI, this, getSize().y, 300.0f, 10.0f, 0.4f);

		geGUIBase* lastControl = NULL;

		for(std::vector<stShaderProperty_Range*>::iterator it = rangeList->begin(); it != rangeList->end(); ++it)
		{
			stShaderProperty_Range* range = *it;
			geHorizontalSlider* range_slider = new geHorizontalSlider(fontManagerGUI);
			range_slider->create(rendererGUI, this, 10, 85, 130);
			range_slider->setRange(range->range_min, range->range_max);
			range_slider->setSliderValueWithInRange(range->range_value);
			//range_slider->setSliderValue(1.0f);
			range_slider->setGUIObserver(this);

			char formatted_buffer[64];
			sprintf(formatted_buffer, "(%3.2f)", range->range_value);

			geStaticTextBox* pLabel = new geStaticTextBox("", fontManagerGUI);
			pLabel->create(rendererGUI, this, formatted_buffer, 20, 80+cntr*80, -5, geFontManager::g_pFontArial10_80Ptr);
			pLabel->setGUIObserver(this);
			range_slider->setUserData(pLabel);
			pLabel->setUserData(range);

			lastControl=range_slider;
			sprintf(formatted_buffer, "%s (%3.1f, %3.1f)", range->name.c_str(), range->range_min, range->range_max);
			stWindowColumnRow* row = shaderPropertiesWindowColumnControl->addRow(formatted_buffer);
			shaderPropertiesWindowColumnControl->addControl(row, range_slider, range_slider->getSize().y*3);
			shaderPropertiesWindowColumnControl->addControl(row, pLabel, range_slider->getSize().y*3);
		}

		for(std::vector<stShaderProperty_Color*>::iterator it = colorList->begin(); it != colorList->end(); ++it)
		{
			stShaderProperty_Color* color = *it;
			geColorControl* color_control = new geColorControl(fontManagerGUI);
			color_control->create(rendererGUI, this, 10, 10);
			color_control->setControlColor(color->color.x, color->color.y, color->color.z, color->color.w);
			color_control->setGUIObserver(this);
			color_control->setUserData(color);

			lastControl=color_control;
			stWindowColumnRow* row = shaderPropertiesWindowColumnControl->addRow(color->name.c_str());
			shaderPropertiesWindowColumnControl->addControl(row, color_control, color_control->getSize().y*2);
		}

		bottomSeperatorControl = new geSeperator(fontManagerGUI);
		bottomSeperatorControl->create(rendererGUI, this, 10, lastControl->getPos().y+lastControl->getSize().y+7, shaderPropertiesWindowColumnControl->getSize().x-20);

		setSize(m_cSize.x, lastControl->getPos().y+lastControl->getSize().y+15);
	}
}

void gePropertyMaterial::destroyShaderPropertiesControls()
{
	if(shaderPropertiesWindowColumnControl==NULL) return;

	//destroy the controls in the column
	for(std::vector<stWindowColumnRow*>::iterator it = shaderPropertiesWindowColumnControl->getRowList()->begin(); it != shaderPropertiesWindowColumnControl->getRowList()->end(); ++it)
	{
		stWindowColumnRow* row = *it;
		for(std::vector<geGUIBase*>::iterator ctrl_it = row->getControlList()->begin(); ctrl_it != row->getControlList()->end(); ++ctrl_it)
		{
			geGUIBase* control = *ctrl_it;
			childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), control), childControlList.end());
			GE_DELETE(control);
		}
	}
	//

	childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), shaderPropertiesWindowColumnControl), childControlList.end());
	GE_DELETE(shaderPropertiesWindowColumnControl);

	childControlList.erase(std::remove(childControlList.begin(), childControlList.end(), bottomSeperatorControl), childControlList.end());
	GE_DELETE(bottomSeperatorControl);
}

void gePropertyMaterial::loadClientViewFromMaterial(gxMaterial* material)
{
	destroyShaderPropertiesControls();
	destroySubMapView();

	//destroy the child views
	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		GE_DELETE(tvnode);
	}
	childControlList.clear();
	//

	m_pCurrentMaterialPtr=material;
	if(m_pCurrentMaterialPtr)
	{
		geTextBox* text_material = new geTextBox("MaterialName", fontManagerGUI);
		text_material->create(rendererGUI, this, m_pCurrentMaterialPtr->getMaterialName().c_str(), 50, 10, 130, 16);
		
		colorControl = new geColorControl(fontManagerGUI);
		colorControl->create(rendererGUI, this, 10, 10);
		vector4f diffuse=m_pCurrentMaterialPtr->getDiffuseClr();
		colorControl->setControlColor(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		colorControl->setGUIObserver(this);

		surfaceShaderToolBarDropMenuButton=new geToolBarDropMenu(rendererGUI, "Empty", this, fontManagerGUI);
		surfaceShaderToolBarDropMenuButton->setGUIObserver(this);
		surfaceShaderToolBarDropMenuButton->setPos(text_material->getPos().x+text_material->getSize().x+15, 10);

		HWShaderManager* hWshaderManager=engine_getHWShaderManager();
		std::vector<gxSurfaceShader*>* surfaceshaderlist=hWshaderManager->getSurfaceShaderList();
		unsigned int base=0x00006000;
		for(std::vector<gxSurfaceShader*>::iterator it = surfaceshaderlist->begin(); it != surfaceshaderlist->end(); ++it, base++)
		{
			gxSurfaceShader* surfaceshader = *it;
			surfaceShaderToolBarDropMenuButton->appendMenuItem(surfaceshader->getName(), base);
		}
		surfaceShaderToolBarDropMenuButton->setMenuItem(m_pCurrentMaterialPtr->getMainshaderName());

		//shininess
		shininessHorizontalSlider = new geHorizontalSlider(fontManagerGUI);
		shininessHorizontalSlider->create(rendererGUI, this, 140, 40, 100);
		shininessHorizontalSlider->setSliderValue(m_pCurrentMaterialPtr->getShininess()/MATERIAL_SHININESS_SCALE_FACTOR);
		shininessHorizontalSlider->setGUIObserver(this);

		commonSeperatorControl = new geSeperator(fontManagerGUI);
		commonSeperatorControl->create(rendererGUI, this, 10, shininessHorizontalSlider->getPos().y+shininessHorizontalSlider->getSize().y+7, shininessHorizontalSlider->getPos().x+shininessHorizontalSlider->getSize().x-10);

		loadSubMapView();
	}
}

void gePropertyMaterial::drawNode()
{
	drawRect(&vertexBufferClientArea);

	if(m_pCurrentMaterialPtr)
	{
		//geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

		char buffer[64];
		sprintf(buffer, "Shininess (%3.2f/%3.1f)", m_pCurrentMaterialPtr->getShininess(), MATERIAL_SHININESS_SCALE_FACTOR);
		geFontManager::g_pFontArial10_80Ptr->drawString(buffer, 10, 45, m_cSize.x);

		if(childControlList.size() && hasAtleastOneTreeNodeChild)
		{
			if(is_OpenNode)
				drawTriangle(&vertexBufferToggleButtonArray[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&vertexBufferToggleButtonArray[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}
	}
	else
		geFontManager::g_pFontArial10_80Ptr->drawString("Drag-drop a material.", 35, geFontManager::g_pFontArial10_80Ptr->getLineHeight()+10, m_cSize.x);

	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(sprite)
		sprite->draw();
}

void gePropertyMaterial::onColorChange(geGUIBase* colorControl)
{
	if(colorControl==this->colorControl)
	{
		m_pCurrentMaterialPtr->setDiffuseClr(vector4f(
			this->colorControl->getControlColor().x,
			this->colorControl->getControlColor().y,
			this->colorControl->getControlColor().z,
			this->colorControl->getControlColor().z
			));
	}


	//iterate through the shader properties control
	if(shaderPropertiesWindowColumnControl)
	{
		//destroy the controls in the column
		for(std::vector<stWindowColumnRow*>::iterator it = shaderPropertiesWindowColumnControl->getRowList()->begin(); it != shaderPropertiesWindowColumnControl->getRowList()->end(); ++it)
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
	if(cmd>=0x00006000 && cmd<0x00006000+surfaceShaderToolBarDropMenuButton->getMenuItemCount())
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
					surfaceShaderToolBarDropMenuButton->setMenuItem(surfaceshader->getName());
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

	if(shininessHorizontalSlider==slider)
	{
		m_pCurrentMaterialPtr->setShininess(shininessHorizontalSlider->getSliderValue()*MATERIAL_SHININESS_SCALE_FACTOR);
	}

	//iterate through the shader properties control
	if(shaderPropertiesWindowColumnControl)
	{
		//destroy the controls in the column
		for(std::vector<stWindowColumnRow*>::iterator it = shaderPropertiesWindowColumnControl->getRowList()->begin(); it != shaderPropertiesWindowColumnControl->getRowList()->end(); ++it)
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