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
#include "../../../GEAREngine/src/core/gxSkinnedMesh.h"

LRESULT CALLBACK Proj_InputDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
char g_cszPrefabName[256];

gearSceneFileView::gearSceneFileView():
geWindow("File View")
{
	m_pSerachStringTextBoxPtr=NULL;
	m_pPreviewObj_Cube=NULL;
	memset(m_szDirectoryPath, 0, sizeof(m_szDirectoryPath));
}

gearSceneFileView::~gearSceneFileView()
{
	GE_DELETE(m_pPreviewObj_Cube);
	destroyTVUserData(m_cFileTreeView.getRoot());
}

void gearSceneFileView::onCreate()
{
	m_cFileTreeView.create(m_pRenderer, this, "AssetsFileTV", this);

	m_pSerachStringTextBoxPtr=new geTextBox();
	m_pSerachStringTextBoxPtr->create(m_pRenderer, getToolBar(), "", 0, 1, 100, 13);
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

	loadPreviewObjects();
}

void gearSceneFileView::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_cFileTreeView.draw();
}

void gearSceneFileView::read3dFile(gxFile& file, object3d* obj)
{
	int nChild=0;
	file.Read(nChild);

	for(int x=0;x<nChild; x++)
	{
		int objID=0;
		file.Read(objID);
		object3d* tempObj=NULL;
		switch(objID)
		{
		case OBJECT3D_MESH:
			tempObj = new gxMesh();
			break;
		case OBJECT3D_SKINNED_MESH:
			tempObj = new gxSkinnedMesh();
			break;
		case OBJECT3D_LIGHT:
			tempObj = new gxLight();
			break;
		default:
			tempObj = new object3d(objID);
		}

		tempObj->read(file);
		obj->appendChild(tempObj);
		read3dFile(file, tempObj);
	}
}

void gearSceneFileView::deleteAnmationFromObject3d(object3d* obj3d)
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

void gearSceneFileView::loadPreviewObjects()
{
	//cube
	gxFile file_meta;
	object3d* obj=NULL;
	if(file_meta.OpenFile("res//preview//cube.preview"))
	{
		stMetaHeader metaHeader;
		file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

		int objID=0;
		file_meta.Read(objID);

		object3d* tempObj=NULL;
		if(objID!=OBJECT3D_MESH)
		{
			tempObj = new object3d(objID);
			tempObj->read(file_meta);
			read3dFile(file_meta, tempObj);
			obj=tempObj;
			//arun:transform obj->transformationChangedf();
		}
		file_meta.CloseFile();
	}
	deleteAnmationFromObject3d(obj);
	m_pPreviewObj_Cube=obj;
	//

	//Material
	if(file_meta.OpenFile("res//preview//Material.mat.preview"))
	{
		stMetaHeader metaHeader;
		file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

		m_cPreviewMaterial.read(file_meta);
		file_meta.CloseFile();
	}
	//
}

void gearSceneFileView::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
	object3d* obj=(object3d*)((assetUserData*)tvnode->getUserData())->getAssetObjectPtr();
	if(obj==NULL)
	{
		const char* relativePath=((assetUserData*)tvnode->getUserData())->getAssetPath();
		char crcFile[1024];
		if(relativePath)
		{
			char metaInfoFileName[1024];
			sprintf(metaInfoFileName, "%s/Assets%s.meta", EditorApp::getProjectHomeDirectory(), relativePath);

			gxFile metaInfoFile;
			if(metaInfoFile.OpenFile(metaInfoFileName))
			{
				int crc=0;
				metaInfoFile.Read(crc);
				metaInfoFile.CloseFile();

				sprintf(crcFile, "%s/MetaData/%x", EditorApp::getProjectHomeDirectory(), crc);
			}
		}

		if(util::GE_IS_EXTENSION(relativePath, ".fbx") || util::GE_IS_EXTENSION(relativePath, ".FBX") ||
			util::GE_IS_EXTENSION(relativePath, ".prefab") || util::GE_IS_EXTENSION(relativePath, ".PREFAB"))
		{
			gxFile file_meta;
			if(file_meta.OpenFile(crcFile))
			{
				stMetaHeader metaHeader;
				file_meta.ReadBuffer((unsigned char*)&metaHeader, sizeof(metaHeader));

				int objID=0;
				file_meta.Read(objID);

				object3d* tempObj=NULL;
				switch(objID)
				{
				case OBJECT3D_MESH:
					tempObj = new gxMesh();
					break;
				case OBJECT3D_SKINNED_MESH:
					tempObj = new gxSkinnedMesh();
					break;
				case OBJECT3D_LIGHT:
					tempObj = new gxLight();
					break;

				default:
					tempObj = new object3d(objID);
				}

				if(tempObj)
				{
					tempObj->read(file_meta);
					read3dFile(file_meta, tempObj);
					obj=tempObj;
					//arun:transform obj->transformationChangedf();
				}
				file_meta.CloseFile();
			}
			deleteAnmationFromObject3d(obj);
			gxWorld* world=monoWrapper::mono_engine_getWorld(0);
			world->loadMaterialFromObject3d(obj);

			((assetUserData*)tvnode->getUserData())->setAssetObjectPtr(obj, assetUserData::ASSET_MESH_OBJECT);
		}
		else if(util::GE_IS_EXTENSION(relativePath, ".mat") || util::GE_IS_EXTENSION(relativePath, ".MAT"))
		{
			gxFile file_meta;
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
					gxMesh* mesh=(gxMesh*)m_pPreviewObj_Cube->getChild(0);
					gxTriInfo* triinfo = mesh->getTriInfo(0);
					triinfo->setMaterial(matchingMaterial);
					obj=m_pPreviewObj_Cube;
				}
			}

			if(!matchingMaterial)
			{
				sprintf(crcFile, "%s/MetaData/%x", EditorApp::getProjectHomeDirectory(), crc32);
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
						gxWorld* world=monoWrapper::mono_engine_getWorld(0);
						submap->loadTextureFromMeta(*world->getTextureManager(), submap->getTextureCRC());
					}

					gxMesh* mesh=(gxMesh*)m_pPreviewObj_Cube->getChild(0);
					gxTriInfo* triinfo = mesh->getTriInfo(0);
					triinfo->setMaterial(material);
					world->getMaterialList()->push_back(triinfo->getMaterial());
					obj=m_pPreviewObj_Cube;
				}
			}
			else
			{

			}
		}
		else if(util::GE_IS_EXTENSION(relativePath, ".cs") || util::GE_IS_EXTENSION(relativePath, ".CS"))
		{
			EditorApp::getScenePropertyEditor()->populatePropertyOfOpenInEditor();
		}
		else if(util::GE_IS_EXTENSION(relativePath, ".png") || util::GE_IS_EXTENSION(relativePath, ".PNG")
			|| util::GE_IS_EXTENSION(relativePath, ".tga") || util::GE_IS_EXTENSION(relativePath, ".TGA"))
		{
			HWShaderManager* hwShaderManager = engine_getHWShaderManager();
			//load surface shader
			char mainshaderfilename[1024];
			sprintf(mainshaderfilename, ".//res//shadersWin32//surfaceShader//%s.shader", m_cPreviewMaterial.getMainshaderName());
			m_cPreviewMaterial.setSurfaceShader(hwShaderManager->LoadSurfaceShader(mainshaderfilename));

			gxWorld* world=monoWrapper::mono_engine_getWorld(0);
			//load sub maps
			std::vector<gxSubMap*>* maplist=m_cPreviewMaterial.getSubMapList();
			for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it)
			{
				gxSubMap* submap = *it;
				int crc32=AssetImporter::calcCRC32((unsigned char*)relativePath);
				submap->setTextureCRC(crc32);
				submap->loadTextureFromMeta(*world->getTextureManager(), submap->getTextureCRC());
			}

			gxMesh* mesh=(gxMesh*)m_pPreviewObj_Cube->getChild(0);
			gxTriInfo* triinfo = mesh->getTriInfo(0);
			triinfo->setMaterial(&m_cPreviewMaterial);
			obj=m_pPreviewObj_Cube;
		}
	}
	EditorApp::getScenePreview()->selectedObject3D(obj);
}

