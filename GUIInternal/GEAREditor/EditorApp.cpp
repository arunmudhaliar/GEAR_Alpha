#include "EditorApp.h"
#ifdef _WIN32
#include <direct.h>
#endif

#include "../../GEAREngine/src/mono/src/monoWrapper.h"
#include "scenes/gearSceneProject.h"
#include "scenes/gearSceneHierarchy.h"
#include "scenes/gearScenePropertyEditor.h"
#include "scenes/gearSceneWorldEditor.h"
#include "scenes/gearSceneFileView.h"
#include "scenes/gearSceneSettings.h"
#include "AssetImporter.h"

#ifdef _WIN32
#include "../stdafx.h"
#include "../resource.h"
#include <CommCtrl.h>
#endif

char EditorGEARApp::g_cszProjectHomeDirectory[FILENAME_MAX];
EditorApp* EditorApp::g_pEditorAppInstance = NULL;

#if _WIN32
HWND EditorApp::g_hWnd=NULL;
#endif

gearSceneFileView* EditorGEARApp::g_pSceneFileViewPtr=NULL;
gearSceneHierarchy* EditorGEARApp::g_pSceneHierarchyPtr=NULL;
gearScenePreview* EditorGEARApp::g_pScenePreviewPtr=NULL;
gearSceneProject* EditorGEARApp::g_pSceneProjectPtr=NULL;
gearScenePropertyEditor* EditorGEARApp::g_pScenePropertyEditorPtr=NULL;
gearSceneWorldEditor* EditorGEARApp::g_pSceneWorldEditorPtr=NULL;
gearSceneConsole* EditorGEARApp::g_pSceneConsolePtr=NULL;
gePropertyOctree* EditorGEARApp::g_pPropertyOctreePtr=NULL;
std::string EditorApp::g_cAppDirectory="";
Uint32 EditorApp::g_iAppSpecificEventType_MenuItemCmd=-1;

//rendererGL10* EditorApp::g_pMainRenderer=NULL;

EditorApp::EditorApp()
{
	EditorApp::g_pEditorAppInstance = this;
	is_Initialised=false;
	rendererGL10Instance = NULL;
    guiManager = NULL;
    g_iAppSpecificEventType_MenuItemCmd=-1;
}

EditorApp::~EditorApp()
{
	guiManager->reset();
    GE_DELETE(guiManager);
	GE_DELETE(rendererGL10Instance);
}

