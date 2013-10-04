#include "gearSceneFileView.h"

#include <sys/types.h>

#include <sys/stat.h>

#include <assert.h>
#include <dirent.h>
#include <direct.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "../EditorApp.h"
#include "../win32/MDropSource.h"
#include "assetUserData.h"
#include "../../../GEAREngine/src/core/gxMetaStructures.h"
#include "../../../GEAREngine/src/core/gxAnimationSet.h"
#include <Windows.h>
#include "../../resource.h"

static int find_files(const char *dirname, const char* searchString, geTreeNode* parentNode, Sprite2Dx* spriteArray);
LRESULT CALLBACK Proj_InputDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
char g_cszPrefabName[256];

gearSceneFileView::gearSceneFileView():
geWindow("File View")
{
	m_pSerachStringTextBoxPtr=NULL;
	memset(m_szDirectoryPath, 0, sizeof(m_szDirectoryPath));
}

gearSceneFileView::~gearSceneFileView()
{
	destroyTVUserData(m_cFileTreeView.getRoot());
}

void gearSceneFileView::onCreate()
{
	m_cFileTreeView.create(this, "AssetsFileTV", this);

	m_pSerachStringTextBoxPtr=new geTextBox();
	m_pSerachStringTextBoxPtr->create(getToolBar(), "", 0, 1, 100, 13);
	m_pSerachStringTextBoxPtr->setGUIObserver(this);
	getToolBar()->appendToolBarControl(m_pSerachStringTextBoxPtr);

	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(362, 70, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(46, 26, 16, 16);

	m_cszSprites[2].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[2].setClip(424, 68, 16, 16);

	m_cszSprites[3].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[3].setClip(68, 110, 16, 16);

	m_cszSprites[4].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[4].setClip(110, 238, 16, 16);

}

void gearSceneFileView::onDraw()
{
	m_cFileTreeView.draw();
}

void read3dFile(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;
		if(objID==100)
		{
			tempObj = new gxMesh();
		}
		else
		{
			tempObj = new object3d(objID);
		}

		tempObj->read(file);
		obj->appendChild(tempObj);
		read3dFile(file, tempObj);
	}
}

void deleteAnmationFromObject3d(object3d* obj3d)
{
	gxAnimation* animationController = obj3d->getAnimationController();
	if(animationController)
	{
		std::vector<gxAnimationSet*>* animationSetList=animationController->getAnimationSetList();
		for(std::vector<gxAnimationSet*>::iterator it = animationSetList->begin(); it != animationSetList->end(); ++it)
		{
			gxAnimationSet* animationSet = *it;
			GE_DELETE(animationSet);
		}
		animationSetList->clear();
	}

	std::vector<object3d*>* childList=obj3d->getChildList();
	for(std::vector<object3d*>::iterator it = childList->begin(); it != childList->end(); ++it)
	{
		object3d* childobj = *it;
		deleteAnmationFromObject3d(childobj);
	}
}

void gearSceneFileView::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	object3d* obj=(object3d*)((assetUserData*)tvnode->getUserData())->getAssetObjectPtr();
	if(obj==NULL)
	{
		const char* absolutePath=((assetUserData*)tvnode->getUserData())->getAssetAbsolutePath();
		if(util::GE_IS_EXTENSION(absolutePath, ".fbx") || util::GE_IS_EXTENSION(absolutePath, ".FBX") ||
			util::GE_IS_EXTENSION(absolutePath, ".prefab") || util::GE_IS_EXTENSION(absolutePath, ".PREFAB"))
		{
			char metaInfoFileName[256];
			sprintf(metaInfoFileName, "%s.meta",absolutePath);

			gxFile metaInfoFile;
			if(metaInfoFile.OpenFile(metaInfoFileName))
			{
				int crc=0;
				metaInfoFile.Read(crc);
				metaInfoFile.CloseFile();

				char crcFile[1024];
				sprintf(crcFile, "%s/MetaData/%x", EditorApp::getProjectHomeDirectory(), crc);

				gxFile file_meta;
				if(file_meta.OpenFile(crcFile))
				{
					stMetaHeader metaHeader;
					file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

					int objID=0;
					file_meta.Read(objID);

					object3d* tempObj=NULL;
					if(objID!=100)
					{
						tempObj = new object3d(objID);
						tempObj->read(file_meta);
						read3dFile(file_meta, tempObj);
						obj=tempObj;
						obj->transformationChangedf();
					}
					file_meta.CloseFile();
				}
				deleteAnmationFromObject3d(obj);
			}

			((assetUserData*)tvnode->getUserData())->setAssetObjectPtr(obj, assetUserData::ASSET_MESH_OBJECT);
		}
	}
	EditorApp::getScenePreview()->selectedObject3D(obj);
}