void gearSceneFileView::populateFileView()
{
	populateFiles(m_szDirectoryPath);
}

void gearSceneFileView::populateFiles(const char* dirPath)
{
	EditorApp::getScenePreview()->selectedObject3D(NULL);
	STRCPY(m_szDirectoryPath, dirPath);
	destroyTVUserData(m_cFileTreeView.getRoot());
	m_cFileTreeView.clearAndDestroyAll();

	gearSceneFileView::find_files(m_pRenderer, dirPath, m_pSerachStringTextBoxPtr->getName(), m_cFileTreeView.getRoot(), m_cszSprites);
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

	std::vector<geTreeNode*>* selectedNodeList=m_cFileTreeView.getSelectedNodeList();
	if((flag&MK_LBUTTON) && selectedNodeList->size())
	{
		if(/*m_cFileTreeView.getScrollBar()->isScrollBarVisible() && */m_cFileTreeView.getScrollBar()->isScrollBarGrabbed()/* && x>m_cSize.x-SCROLLBAR_SIZE*/)
			return  true;
		std::vector<geGUIBase*>* newlist = new std::vector<geGUIBase*>();

		for(std::vector<geTreeNode*>::iterator it = selectedNodeList->begin(); it != selectedNodeList->end(); ++it)
		{
			geTreeNode* node = *it;
			newlist->push_back(node);
		}

		MDataObject* dataObject = new MDataObject(newlist, this);
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

	std::vector<geGUIBase*>* list = dropObject->getActualDataList();
	for(std::vector<geGUIBase*>::iterator it = list->begin(); it != list->end(); ++it)
	{
		geGUIBase* droppedDataObject = *it;

		if(dropObject->getSourcePtr()==EditorApp::getSceneHierarchy())
		{
			if(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT_DLG), EditorApp::getMainWindowHandle(), reinterpret_cast<DLGPROC>(Proj_InputDlgProc))==IDCANCEL)
			{
				continue;
			}
			object3d* obj=(object3d*)droppedDataObject->getUserData();
			//create prefab
			char prefabFileName[512];
			sprintf(prefabFileName, "%s.prefab", g_cszPrefabName);
			char absolutepath[512];
			sprintf(absolutepath, "%s/%s", m_szDirectoryPath, prefabFileName);
			unsigned int crc32 = AssetImporter::calcCRC32((unsigned char*)AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(absolutepath));
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

					obj->setAssetFileCRC(crc32, absolutepath);
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
}

int gearSceneFileView::find_files(rendererGL10* renderer, const char *dirname, const char* searchString, geTreeNode* parentNode, Sprite2Dx* spriteArray)
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
						if(!gxUtil::isSubString(ent->d_name, searchString))
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

						geTreeNode* newtvNode = new geTreeNode(renderer, parentNode, ent->d_name, sprite);
						assetUserData* userdata = new assetUserData(assetUserData::ASSET_ONLY_PATH, AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(buffer), NULL);
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