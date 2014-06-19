#include "gePropertyMaterial.h"
#include "../../EditorApp.h"

gePropertyMaterial::gePropertyMaterial(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, gxTriInfo* triinfo):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	m_pHorizontalSliderShininess=NULL;
	m_pColorControl=NULL;
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
}

void gePropertyMaterial::onDragDrop(int x, int y, MDataObject* dropObject)
{
	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;
		const char* relativePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

		if (util::GE_IS_EXTENSION(relativePath, ".mat") || util::GE_IS_EXTENSION(relativePath, ".MAT"))
		{
			int crc32=AssetImporter::calcCRC32((unsigned char*)relativePath);

			gxMaterial* matchingMaterial=NULL;
			//check if the material name already exists in our list or not
			gxWorld* world=monoWrapper::mono_engine_getWorld(0);
			std::vector<gxMaterial*>* materialList = world->getMaterialList();
			for(std::vector<gxMaterial*>::iterator it = materialList->begin(); it != materialList->end(); ++it)
			{
				gxMaterial* material_in_list = *it;
				if(material_in_list->getFileCRC()==crc32)
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

	//if fog
	if(m_cFogSubView.edit_fog_start)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.edit_fog_start), m_vControls.end());
	GE_DELETE(m_cFogSubView.edit_fog_start);
	if(m_cFogSubView.edit_fog_end)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.edit_fog_end), m_vControls.end());
	GE_DELETE(m_cFogSubView.edit_fog_end);
	if(m_cFogSubView.edit_fog_density)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.edit_fog_density), m_vControls.end());
	GE_DELETE(m_cFogSubView.edit_fog_density);
	if(m_cFogSubView.color_fog_color)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.color_fog_color), m_vControls.end());
	GE_DELETE(m_cFogSubView.color_fog_color);
	if(m_cFogSubView.menu_fog_type)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.menu_fog_type), m_vControls.end());
	GE_DELETE(m_cFogSubView.menu_fog_type);
	if(m_cFogSubView.pWindowColumn)
		m_vControls.erase(std::remove(m_vControls.begin(), m_vControls.end(), m_cFogSubView.pWindowColumn), m_vControls.end());
	GE_DELETE(m_cFogSubView.pWindowColumn);

	m_cFogSubView.reset();
	//
}

void gePropertyMaterial::loadSubMapView(bool& fog)
{
	if(!m_pCurrentMaterialPtr) return;

	//m_vSubMap
	int cntr=0;
	std::vector<stMaterialPass*>* mpasslist=m_pCurrentMaterialPtr->getShaderPassList();
	for(std::vector<stMaterialPass*>::iterator mpass_it = mpasslist->begin(); mpass_it != mpasslist->end(); ++mpass_it)
	{
		stMaterialPass* mpass = *mpass_it;
		if(!fog)
			fog=mpass->pass->Fog;
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
			submapview->m_pTiling->create(m_pRenderer, this, "Tiling", 20, 70+cntr*80, -5, geGUIManager::g_pFontArial10_80Ptr);
			submapview->m_pTiling->setGUIObserver(this);

			submapview->m_pText_tileX = new geTextBox("1.0");
			submapview->m_pText_tileX->create(m_pRenderer, this, tileX_temp_buffer, 60, 70+cntr*80, 60, 16);
			submapview->m_pText_tileX->setGUIObserver(this);
			submapview->m_pText_tileY = new geTextBox("1.0");
			submapview->m_pText_tileY->create(m_pRenderer, this, tileY_temp_buffer, 130, 70+cntr*80, 60, 16);
			submapview->m_pText_tileY->setGUIObserver(this);

			submapview->m_pMapName = new geStaticTextBox("");
			submapview->m_pMapName->create(m_pRenderer, this, map->getShaderTextureProperty()->name.c_str(), 10, 60+cntr*80, -5, geGUIManager::g_pFontArial10_80Ptr);
			submapview->m_pMapName->setGUIObserver(this);

			submapview->thumbnail = new geTextureThumbnailExtended();
			submapview->thumbnail->create(m_pRenderer, this, map->getTexture(), 260, 60+cntr*80, 70, 70);
			submapview->thumbnail->setUserData(map);

			submapview->m_pSeperator = new geSeperator();
			submapview->m_pSeperator->create(m_pRenderer, this, 10, 135+cntr*80, submapview->thumbnail->getPos().x+submapview->thumbnail->getSize().x-10);

			m_vSubMap.push_back(submapview);
			cntr++;
		}

		if(cntr)
			setSize(m_cSize.x, 145.0f+(cntr-1)*80);
	}

	if(fog)
	{
		geVector2f clientSz(getSize());
		stFog* fog_struct=m_pCurrentMaterialPtr->getFog();

		char buffer_fog_start[32];
		char buffer_fog_end[32];
		char buffer_fog_density[32];
		sprintf(buffer_fog_start, "%4.4f", fog_struct->fog_start);
		sprintf(buffer_fog_end, "%4.4f", fog_struct->fog_end);
		sprintf(buffer_fog_density, "%4.4f", fog_struct->fog_density);

		//fog
		m_cFogSubView.edit_fog_start = new geTextBox(buffer_fog_start);
		m_cFogSubView.edit_fog_start->create(m_pRenderer, this, buffer_fog_start, 0, 0, 80, 16);
		m_cFogSubView.edit_fog_start->setGUIObserver(this);

		m_cFogSubView.edit_fog_end = new geTextBox(buffer_fog_end);
		m_cFogSubView.edit_fog_end->create(m_pRenderer, this, buffer_fog_end, 0, 0, 80, 16);
		m_cFogSubView.edit_fog_end->setGUIObserver(this);

		m_cFogSubView.edit_fog_density = new geTextBox(buffer_fog_density);
		m_cFogSubView.edit_fog_density->create(m_pRenderer, this, buffer_fog_density, 0, 0, 80, 16);
		m_cFogSubView.edit_fog_density->setGUIObserver(this);

		m_cFogSubView.color_fog_color = new geColorControl();
		m_cFogSubView.color_fog_color->create(m_pRenderer, this, 10, 10);
		m_cFogSubView.color_fog_color->setControlColor(1.0, 1.0, 1.0, 1.0);
		m_cFogSubView.color_fog_color->setGUIObserver(this);

		m_cFogSubView.menu_fog_type=new geToolBarDropMenu(m_pRenderer, "FogType", this);
		m_cFogSubView.menu_fog_type->setGUIObserver(this);
		m_cFogSubView.menu_fog_type->setPos(10, 35);
		m_cFogSubView.menu_fog_type->appendMenuItem("Linear", 0x00005100);
		m_cFogSubView.menu_fog_type->appendMenuItem("Exp", 0x00005102);
		m_cFogSubView.menu_fog_type->appendMenuItem("Exp2", 0x00005104);

		//window column
		m_cFogSubView.pWindowColumn = new geWindowColumn();
		m_cFogSubView.pWindowColumn->create(m_pRenderer, this, clientSz.y+5, 300.0f, 10.0f, 0.35f);
		stWindowColumnRow* row = m_cFogSubView.pWindowColumn->addRow("Fog start");
		m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_start, m_cFogSubView.edit_fog_start->getSize().y+2);
		row = m_cFogSubView.pWindowColumn->addRow("Fog end");
		m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_end, m_cFogSubView.edit_fog_end->getSize().y+3);
		row = m_cFogSubView.pWindowColumn->addRow("Fog density");
		m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.edit_fog_density, m_cFogSubView.edit_fog_density->getSize().y+2);
		row = m_cFogSubView.pWindowColumn->addRow("Fog Color");
		m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.color_fog_color, m_cFogSubView.color_fog_color->getSize().y+2);
		row = m_cFogSubView.pWindowColumn->addRow("Fog Type");
		m_cFogSubView.pWindowColumn->addControl(row, m_cFogSubView.menu_fog_type);

		setSize(clientSz.x, row->getYPoistion()+m_cFogSubView.menu_fog_type->getSize().y + 5);
	}
}

