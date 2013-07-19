#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "util\geDefines.h"
#include "renderer\rendererGL10.h"
#include "gui\geGUIManager.h"

#include "../mono/src/monoWrapper.h"

#include "scenes\gearSceneFileView.h"
#include "scenes\gearSceneHierarchy.h"
#include "scenes\gearScenePreview.h"
#include "scenes\gearSceneProject.h"
#include "scenes\gearScenePropertyEditor.h"
#include "scenes\gearSceneWorldEditor.h"

class EditorApp
{
public:
	EditorApp();
	~EditorApp();

	void init(HWND hWnd);
	void size(int cx, int cy);
	void update(float dt);
	void draw();

	void MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);
	void MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);

	void MouseMove(float x, float y, int flag);
	void MouseWheel(int zDelta, int x, int y, int flag);

	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDataObject* dropObject);
	void DragLeave();

	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	static int createNewProject(const char* projectDirectory);
	bool importAssetToMetaData();

	static void setProjectHomeDirectory(const char* projectDir);
	static const char* getProjectHomeDirectory();



	static void setSceneFileView(gearSceneFileView* ptr);
	static void setSceneHierarchy(gearSceneHierarchy* ptr);
	static void setScenePreview(gearScenePreview* ptr);
	static void setSceneProject(gearSceneProject* ptr);
	static void setScenePropertyEditor(gearScenePropertyEditor* ptr);
	static void setSceneWorldEditor(gearSceneWorldEditor* ptr);

	static gearSceneFileView* getSceneFileView();
	static gearSceneHierarchy* getSceneHierarchy();
	static gearScenePreview* getScenePreview();
	static gearSceneProject* getSceneProject();
	static gearScenePropertyEditor* getScenePropertyEditor();
	static gearSceneWorldEditor* getSceneWorldEditor();
	static HWND getMainWindowHandle()	{	return g_hWnd;	}

private:
	static HWND g_hWnd;	//main window

	geGUIManager m_cGUIManager;
	geVector2i m_cViewPortSz;

	monoWrapper m_cMonoWrapper;
	float angle;
	bool m_bInitialised;

	static char g_cszProjectHomeDirectory[1024];

	static gearSceneFileView* g_pSceneFileViewPtr;
	static gearSceneHierarchy* g_pSceneHierarchyPtr;
	static gearScenePreview* g_pScenePreviewPtr;
	static gearSceneProject* g_pSceneProjectPtr;
	static gearScenePropertyEditor* g_pScenePropertyEditorPtr;
	static gearSceneWorldEditor* g_pSceneWorldEditorPtr;
};

#endif