void gearSceneFileView::populateFiles(const char* dirPath)
{
	EditorApp::getScenePreview()->selectedObject3D(NULL);
	STRCPY(m_szDirectoryPath, dirPath);
	destroyTVUserData(m_cFileTreeView.getRoot());
	m_cFileTreeView.clearAndDestroyAll();

	find_files(dirPath, m_pSerachStringTextBoxPtr->getName(), m_cFileTreeView.getRoot(), m_cszSprites);
	m_cFileTreeView.getRoot()->traverseSetWidth(m_cSize.x);
	m_cFileTreeView.refreshTreeView();
}

void gearSceneFileView::destroyTVUserData(geGUIBase* parent)
{
	std::vector<geGUIBase*>* list=parent->getChildControls();

	assetUserData* userdata=(assetUserData*)((geTreeNode*)parent)->getUserData();
	if(userdata)
	{
		object3d* obj=(object3d*)userdata->getAssetObjectPtr();
		GE_DELETE(obj);
	}
	GE_DELETE(userdata);
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* tvnode = *it;
		if(tvnode->getGUIID()!=GEGUI_TREEVIEW_NODE)
			continue;
		destroyTVUserData(tvnode);
	}
}

bool gearSceneFileView::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonDown(x, y, nFlag);
}

bool gearSceneFileView::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonUp(x, y, nFlag);
}

bool gearSceneFileView::onMouseMove(float x, float y, int flag)
{
	//if(!isPointInsideWindow(x, y-getTopMarginOffsetHeight()))
	//	return;

	geTreeNode* selectedNode=m_cFileTreeView.getSelectedNode();
	if((flag&MK_LBUTTON) && selectedNode)
	{
		if(/*m_cFileTreeView.getScrollBar()->isScrollBarVisible() && */m_cFileTreeView.getScrollBar()->isScrollBarGrabbed()/* && x>m_cSize.x-SCROLLBAR_SIZE*/)
			return  true;
		MDataObject* dataObject = new MDataObject(selectedNode, this);
		MDropSource* dropSource = new MDropSource();

		DWORD lpd=0;
		HRESULT ok=DoDragDrop(dataObject, dropSource, DROPEFFECT_MOVE, &lpd);

		dataObject->Release();
		dropSource->Release();
		return true;
	}

	return geWindow::onMouseMove(x, y, flag);
}

void gearSceneFileView::onMouseWheel(int zDelta, int x, int y, int flag)
{
	geWindow::onMouseWheel(zDelta, x, y, flag);
}

void gearSceneFileView::onTextChange(geGUIBase* textBox)
{
	if(textBox==m_pSerachStringTextBoxPtr)
		populateFiles(m_szDirectoryPath);
}

void gearSceneFileView::onDragDrop(int x, int y, MDataObject* dropObject)
{
	geTreeNode* rootNode = m_cFileTreeView.getRoot();
	geGUIBase* droppedDataObject = dropObject->getActualData();

	if(dropObject->getSourcePtr()==EditorApp::getSceneHierarchy())
	{
		if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_DLG), EditorApp::getMainWindowHandle(), reinterpret_cast<DLGPROC>(Proj_InputDlgProc))==IDCANCEL)
		{
			return;
		}
		object3d* obj=(object3d*)droppedDataObject->getUserData();
		//create prefab
		char prefabFileName[512];
		sprintf(prefabFileName, "%s.prefab", g_cszPrefabName);
		char absolutepath[512];
		sprintf(absolutepath, "%s/%s", m_szDirectoryPath, prefabFileName);
		int crc32 = AssetImporter::calcCRC32((unsigned char*)absolutepath);
		gxFile prefabFile;
		if(prefabFile.OpenFile(absolutepath, gxFile::FILE_w))
		{
			prefabFile.CloseFile();

			struct stat fst;
			memset(&fst, 0, sizeof(fst));
			if(stat(absolutepath, &fst)==0) 
			{
				char crcFileName[512];
				sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", crc32);

				obj->setFileCRC(crc32);
				if(AssetImporter::saveObject3DToMetaData(crcFileName, obj, fst))
				{
					//create the meta-info file
					gxFile metaInfoFile;
					char metaInfoFileName[512];
					sprintf(metaInfoFileName, "%s/%s.meta", m_szDirectoryPath, prefabFileName);
					if(metaInfoFile.OpenFile(metaInfoFileName, gxFile::FILE_w))
					{
						metaInfoFile.Write(crc32);
						metaInfoFile.CloseFile();

						//repopulate the file view
						populateFiles(m_szDirectoryPath);
					}
				}
			}
		}
	}
}

