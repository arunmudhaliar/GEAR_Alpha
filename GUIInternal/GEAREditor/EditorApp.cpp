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
#include "scenes/gearSceneAnimationEditor.h"
#include "AssetImporter.h"

#ifdef _WIN32
#include "../stdafx.h"
#include "../resource.h"
#include <CommCtrl.h>
#endif

#include <sys/stat.h>

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
gearSceneAnimationEditor* EditorGEARApp::g_pSceneAnimationEditorPtr=NULL;
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

void EditorApp::init(SDL_Window* window)
{
	is_Initialised=true;
    rendererGL10Instance = new rendererGL10(window);
    
#ifdef _WIN32
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if(SDL_GetWindowWMInfo(rendererGL10Instance->getWindow(), &info))
	{
		g_hWnd = info.info.win.window;
	}
#endif

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

    rendererGL10Instance->makeCurrent();
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

void EditorApp::MouseMButtonDown(float x, float y, int nFlag)
{
    if(!is_Initialised) return;
    
    guiManager->MouseMButtonDown(x, y, nFlag);

}

void EditorApp::MouseMButtonUp(float x, float y, int nFlag)
{
    if(!is_Initialised) return;
    
    guiManager->MouseMButtonUp(x, y, nFlag);
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

bool EditorApp::createDirectory(std::string path)
{
#ifdef _WIN32
    if(MKDIR(path.c_str())==0)
#else
        if(MKDIR(path.c_str(), 0777)==0)
#endif
        {
            return true;
        }
    
    DEBUG_PRINT("Error creating directory %s", path.c_str());
    return false;
}

bool EditorApp::isDirecoryExist(std::string path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }
    
    return false;
}

bool EditorApp::isFileExist(std::string path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
    {
        return true;
    }
    
    return false;
}

#if _WIN32
HWND EditorApp::getMainWindowHandle()
{
	return g_hWnd;
}

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

void EditorGEARApp::init(SDL_Window* window)
{
    EditorApp::init(window);

    //asset importer.
    geAssetImportDlg* assetImporterDlg = new geAssetImportDlg(&geFontManager::g_cFontManager, rendererGL10Instance);
    //SDL_Thread *thread = SDL_CreateThread( assetImport_secondryThread, "testThread", (void *)assetImporterDlg);
    //SDL_DetachThread(thread);
    //UNUSED(thread);
    assetImport_secondryThread(assetImporterDlg);
    assetImporterDlg->showView();
    //assetImporterDlg->destroyView();

    gearSceneWorldEditor* worldEditorWnd = new gearSceneWorldEditor(guiManager->getLayoutManager()->getFontManager());
    worldEditorWnd->create(rendererGL10Instance, NULL, 0, 0, 300, 200, true);
    guiManager->appendWindow(worldEditorWnd);
    geLayout* worldEditorLayout=guiManager->getLayoutManager()->getRootLayout()->createLeft(worldEditorWnd, 1.0f);
    setSceneWorldEditor(worldEditorWnd);
    
	gearSceneAnimationEditor* animEditorWnd = new gearSceneAnimationEditor(guiManager->getLayoutManager()->getFontManager());
	animEditorWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250, true);
	guiManager->appendWindow(animEditorWnd);
	geLayout* animEditorLayout=worldEditorLayout->createBottom(animEditorWnd, 0.2f);
    setAnimationEditor(animEditorWnd);
    
    gearSceneGraphEditor* graphEditorWnd = new gearSceneGraphEditor(guiManager->getLayoutManager()->getFontManager());
    graphEditorWnd->create(rendererGL10Instance, NULL, 0, 0, 400, 250, true);
    guiManager->appendWindow(graphEditorWnd);
    geLayout* graphEditorLayout=animEditorLayout->createRight(graphEditorWnd, 0.75f);
    UNUSED(graphEditorLayout);
    animEditorWnd->setGraphControlView(graphEditorWnd->getGraphControl());

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

    if(EditorApp::createDirectory(temp_buffer))
    {
        sprintf(temp_buffer, "%s/ProjectSettings", projectDirectory);
        if(EditorApp::createDirectory(temp_buffer))
        {
            sprintf(temp_buffer, "%s/MetaData", projectDirectory);
            if(EditorApp::createDirectory(temp_buffer))
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

bool EditorGEARApp::saveSceneToTempFolder()
{
    std::string tmp_dir = EditorGEARApp::getProjectHomeDirectory();
    tmp_dir+="/Temp";

    if(!EditorApp::isDirecoryExist(tmp_dir))
    {
        DEBUG_PRINT("Creating Temp folder");
        EditorApp::createDirectory(tmp_dir);
    }
    
    if(!EditorGEARApp::getSceneHierarchy()->saveCurrentScene(tmp_dir+"/temp.gearscene"))
    {
        DEBUG_PRINT("Error saving temp file");
        return false;
    }
    
    return true;
}

bool EditorGEARApp::loadSceneFromTempFolder()
{
    std::string tmp_dir = EditorGEARApp::getProjectHomeDirectory();
    tmp_dir+="/Temp/temp.gearscene";
    
    if(!EditorGEARApp::getSceneHierarchy()->loadScene(tmp_dir))
    {
        DEBUG_PRINT("Error loading temp.gearscene");
        return false;
    }
    
    return true;
}

bool EditorGEARApp::saveScene(const std::string& filename)
{
    if(EditorGEARApp::getSceneHierarchy()->saveCurrentScene(filename))
    {
        EditorGEARApp::getSceneProject()->populateProjectView();
        EditorGEARApp::getSceneFileView()->populateFileView();
        return true;
    }
    
    return false;
}

bool EditorGEARApp::loadScene(const std::string &filename)
{
    if(EditorGEARApp::getSceneHierarchy()->loadScene(filename))
    {
        //EditorGEARApp::getSceneProject()->populateProjectView();
        //EditorGEARApp::getSceneFileView()->populateFileView();
        return true;
    }
    
    return false;
}

bool EditorGEARApp::updateCurrentSceneFile(const std::string& filename)
{
    //update the currentscene file
    std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    
    //check if ProjectSettings dir exist or not
    if(!EditorApp::isDirecoryExist(root_dir+"/ProjectSettings"))
    {
        EditorApp::createDirectory(root_dir+"/ProjectSettings");
        DEBUG_PRINT("Creating ProjectSettings directory");
    }
    
    root_dir+="/ProjectSettings/currentscene";
    gxFile currenSceneFile;
    if(currenSceneFile.OpenFile(root_dir.c_str(), gxFile::FILE_w))
    {
        const char* relativepath=AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(filename);
        char unix_path[FILENAME_MAX];
        memset(unix_path, 0, FILENAME_MAX);
        strcpy(unix_path, relativepath);
        geUtil::convertPathToUnixFormat(unix_path);
        currenSceneFile.Write(unix_path);
        currenSceneFile.CloseFile();
        
        std::string wndTitle ="GEAR Alpha [";
        wndTitle+=relativepath;
        wndTitle+=+"]";
        //SetWindowText(hWnd, wndTitle.c_str());
        return true;
    }
    
    return false;
}

bool EditorGEARApp::importAssetToMetaData()
{
    return true;
}

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

gearSceneAnimationEditor* EditorGEARApp::getAnimationEditor()
{
    return g_pSceneAnimationEditorPtr;
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

void EditorGEARApp::setAnimationEditor(gearSceneAnimationEditor* ptr)
{
    g_pSceneAnimationEditorPtr = ptr;
}
