#ifndef EDITORAPP_H
#define EDITORAPP_H

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
#include <string>

#ifdef _WIN32
#include <SDL_syswm.h>	//for SDL_SysWMinfo
#endif

#ifdef _WIN32
#define MKDIR _mkdir
#else
#define MKDIR mkdir
#endif

#if DEPRECATED
LRESULT CALLBACK Proj_AssetImportDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif

class EditorApp
{
public:
	EditorApp();
	virtual ~EditorApp();

#if DEPRECATED
	virtual void init(HWND hWnd, HINSTANCE hInst);
#else
    virtual void init(SDL_Window* window);
#endif
    
	void size(int cx, int cy);
	void update(float dt);
	void draw();

	void MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);
	void MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);

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
	//static rendererGL10* getMainRenderer()	{	return g_pMainRenderer;	}

	bool isInitialized()	{	return m_bInitialised;	}

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
	geGUIManager* m_pGUIManager;
	rendererGL10* m_pRendererGL10;
	bool m_bInitialised;

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
    
#if DEPRECATED
    void init(HWND hWnd, HINSTANCE hInst);
#else
    void init(SDL_Window* window);
#endif
    
    static int createNewProject(const char* projectDirectory);
    
#if DEPRECATED
    bool importAssetToMetaData(HWND hWnd, HINSTANCE hInst);
#else
    bool importAssetToMetaData();
#endif
    
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

    static char g_cszProjectHomeDirectory[FILENAME_MAX];

    static gearSceneFileView* g_pSceneFileViewPtr;
    static gearSceneHierarchy* g_pSceneHierarchyPtr;
    static gearScenePreview* g_pScenePreviewPtr;
    static gearSceneProject* g_pSceneProjectPtr;
    static gearScenePropertyEditor* g_pScenePropertyEditorPtr;
    static gearSceneWorldEditor* g_pSceneWorldEditorPtr;
    static gearSceneConsole* g_pSceneConsolePtr;
    
    static gePropertyOctree* g_pPropertyOctreePtr;
};
#endif