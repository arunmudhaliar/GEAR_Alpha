#include "gePropertyMaterial.h"
#include "../../EditorApp.h"

void gePropertyMaterial::onDragDrop(int x, int y, MDataObject* dropObject)
{
	geGUIBase* droppedDataObject = dropObject->getActualData();
	const char* absolutePath=((assetUserData*)((geTreeNode*)droppedDataObject)->getUserData())->getAssetAbsolutePath();

	if (util::GE_IS_EXTENSION(absolutePath, ".mat") || util::GE_IS_EXTENSION(absolutePath, ".MAT"))
	{
		int crc32=AssetImporter::calcCRC32((unsigned char*)absolutePath);

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

			//load surface shader
			char mainshaderfilename[1024];
			sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", material->getMainshaderName());
			material->loadSurfaceShader(mainshaderfilename);
			material->loadLightingShaders();

			//load sub maps
			std::vector<gxSubMap*>* maplist=material->getSubMapList();
			for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it)
			{
				gxSubMap* map = *it;
				map->loadTextureFromMeta(*world->getTextureManager(), map->getTextureCRC());
			}


			m_pTriInfoPtr->setMaterial(material);
			world->getMaterialList()->push_back(m_pTriInfoPtr->getMaterial());
			loadClientViewFromMaterial(material);
		}
	}
}

void gePropertyMaterial::loadClientViewFromMaterial(gxMaterial* material)
{
	//destroy submapview nodes
	for(std::vector<stSubMapView*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		stSubMapView* tvnode = *it;
		GE_DELETE(tvnode);
	}
	m_vSubMap.clear();

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
		text_material->create(m_pRenderer, this, m_pCurrentMaterialPtr->getMaterialName(), 50, 10, 200, 16);
		
		m_pColorControl = new geColorControl();
		m_pColorControl->create(m_pRenderer, this, 10, 10);
		vector4f diffuse=m_pCurrentMaterialPtr->getDiffuseClr();
		m_pColorControl->setControlColor(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_pColorControl->setGUIObserver(this);

		//m_vSubMap
		int cntr=0;
		std::vector<gxSubMap*>* maplist=m_pCurrentMaterialPtr->getSubMapList();
		for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it, ++cntr)
		{
			gxSubMap* map = *it;
			stSubMapView* submapview = new stSubMapView();

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
			submapview->m_pText_tileX = new geTextBox("1.0");
			submapview->m_pText_tileX->create(m_pRenderer, this, tileX_temp_buffer, 100, 50+cntr*80, 60, 16);
			submapview->m_pText_tileX->setGUIObserver(this);
			submapview->m_pText_tileY = new geTextBox("1.0");
			submapview->m_pText_tileY->create(m_pRenderer, this, tileY_temp_buffer, 100, 70+cntr*80, 60, 16);
			submapview->m_pText_tileY->setGUIObserver(this);

			submapview->thumbnail = new geTextureThumbnailExtended();
			submapview->thumbnail->create(m_pRenderer, this, map->getTexture(), 260, 40+cntr*80, 70, 70);
			submapview->thumbnail->setUserData(map);
			m_vSubMap.push_back(submapview);
		}

		if(maplist->size())
			setSize(m_cSize.x, 115.0f+(cntr-1)*80);
	}
}

void gePropertyMaterial::drawNode()
{
	drawRect(&m_cVBClientArea);

	if(m_pCurrentMaterialPtr)
	{
		geGUIManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

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