#if DEPRECATED
void EditorApp::init(HWND hWnd, HINSTANCE hInst)
{
	g_hWnd=hWnd;
#else
void EditorApp::init(SDL_Window* window)
{
#endif
	is_Initialised=true;
#if DEPRECATED
	rendererGL10Instance = new rendererGL10(hWnd);
#else
    rendererGL10Instance = new rendererGL10(window);
#endif
    
#ifdef _WIN32
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if(SDL_GetWindowWMInfo(rendererGL10Instance->getWindow(), &info))
	{
		g_hWnd = info.info.win.window;
	}
#endif

	//EditorApp::g_pMainRenderer=rendererGL10Instance;

	rendererGL10Instance->setupRenderer();
	rendererGL10Instance->loadDefaultRenderState();

#ifdef _WIN32
	rendererGL10Instance->swapGLBuffer();
#endif

    guiManager = new geGUIManager(&geFontManager::g_cFontManager);
	guiManager->init(rendererGL10Instance);
    geFontManager::InitializeFonts();
    
    //App specific events
    g_iAppSpecificEventType_MenuItemCmd = SDL_RegisterEvents(1);
}

void EditorApp::size(int cx, int cy)
{
	if(!is_Initialised) return;

	if(rendererGL10Instance)
	{
		rendererGL10Instance->setViewPort(cx, cy);
		guiManager->size(cx, cy);
	}
}

void EditorApp::update(float dt)
{
	if(!is_Initialised) return;

	guiManager->update(dt);
}

void EditorApp::draw()
{
	if(!is_Initialised) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	guiManager->draw();
	rendererGL10Instance->swapGLBuffer();
}

void EditorApp::MouseLButtonDown(float x, float y, int nFlag)
{
	if(!is_Initialised) return;

	guiManager->MouseLButtonDown(x, y, nFlag);
}

void EditorApp::MouseLButtonUp(float x, float y, int nFlag)
{
	if(!is_Initialised) return;

	guiManager->MouseLButtonUp(x, y, nFlag);
}

void EditorApp::MouseRButtonDown(float x, float y, int nFlag)
{
	if(!is_Initialised) return;

	guiManager->MouseRButtonDown(x, y, nFlag);
}

void EditorApp::MouseRButtonUp(float x, float y, int nFlag)
{
	if(!is_Initialised) return;

	guiManager->MouseRButtonUp(x, y, nFlag);
}

void EditorApp::MouseMove(float x, float y, int flag)
{	
	if(!is_Initialised) return;

	guiManager->MouseMove(x, y, flag);
}

void EditorApp::MouseWheel(int zDelta, int x, int y, int flag)
{
	if(!is_Initialised) return;

	guiManager->MouseWheel(zDelta, x, y, flag);
}

//#if !defined(__APPLE__) //disable Drag-Drop
void EditorApp::DragEnter(int x, int y)
{
	guiManager->DragEnter(x, y);
}

void EditorApp::DragDrop(int x, int y, MDropData* dropObject)
{
	guiManager->DragDrop(x, y, dropObject);
}

void EditorApp::DragLeave()
{
	guiManager->DragLeave();
}
//#endif

bool EditorApp::KeyDown(int charValue, int flag)
{
	if(!is_Initialised) return false;

	return guiManager->KeyDown(charValue, flag);
}

bool EditorApp::KeyUp(int charValue, int flag)
{
	if(!is_Initialised) return false;

	return guiManager->KeyUp(charValue, flag);
}

void EditorApp::DoCommand(int cmd)
{
	if(!is_Initialised) return;

	guiManager->DoCommand(cmd);
}

#if _WIN32
HWND EditorApp::getMainWindowHandle()
{
	return g_hWnd;
}
#endif


#ifdef _WIN32
bool EditorApp::showSaveCommonDlg(HWND hWnd, char* out_savefilename, int out_savefilename_size, const char* filter, const char* defaultext, const char* root_dir)
{
	memset(out_savefilename, 0, out_savefilename_size);

	OPENFILENAME saveFileDialog;
	ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
	saveFileDialog.lStructSize= sizeof(saveFileDialog);
	saveFileDialog.hwndOwner = hWnd;
	saveFileDialog.lpstrFilter = filter;
	saveFileDialog.lpstrFile = out_savefilename;
	saveFileDialog.nMaxFile = out_savefilename_size;
	saveFileDialog.lpstrInitialDir = root_dir;
	saveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
	saveFileDialog.lpstrDefExt = defaultext;
	return GetSaveFileName(&saveFileDialog);
}

bool EditorApp::showOpenCommonDlg(HWND hWnd, char* out_openfilename, int out_openfilename_size, const char* filter, const char* defaultext, const char* root_dir)
{
	OPENFILENAME openFileDialog;
	ZeroMemory(&openFileDialog, sizeof(openFileDialog));
	openFileDialog.lStructSize= sizeof(openFileDialog);
	openFileDialog.hwndOwner = hWnd;
	openFileDialog.lpstrFilter = filter;
	openFileDialog.lpstrFile = out_openfilename;
	openFileDialog.lpstrFile[0] = '\0';
	openFileDialog.nMaxFile = out_openfilename_size;
	openFileDialog.lpstrInitialDir = root_dir;
	openFileDialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	openFileDialog.lpstrDefExt = defaultext;
	return GetOpenFileName(&openFileDialog);
}

#if DEPRECATED
LRESULT CALLBACK Proj_AssetImportDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			SendDlgItemMessage(hWndDlg, IDC_ASSET_PROGRESS_BAR, PBM_SETSTEP, 1, 0);
			ShowWindow(hWndDlg, SW_SHOW);
			AssetImporter assetImporter;
			assetImporter.importAssets(EditorApp::getProjectHomeDirectory(), hWndDlg, IDC_ASSET_PROGRESS_BAR, IDC_ASSET_FILE_STATIC);
			EndDialog(hWndDlg, 0);
			return TRUE;
		}
		break;

	case WM_COMMAND:
		break;

	case WM_CLOSE:
		{
			return EndDialog(hWndDlg, 0);
		}
		break;
	}

	return FALSE;
}
#endif
#endif


///////////////////////////////////////////////////////////////////////////////////////////

EditorGEARApp::EditorGEARApp():
    EditorApp()
{
        
}
    
EditorGEARApp::~EditorGEARApp()
{
        
}