static const char* STRCHR_nocase(const char* str, char value)
{
	for(int x=0;x<strlen(str);x++)
	{
		if(tolower(str[x])==tolower(value))
			return &str[x];
	}

	return NULL;
}

static bool isSubString(const char* str, const char* substr)
{
	const char* fond_str=STRCHR_nocase(str, substr[0]);
	if(fond_str==NULL) return false;

	bool bFound=false;

	while(!bFound)
	{
		int sub_strlen=strlen(substr);
		int str_len=strlen(fond_str);
		if(sub_strlen>str_len) return false;

		bFound=true;
		for(int x=0;x<sub_strlen; x++)
		{
			if(tolower(substr[x])!=tolower(*fond_str))
			{
				bFound=false;
				break;
			}
			fond_str++;
		}

		if(bFound)
			return bFound;

		fond_str=STRCHR_nocase(fond_str, substr[0]);
		if(fond_str==NULL) return false;
	}

	return false;
}

static int find_files(const char *dirname, const char* searchString, geTreeNode* parentNode, Sprite2Dx* spriteArray)
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
					if(searchString!=NULL && strlen(searchString)>0)
					{
						if(!isSubString(ent->d_name, searchString))
							break;
					}

					if(!util::GE_IS_EXTENSION(buffer, ".meta"))
					{
						Sprite2Dx* sprite=&spriteArray[1];

						if(util::GE_IS_EXTENSION(buffer, ".png") || util::GE_IS_EXTENSION(buffer, ".PNG") ||
							util::GE_IS_EXTENSION(buffer, ".tga") || util::GE_IS_EXTENSION(buffer, ".TGA"))
							sprite=&spriteArray[2];
						else if(util::GE_IS_EXTENSION(buffer, ".mat") || util::GE_IS_EXTENSION(buffer, ".MAT"))
							sprite=&spriteArray[3];
						else if(util::GE_IS_EXTENSION(buffer, ".prefab") || util::GE_IS_EXTENSION(buffer, ".PREFAB"))
							sprite=&spriteArray[4];
						else
							sprite=&spriteArray[1];

						geTreeNode* newtvNode = new geTreeNode(parentNode, ent->d_name, sprite);
						assetUserData* userdata = new assetUserData(assetUserData::ASSET_ONLY_PATH, buffer, NULL);
						newtvNode->setUserData(userdata);
						newtvNode->closeNode();
					}
				}
                break;

            case DT_DIR:
    //            /* Scan sub-directory recursively */
    //            if (strcmp (ent->d_name, ".") != 0  &&  strcmp (ent->d_name, "..") != 0)
				//{
				//	geTreeNode* newtvNode = new geTreeNode(parentNode, ent->d_name, &spriteArray[0]);
				//	newtvNode->closeNode();
    //                find_directory (buffer, newtvNode, spriteArray);
    //            }
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

LRESULT CALLBACK Proj_InputDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//read the recent dialog
			memset(g_cszPrefabName, 0, sizeof(g_cszPrefabName));
			GX_STRCPY(g_cszPrefabName, "PrefabName");
			SetDlgItemText(hWndDlg, IDC_EDIT_INPUT_NAME, g_cszPrefabName);
			//SendDlgItemMessage(hWndDlg, IDC_EDIT_INPUT_NAME, WM_SETFOCUS,NULL,NULL);
			SendDlgItemMessage(hWndDlg, IDC_EDIT_INPUT_NAME, EM_SETSEL, 0, -1);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		switch(wParam)
		{
			case IDOK:
			{
				GetDlgItemText(hWndDlg, IDC_EDIT_INPUT_NAME, g_cszPrefabName, sizeof(g_cszPrefabName));
				char* trimmed=geUtil::trimwhitespace(g_cszPrefabName);
				if(strlen(trimmed))
				{
					char temp[256];
					GX_STRCPY(temp,trimmed);
					GX_STRCPY(g_cszPrefabName,temp);

					EndDialog(hWndDlg, IDOK);
				}
				else
					EndDialog(hWndDlg, IDCANCEL);
				return true;
			}
			break;

			case IDCANCEL:
			{
				EndDialog(hWndDlg, IDCANCEL);
				return true;
			}
			break;
		}
		break;

	case WM_CLOSE:
		{
			return EndDialog(hWndDlg, IDCANCEL);
		}
		break;
	}

	return FALSE;
}