void gePropertyMaterial::loadClientViewFromMaterial(gxMaterial* material)
{
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

		bool bFog=false;
		loadSubMapView(bFog);
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
	else if(colorControl==m_cFogSubView.color_fog_color)
	{
		m_pCurrentMaterialPtr->getFog()->setFogColor(vector4f(
			m_cFogSubView.color_fog_color->getControlColor().x,
			m_cFogSubView.color_fog_color->getControlColor().y,
			m_cFogSubView.color_fog_color->getControlColor().z,
			m_cFogSubView.color_fog_color->getControlColor().z
			));
	}
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
					destroySubMapView();
					bool bFog=false;
					loadSubMapView(bFog);
					break;
				}
			}
		}
	}
	else if(m_cFogSubView.menu_fog_type && cmd>=0x00005100 && cmd<0x00005100+m_cFogSubView.menu_fog_type->getMenuItemCount())
	{

	}
}

void gePropertyMaterial::onTextChange(geGUIBase* textbox)
{
	if(m_pCurrentMaterialPtr)
	{
		if(m_cFogSubView.edit_fog_start==textbox)
		{
			float value=geUtil::getFloat(m_cFogSubView.edit_fog_start->getName());
			m_pCurrentMaterialPtr->getFog()->setFogStart(value);
		}
		else if(m_cFogSubView.edit_fog_end==textbox)
		{
			float value=geUtil::getFloat(m_cFogSubView.edit_fog_end->getName());
			m_pCurrentMaterialPtr->getFog()->setFogEnd(value);
		}
		else if(m_cFogSubView.edit_fog_density==textbox)
		{
			float value=geUtil::getFloat(m_cFogSubView.edit_fog_density->getName());
			m_pCurrentMaterialPtr->getFog()->setFogDensity(value);
		}

		if(abs(m_pCurrentMaterialPtr->getFog()->fog_end)>0.00001f || abs(m_pCurrentMaterialPtr->getFog()->fog_start)>0.00001f)
			m_pCurrentMaterialPtr->getFog()->calculateScale();
	}

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

void gePropertyMaterial::onSliderChange(geGUIBase* slider)
{

	if(m_pHorizontalSliderShininess==slider)
	{
		m_pCurrentMaterialPtr->setShininess(m_pHorizontalSliderShininess->getSliderValue()*MATERIAL_SHININESS_SCALE_FACTOR);
	}
}