int assetImport_secondryThread( void *ptr )
{
    geAssetImportDlg* assetImporterDlg = (geAssetImportDlg*)ptr;
    AssetImporter assetImporter;
    assetImporter.importAssets(EditorGEARApp::getProjectHomeDirectory(), assetImporterDlg);
    return 0;
}
    
#if DEPRECATED
void EditorGEARApp::init(HWND hWnd, HINSTANCE hInst)
{
    EditorApp::init(hWnd, hInst);
#else
void EditorGEARApp::init(SDL_Window* window)
{
    EditorApp::init(window);
#endif
    
    //import all assets to the metadata folder
    //importAssetToMetaData(hWnd, hInst);
    
    geAssetImportDlg* assetImporterDlg = new geAssetImportDlg(&geFontManager::g_cFontManager, rendererGL10Instance);
    SDL_Thread *thread = SDL_CreateThread( assetImport_secondryThread, "testThread", (void *)assetImporterDlg);
    UNUSED(thread);
    assetImporterDlg->showView();
    //assetImporterDlg->destroyView();

//    AssetImporter assetImporter;
//    assetImporter.importAssets(EditorGEARApp::getProjectHomeDirectory(), NULL);

    gearSceneWorldEditor* worldEditorWnd = new gearSceneWorldEditor(guiManager->getLayoutManager()->getFontManager());
    worldEditorWnd->create(rendererGL10Instance, NULL, 0, 0, 300, 200, true);
    guiManager->appendWindow(worldEditorWnd);
    geLayout* worldEditorLayout=guiManager->getLayoutManager()->getRootLayout()->createAsParent(worldEditorWnd);
    setSceneWorldEditor(worldEditorWnd);
    
	//gearSceneAnimationEditor* animEditorWnd = new gearSceneAnimationEditor(guiManager->getLayoutManager()->getFontManager());
	//animEditorWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250);
	//guiManager->appendWindow(animEditorWnd);
	//geLayout* animEditorLayout=worldEditorLayout->createBottom(animEditorWnd, 0.2f);

    gearSceneProject* projectWindow = new gearSceneProject(guiManager->getLayoutManager()->getFontManager());
    projectWindow->create(rendererGL10Instance, NULL, 0, 0, 300, 200, true);
    guiManager->appendWindow(projectWindow);
    geLayout* projectLayout=worldEditorLayout->createRight(projectWindow, 0.5f);
    setSceneProject(projectWindow);
    
    gearScenePropertyEditor* propertyEditorWnd = new gearScenePropertyEditor(guiManager->getLayoutManager()->getFontManager());
    propertyEditorWnd->create(rendererGL10Instance, NULL, 0, 300, 200, 250);
    guiManager->appendWindow(propertyEditorWnd);
    geLayout* propertyEditorLayout=projectLayout->createRight(propertyEditorWnd);
    setScenePropertyEditor(propertyEditorWnd);
    
    gearSceneConsole* consoleWindow = new gearSceneConsole(guiManager->getLayoutManager()->getFontManager());
    consoleWindow->create(rendererGL10Instance, NULL, 0, 0, 300, 200, true);
    guiManager->appendWindow(consoleWindow);
    propertyEditorLayout->appendWindow(consoleWindow);
    setSceneConsole(consoleWindow);
    
    gearSceneSettings* settingsWindow = new gearSceneSettings(guiManager->getLayoutManager()->getFontManager());
    settingsWindow->create(rendererGL10Instance, NULL, 0, 0, 300, 500);
    guiManager->appendWindow(settingsWindow);
    propertyEditorLayout->appendWindow(settingsWindow);
    
    propertyEditorLayout->setActiveWindow(0);
    
    gearSceneHierarchy* hierarchyWnd = new gearSceneHierarchy(guiManager->getLayoutManager()->getFontManager());
    hierarchyWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250, true);
    guiManager->appendWindow(hierarchyWnd);
    geLayout* hierarchyLayout=projectLayout->createTop(hierarchyWnd, 0.45f);
    UNUSED(hierarchyLayout);
    setSceneHierarchy(hierarchyWnd);
    //hierarchyLayout->appendWindow(
    
    gearSceneFileView* fileViewWnd = new gearSceneFileView(guiManager->getLayoutManager()->getFontManager());
    fileViewWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250, true);
    guiManager->appendWindow(fileViewWnd);
    geLayout* fileViewLayout=projectLayout->createRight(fileViewWnd, 0.55f);
    UNUSED(fileViewLayout);
    setSceneFileView(fileViewWnd);
    
    gearScenePreview* previewWnd = new gearScenePreview(guiManager->getLayoutManager()->getFontManager());
    previewWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250);
    guiManager->appendWindow(previewWnd);
    geLayout* previewLayout=propertyEditorLayout->createBottom(previewWnd, 0.35f);
    UNUSED(previewLayout);
    setScenePreview(previewWnd);
}
    
