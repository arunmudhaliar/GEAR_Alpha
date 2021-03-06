#include "gearSceneProject.h"

#include <sys/types.h>

#include <sys/stat.h>

#include <assert.h>
#include <dirent.h>
#if !defined(__APPLE__)
    #include <direct.h>
#endif
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "../EditorApp.h"

#if DEPRECATED
#include "../win32/MDropSource.h"
#endif
#include"assetUserData.h"

static int find_directory(rendererGL10* renderer, const char *dirname, geTreeNode* parentNode, Sprite2Dx* spriteArray, geFontManager* fontmanager);

gearSceneProject::gearSceneProject(geFontManager* fontmanager):
geWindow("Project", fontmanager)
{
	//m_pFileViewScenePtr=NULL;
    m_pAssetTreeView = new geTreeView(fontmanager);
}

gearSceneProject::~gearSceneProject()
{
	destroyTVUserData(m_pAssetTreeView->getRoot());
    GE_DELETE(m_pAssetTreeView)
}

void gearSceneProject::onCreate()
{
	m_pAssetTreeView->create(m_pRenderer, this, "AssetsTV", this);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(362, 70, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(46, 26, 16, 16);

	populateProjectView();
}

void gearSceneProject::populateProjectView()
{
	destroyTVUserData(m_pAssetTreeView->getRoot());
	m_pAssetTreeView->clearAndDestroyAll();

	find_directory(m_pRenderer, EditorGEARApp::getProjectHomeDirectory(), m_pAssetTreeView->getRoot(), m_cszSprites, m_pFontManagerPtr);
	m_pAssetTreeView->getRoot()->traverseSetWidth(m_cSize.x);
	m_pAssetTreeView->refreshTreeView();
}

void gearSceneProject::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_pAssetTreeView->draw();
}

void gearSceneProject::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	EditorGEARApp::getSceneFileView()->populateFiles(((assetUserData*)tvnode->getUserData())->getAssetPath());
}

void gearSceneProject::destroyTVUserData(geGUIBase* parent)
{
	std::vector<geGUIBase*>* list=parent->getChildControls();

	assetUserData* userdata=(assetUserData*)((geTreeNode*)parent)->getUserData();
	GE_DELETE(userdata);
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		destroyTVUserData(tvnode);
	}
}

bool gearSceneProject::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonDown(x, y, nFlag);
}

bool gearSceneProject::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonUp(x, y, nFlag);
}

bool gearSceneProject::onMouseMove(float x, float y, int flag)
{
	//geTreeNode* selectedNode=m_pAssetTreeView->getSelectedNode();
	//if((flag&MK_LBUTTON) && selectedNode)
	//{
	//	MDataObject* dataObject = new MDataObject(selectedNode);
	//	MDropSource* dropSource = new MDropSource();

	//	DWORD lpd=0;
	//	HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

	//	dataObject->Release();
	//	dropSource->Release();
	//	return;
	//}

	return geWindow::onMouseMove(x, y, flag);
}

void gearSceneProject::onMouseWheel(int zDelta, int x, int y, int flag)
{
	geWindow::onMouseWheel(zDelta, x, y, flag);
}

//#if !defined(__APPLE__) //disable Drag-Drop
void gearSceneProject::onDragEnter(int x, int y)
{

}

void gearSceneProject::onDragDrop(int x, int y, MDropData* dropObject)
{

}

void gearSceneProject::onDragLeave()
{
}
//#endif

static int find_directory(rendererGL10* renderer, const char *dirname, geTreeNode* parentNode, Sprite2Dx* spriteArray, geFontManager* fontmanager)
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
					//if (strlen(buffer)>=5 && strcmp(&buffer[strlen(buffer)-4], ".fbx")==0)
					//{
					//	object3d* obj = engine_loadFBX(buffer);
					//	if(obj)
					//	{
					//		assetUserData* userdata = (assetUserData*)parentNode->getUserData();
					//		if(userdata==NULL)
					//		{
					//			userdata = new assetUserData(99, buffer, NULL);
					//			parentNode->setUserData(userdata);
					//		}
					//		std::vector<object3d*>* list = (std::vector<object3d*>*)userdata->getAssetObjectPtr();
					//		if(userdata->getAssetObjectPtr()==NULL)
					//		{
					//			list = new std::vector<object3d*>();
					//		}
					//		list->push_back(obj);
					//	}
					//}
				}
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				{
					geTreeNode* newtvNode = new geTreeNode(renderer, parentNode, ent->d_name, &spriteArray[0], fontmanager);

					assetUserData* userdata = new assetUserData(assetUserData::ASSET_ONLY_PATH, buffer, NULL);
					newtvNode->setUserData(userdata);

					newtvNode->closeNode();
                    find_directory (renderer, buffer, newtvNode, spriteArray, fontmanager);
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