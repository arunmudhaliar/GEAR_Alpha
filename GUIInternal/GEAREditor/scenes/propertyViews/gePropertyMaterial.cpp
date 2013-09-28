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
			if(material->getTextureReadCRC())
			{
				//load the texture
				material->loadTextureFromMeta(*world->getTextureManager(), material->getTextureReadCRC());
			}
			file_meta.CloseFile();

			m_pTriInfoPtr->setMaterial(material);
			world->getMaterialList()->push_back(m_pTriInfoPtr->getMaterial());
			loadClientViewFromMaterial(material);
		}
	}
}

void gePropertyMaterial::loadClientViewFromMaterial(gxMaterial* material)
{
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
		text_material->create(this, m_pCurrentMaterialPtr->getMaterialName(), 50, 10, 200, 16);
		
		char tileX_temp_buffer[10];
		char tileY_temp_buffer[10];

		if(m_pCurrentMaterialPtr->getTexture())
		{
			const float* texMat4x4=m_pCurrentMaterialPtr->getTexture()->getTextureMatrix()->getMatrix();
			sprintf(tileX_temp_buffer, "%3.2f", texMat4x4[0]);
			sprintf(tileY_temp_buffer, "%3.2f", texMat4x4[5]);
		}
		else
		{
			sprintf(tileX_temp_buffer, "%3.2f", 1.0f);
			sprintf(tileY_temp_buffer, "%3.2f", 1.0f);
		}

		m_pText_tileX = new geTextBox("1.0");
		m_pText_tileX->create(this, tileX_temp_buffer, 100, 40, 50, 16);
		m_pText_tileX->setGUIObserver(this);
		m_pText_tileY = new geTextBox("1.0");
		m_pText_tileY->create(this, tileY_temp_buffer, 100, 60, 50, 16);
		m_pText_tileY->setGUIObserver(this);

		geColorControl* colorControl = new geColorControl();
		colorControl->create(this, 10, 10);
		vector4f diffuse=m_pCurrentMaterialPtr->getDiffuseClr();
		colorControl->setControlColor(diffuse.x, diffuse.y, diffuse.z, diffuse.w);

		geTextureThumbnailExtended* thumbnail = new geTextureThumbnailExtended();
		thumbnail->create(this, m_pCurrentMaterialPtr->getTexture(), 260, 10, 70, 70);
		thumbnail->setUserData(m_pCurrentMaterialPtr);
	}
	//else
	//{
	//	geNullMaterialExtended* thumbnail = new geNullMaterialExtended();
	//	thumbnail->create(this, NULL, 5, 5, m_cSize.x-10, m_cSize.y-10);
	//}
}

void gePropertyMaterial::drawNode()
{
	drawRect(&m_cVBClientArea);

	if(m_pCurrentMaterialPtr)
	{
		geGUIManager::g_pFontArial12Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

		if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
		{
			if(m_bNodeOpen)
				drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}
	}

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(m_pSprite)
		m_pSprite->draw();
}