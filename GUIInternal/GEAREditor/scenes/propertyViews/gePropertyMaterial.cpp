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
			submapview->m_pSeperator->create(m_pRenderer, this, 10, 135+cntr*80, submapview->thumbnail->getPos().x+submapview->thumbnail->getSize().x-10);

			m_vSubMap.push_back(submapview);
			cntr++;
		}

		if(cntr)
			setSize(m_cSize.x, 145.0f+(cntr-1)*80);
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
					loadSubMapView();
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
}