int EditorGEARApp::createNewProject(const char* projectDirectory)
{
    char temp_buffer[FILENAME_MAX];
    sprintf(temp_buffer, "%s/Assets", projectDirectory);
#ifdef _WIN32
    if(MKDIR(temp_buffer)==0)
#else
        if(MKDIR(temp_buffer, 0777)==0)
#endif
        {
            sprintf(temp_buffer, "%s/ProjectSettings", projectDirectory);
#ifdef _WIN32
            if(MKDIR(temp_buffer)==0)
#else
                if(MKDIR(temp_buffer, 0777)==0)
#endif
                {
                    sprintf(temp_buffer, "%s/MetaData", projectDirectory);
#ifdef _WIN32
                    if(MKDIR(temp_buffer)==0)
#else
                        if(MKDIR(temp_buffer, 0777)==0)
#endif
                        {
                            //add to recent projects file
                            FILE* fp= fopen("recentProjects", "a");
                            if(fp)
                            {
                                fprintf(fp, "%s\n", projectDirectory);
                                fclose(fp);
                            }
                            return 0;
                        }
                }
        }
    
    return -1;
}

#if DEPRECATED
bool EditorGEARApp::importAssetToMetaData(HWND hWnd, HINSTANCE hInst)
{
    HWND hWndprogress=CreateDialog(hInst, MAKEINTRESOURCE(IDD_ASSET_IMPORT_PROGRESS_DLG), hWnd, reinterpret_cast<DLGPROC>(Proj_AssetImportDlgProc));
    return true;
}
#else
bool EditorGEARApp::importAssetToMetaData()
{
    return true;
}
#endif

void EditorGEARApp::setProjectHomeDirectory(const char* projectDir)
{
    STRCPY(g_cszProjectHomeDirectory, projectDir);
}

const char* EditorGEARApp::getProjectHomeDirectory()
{
    return g_cszProjectHomeDirectory;
}

gearSceneFileView* EditorGEARApp::getSceneFileView()
{
    return g_pSceneFileViewPtr;
}

gearSceneHierarchy* EditorGEARApp::getSceneHierarchy()
{
    return g_pSceneHierarchyPtr;
}

gearScenePreview* EditorGEARApp::getScenePreview()
{
    return g_pScenePreviewPtr;
}

gearSceneProject* EditorGEARApp::getSceneProject()
{
    return g_pSceneProjectPtr;
}

gearScenePropertyEditor* EditorGEARApp::getScenePropertyEditor()
{
    return g_pScenePropertyEditorPtr;
}

gearSceneWorldEditor* EditorGEARApp::getSceneWorldEditor()
{
    return g_pSceneWorldEditorPtr;
}

gearSceneConsole* EditorGEARApp::getSceneConsole()
{
    return g_pSceneConsolePtr;
}

gePropertyOctree* EditorGEARApp::getPropertyOctree()
{
    return g_pPropertyOctreePtr;
}

void EditorGEARApp::setSceneFileView(gearSceneFileView* ptr)
{
    g_pSceneFileViewPtr=ptr;
}

void EditorGEARApp::setSceneHierarchy(gearSceneHierarchy* ptr)
{
    g_pSceneHierarchyPtr=ptr;
}

void EditorGEARApp::setScenePreview(gearScenePreview* ptr)
{
    g_pScenePreviewPtr=ptr;
}

void EditorGEARApp::setSceneProject(gearSceneProject* ptr)
{
    g_pSceneProjectPtr=ptr;
}

void EditorGEARApp::setScenePropertyEditor(gearScenePropertyEditor* ptr)
{
    g_pScenePropertyEditorPtr=ptr;
}

void EditorGEARApp::setSceneWorldEditor(gearSceneWorldEditor* ptr)
{
    g_pSceneWorldEditorPtr = ptr;
}

void EditorGEARApp::setSceneConsole(gearSceneConsole* ptr)
{
    g_pSceneConsolePtr=ptr;
}

void EditorGEARApp::setPropertyOctree(gePropertyOctree* ptr)
{
    g_pPropertyOctreePtr=ptr;
}

