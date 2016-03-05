#pragma once

#include "util/geDefines.h"
#include "renderer/rendererGL10.h"
#include "gui/geGUIManager.h"
#include "../../GEAREngine/src/core/Timer.h"
#include "../../GEAREngine/src/mono/src/monoWrapper.h"

#include "scenes/gearSceneFileView.h"
#include "scenes/gearSceneHierarchy.h"
#include "scenes/gearScenePreview.h"
#include "scenes/gearSceneProject.h"
#include "scenes/gearScenePropertyEditor.h"
#include "scenes/gearSceneWorldEditor.h"
#include "scenes/gearSceneConsole.h"
#include "scenes/propertyViews/gePropertyOctree.h"
#include "scenes/gearSceneAnimationEditor.h"
#include "scenes/gearSceneGraphEditor.h"
#include <string>

#ifdef _WIN32
#include <SDL_syswm.h>	//for SDL_SysWMinfo
#endif

#ifdef _WIN32
#define MKDIR _mkdir
#else
#define MKDIR mkdir
#endif

class EditorApp
{
public:
	EditorApp();
	virtual ~EditorApp();

    virtual void init(SDL_Window* window);
    
	void size(int cx, int cy);
	void update(float dt);
	void draw();

	void MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);
	void MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);
    void MouseMButtonDown(float x, float y, int nFlag);
    void MouseMButtonUp(float x, float y, int nFlag);

	void MouseMove(float x, float y, int flag);
	void MouseWheel(int zDelta, int x, int y, int flag);

//#if !defined(__APPLE__) //disable Drag-Drop
	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDropData* dropObject);
	void DragLeave();
//#endif

	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	void DoCommand(int cmd);

#if _WIN32
    static HWND getMainWindowHandle();
#endif

	bool isInitialized()	{	return is_Initialised;	}

#if _WIN32
	static bool showSaveCommonDlg(HWND hWnd, char* out_savefilename, int out_savefilename_size, const char* filter, const char* defaultext, const char* root_dir);
	static bool showOpenCommonDlg(HWND hWnd, char* out_openfilename, int out_openfilename_size, const char* filter, const char* defaultext, const char* root_dir);
#endif
    
	static void setAppDirectory(const char* dir)	{	g_cAppDirectory = dir;	}
	static std::string getAppDirectory()			{	return g_cAppDirectory;	}

private:
#if _WIN32
    static HWND g_hWnd;	//main window
#endif

public:

	static EditorApp* g_pEditorAppInstance;
	geGUIManager* guiManager;
	rendererGL10* rendererGL10Instance;
	bool is_Initialised;

	//static rendererGL10* g_pMainRenderer;
	static std::string g_cAppDirectory;
    
    //app-specific events
    static Uint32 g_iAppSpecificEventType_MenuItemCmd;
};


class EditorGEARApp : public EditorApp
{
public:
    EditorGEARApp();
    ~EditorGEARApp();
    
    void init(SDL_Window* window);
    bool importAssetToMetaData();
    
    static int createNewProject(const char* projectDirectory);
    static void setProjectHomeDirectory(const char* projectDir);
    static const char* getProjectHomeDirectory();
    static void setSceneFileView(gearSceneFileView* ptr);
    static void setSceneHierarchy(gearSceneHierarchy* ptr);
    static void setScenePreview(gearScenePreview* ptr);
    static void setSceneProject(gearSceneProject* ptr);
    static void setScenePropertyEditor(gearScenePropertyEditor* ptr);
    static void setSceneWorldEditor(gearSceneWorldEditor* ptr);
    static void setSceneConsole(gearSceneConsole* ptr);
    
    static gearSceneFileView* getSceneFileView();
    static gearSceneHierarchy* getSceneHierarchy();
    static gearScenePreview* getScenePreview();
    static gearSceneProject* getSceneProject();
    static gearScenePropertyEditor* getScenePropertyEditor();
    static gearSceneWorldEditor* getSceneWorldEditor();
    static gearSceneConsole* getSceneConsole();
    
    static void setPropertyOctree(gePropertyOctree* ptr);
    static gePropertyOctree* getPropertyOctree();
    
    static void setAnimationEditor(gearSceneAnimationEditor* ptr);
    static gearSceneAnimationEditor* getAnimationEditor();

    static char g_cszProjectHomeDirectory[FILENAME_MAX];
    static gearSceneFileView* g_pSceneFileViewPtr;
    static gearSceneHierarchy* g_pSceneHierarchyPtr;
    static gearScenePreview* g_pScenePreviewPtr;
    static gearSceneProject* g_pSceneProjectPtr;
    static gearScenePropertyEditor* g_pScenePropertyEditorPtr;
    static gearSceneWorldEditor* g_pSceneWorldEditorPtr;
    static gearSceneConsole* g_pSceneConsolePtr;
    
    static gePropertyOctree* g_pPropertyOctreePtr;
    
    static gearSceneAnimationEditor* g_pSceneAnimationEditorPtr;
};