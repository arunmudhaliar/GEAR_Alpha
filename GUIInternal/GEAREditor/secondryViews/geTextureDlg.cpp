#include "geTextureDlg.h"
#include "../EditorApp.h"

#include <sys/types.h>

#include <sys/stat.h>

#include <assert.h>
#include <dirent.h>
#include <direct.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

//static int find_textures(rendererGL10* renderer, const char *dirname, geGUIBase* dlg, std::vector<geTextureThumnail*>& textureThumbList);

geTextureDlgMainWindow::geTextureDlgMainWindow():
geWindow("Texture Dialog")
{
}

geTextureDlgMainWindow::~geTextureDlgMainWindow()
{
}

void geTextureDlgMainWindow::onDraw()
{
	std::vector<geGUIBase*>* childList=getChildControls();
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
		geTextureThumbnail* thumb=(geTextureThumbnail*)tvnode;
		geGUIManager::g_pFontArial10_80Ptr->drawString(gxUtil::getFileNameFromPath(thumb->getTexturePtr()->getTextureName()), tvnode->getPos().x, tvnode->getPos().y+ tvnode->getSize().y +geGUIManager::g_pFontArial10_80Ptr->getLineHeight(), m_cSize.x);

		if(tvnode->getPos().y>m_cSize.y)
			break;
	}
}

geTextureDlgInfoWindow::geTextureDlgInfoWindow():
geWindow("Info")
{
}

geTextureDlgInfoWindow::~geTextureDlgInfoWindow()
{
}

void geTextureDlgInfoWindow::onDraw()
{
	std::vector<geGUIBase*>* childList=getChildControls();
	for(std::vector<geGUIBase*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}
}

geTextureDlg::geTextureDlg(geTextureThumbnail* pObserverControlPtr):
	geSecondryView("Texture Dialog")
{
	m_pObserverControlPtr=pObserverControlPtr;
	setSize(geVector2f(1024.0f, 600.0f));
	m_pWindow=NULL;
	m_pInfoWindow=NULL;
}

geTextureDlg::~geTextureDlg()
{
	for(std::vector<geGUIBase*>::iterator it = m_vTextureThumbs.begin(); it != m_vTextureThumbs.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		geTextureThumbnail* thumb=(geTextureThumbnail*)tvnode;
		gxTexture* tex=thumb->getTexturePtr();
		GE_DELETE(tex);
		GE_DELETE(thumb);
	}
	m_vTextureThumbs.clear();
	GE_DELETE(m_pWindow);
	GE_DELETE(m_pInfoWindow);
}

void geTextureDlg::onCreate()
{
	//main window
	m_pWindow = new geTextureDlgMainWindow();
	m_pWindow->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y, false);
	geLayout* mainLayout = m_cLayoutManager.getRootLayout()->createAsParent(m_pWindow);
	//

	//info window
	m_pInfoWindow = new geTextureDlgInfoWindow();
	m_pInfoWindow->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, 250, false);
	mainLayout->createBottom(m_pInfoWindow, 0.2f);
	//

	find_textures(m_pSecondryRenderer, EditorApp::getProjectHomeDirectory(), m_pWindow, m_vTextureThumbs);

	int startX=10;
	int startY=10;
	for(std::vector<geGUIBase*>::iterator it = m_vTextureThumbs.begin(); it != m_vTextureThumbs.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(startX+tvnode->getSize().x+5>m_cSize.x)
		{
			startX=10;
			startY+=tvnode->getSize().y+20;
		}
		tvnode->setPos(startX, startY);
		startX+=tvnode->getSize().x+5;
	}
}

void geTextureDlg::onSize(float cx, float cy)
{

}

void geTextureDlg::onDraw()
{
}

void geTextureDlg::onDestroy()
{
}

bool geTextureDlg::onMouseMove(float x, float y, int flag)
{
	if(flag&MK_LBUTTON)
	{
	}

	return geSecondryView::onMouseMove(x, y, flag);
}

gxTexture* loadTextureFromMeta(CTextureManager& textureManager, int crc)
{
	char metaDataFileName[256];
	sprintf(metaDataFileName, "%x", crc);

	stTexturePacket* texturePack=textureManager.LoadTexture(metaDataFileName);
	if(texturePack)
	{
		gxTexture* pTexture = new gxTexture();
		pTexture->setTexture(texturePack);
		pTexture->setFileCRC(crc);
		if(texturePack->bAlphaTex)
		{
			pTexture->setTextureType(gxTexture::TEX_ALPHA);
		}
		else
		{
			pTexture->setTextureType(gxTexture::TEX_NORMAL);
		}

		return pTexture;
	}
	return NULL;
}

gxTexture* loadTextureFromFile(CTextureManager& textureManager, const char* filename)
{
	char metaInfoFileName[256];
	const char* onlyFilename = gxUtil::getFileNameFromPath(filename);

	sprintf(metaInfoFileName, "%s.meta", filename);

	gxFile metaInfoFile;
	if(metaInfoFile.OpenFile(metaInfoFileName))
	{
		int crc=0;
		metaInfoFile.Read(crc);
		metaInfoFile.CloseFile();
		return loadTextureFromMeta(textureManager, crc);
	}

	return NULL;
}

int geTextureDlg::find_textures(rendererGL10* renderer, const char *dirname, geGUIBase* dlg, std::vector<geGUIBase*>& textureThumbList)
{
    DIR *dir;
    char buffer[PATH_MAX + 2];
    char *p = buffer;
    const char *src;
    char *end = &buffer[PATH_MAX];
    int ok;

    /* Copy directory name to buffer */
    src = dirname;
    while (p < end  &&  *src != '\0') {
        *p++ = *src++;
    }
    *p = '\0';

    /* Open directory stream */
    dir = opendir (dirname);
    if (dir != NULL) {
        struct dirent *ent;

        /* Print all files and directories within the directory */
        while ((ent = readdir (dir)) != NULL) {
            char *q = p;
            char c;

            /* Get final character of directory name */
            if (buffer < q) {
                c = q[-1];
            } else {
                c = ':';
            }

            /* Append directory separator if not already there */
            if (c != ':'  &&  c != '/'  &&  c != '\\') {
                *q++ = '/';
            }

            /* Append file name */
            src = ent->d_name;
            while (q < end  &&  *src != '\0') {
                *q++ = *src++;
            }
            *q = '\0';

            /* Decide what to do with the directory entry */
            switch (ent->d_type) {
            case DT_REG:
                {
					if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG")
						|| util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA"))
					{
						
						gxTexture* texture= loadTextureFromFile(*monoWrapper::mono_engine_getWorld(0)->getTextureManager(), buffer);

						geTextureThumbnail* thumbnail = new geTextureThumbnail();
						thumbnail->create(renderer, dlg, texture, 260, 10, 70, 70);
						m_vTextureThumbs.push_back(thumbnail);
						/*
						geTreeNode* newtvNode = new geTreeNode(renderer, parentNode, ent->d_name, sprite);
						assetUserData* userdata = new assetUserData(assetUserData::ASSET_ONLY_PATH, buffer, NULL);
						newtvNode->setUserData(userdata);
						newtvNode->closeNode();
						*/
					}
				}
                break;

            case DT_DIR:
    //            /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				{
                    find_textures (renderer, buffer, dlg, textureThumbList);
                }
                break;

            default:
                /* Do not device entries */
                /*NOP*/;
            }

        }

        closedir (dir);
        ok = 1;

    } else {
        /* Could not open directory */
        printf ("Cannot open directory %s\n", dirname);
        ok = 0;
    }

    return ok;
}