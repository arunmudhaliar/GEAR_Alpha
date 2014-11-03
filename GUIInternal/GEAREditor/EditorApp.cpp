#include "EditorApp.h"
#include <direct.h>

#include "../mono/src/monoWrapper.h"

#include "scenes\gearSceneProject.h"
#include "scenes\gearSceneHierarchy.h"
#include "scenes\gearScenePropertyEditor.h"
#include "scenes\gearSceneWorldEditor.h"
#include "scenes\gearSceneFileView.h"
#include "scenes\gearSceneSettings.h"
#include "AssetImporter.h"

#include "../stdafx.h"
#include "../resource.h"
#include <CommCtrl.h>

char EditorApp::g_cszProjectHomeDirectory[1024];
HWND EditorApp::g_hWnd=NULL;

gearSceneFileView* EditorApp::g_pSceneFileViewPtr=NULL;
gearSceneHierarchy* EditorApp::g_pSceneHierarchyPtr=NULL;
gearScenePreview* EditorApp::g_pScenePreviewPtr=NULL;
gearSceneProject* EditorApp::g_pSceneProjectPtr=NULL;
gearScenePropertyEditor* EditorApp::g_pScenePropertyEditorPtr=NULL;
gearSceneWorldEditor* EditorApp::g_pSceneWorldEditorPtr=NULL;
gearSceneConsole* EditorApp::g_pSceneConsolePtr=NULL;
gePropertyOctree* EditorApp::g_pPropertyOctreePtr=NULL;
std::string EditorApp::g_cAppDirectory="";

rendererGL10* EditorApp::g_pMainRenderer=NULL;

EditorApp::EditorApp()
{
	m_bInitialised=false;
	m_pRendererGL10 = NULL;
}

EditorApp::~EditorApp()
{
	m_cGUIManager.reset();
	GE_DELETE(m_pRendererGL10);
}

void EditorApp::init(HWND hWnd, HINSTANCE hInst)
{
	g_hWnd=hWnd;
	m_bInitialised=true;
	m_pRendererGL10 = new rendererGL10(hWnd);
	EditorApp::g_pMainRenderer=m_pRendererGL10;

	m_pRendererGL10->setupRenderer();
	m_pRendererGL10->loadDefaultRenderState();

	m_cGUIManager.init(m_pRendererGL10);

	//import all assets to the metadata folder
	//importAssetToMetaData(hWnd, hInst);

	gearSceneWorldEditor* worldEditorWnd = new gearSceneWorldEditor();
	worldEditorWnd->create(m_pRendererGL10, NULL, 0, 0, 300, 200, true);
	m_cGUIManager.appendWindow(worldEditorWnd);
	geLayout* worldEditorLayout=m_cGUIManager.getLayoutManager()->getRootLayout()->createAsParent(worldEditorWnd);
	setSceneWorldEditor(worldEditorWnd);

	gearSceneProject* projectWindow = new gearSceneProject();
	projectWindow->create(m_pRendererGL10, NULL, 0, 0, 300, 200, true);
	m_cGUIManager.appendWindow(projectWindow);
	geLayout* projectLayout=worldEditorLayout->createRight(projectWindow, 0.5f);
	setSceneProject(projectWindow);

	gearScenePropertyEditor* propertyEditorWnd = new gearScenePropertyEditor();
	propertyEditorWnd->create(m_pRendererGL10, NULL, 0, 300, 200, 250);
	m_cGUIManager.appendWindow(propertyEditorWnd);
	geLayout* propertyEditorLayout=projectLayout->createRight(propertyEditorWnd);	
	setScenePropertyEditor(propertyEditorWnd);

	gearSceneConsole* consoleWindow = new gearSceneConsole();
	consoleWindow->create(m_pRendererGL10, NULL, 0, 0, 300, 200, true);
	m_cGUIManager.appendWindow(consoleWindow);
	propertyEditorLayout->appendWindow(consoleWindow);
	setSceneConsole(consoleWindow);

	gearSceneSettings* settingsWindow = new gearSceneSettings();
	settingsWindow->create(m_pRendererGL10, NULL, 0, 0, 300, 500);
	m_cGUIManager.appendWindow(settingsWindow);
	propertyEditorLayout->appendWindow(settingsWindow);

	propertyEditorLayout->setActiveWindow(0);

	gearSceneHierarchy* hierarchyWnd = new gearSceneHierarchy();
	hierarchyWnd->create(m_pRendererGL10, NULL, 0, 0, 400, 250, true);
	m_cGUIManager.appendWindow(hierarchyWnd);
	geLayout* hierarchyLayout=projectLayout->createTop(hierarchyWnd, 0.45f);
	setSceneHierarchy(hierarchyWnd);
	//hierarchyLayout->appendWindow(

	gearSceneFileView* fileViewWnd = new gearSceneFileView();
	fileViewWnd->create(m_pRendererGL10, NULL, 0, 0, 400, 250, true);
	m_cGUIManager.appendWindow(fileViewWnd);
	geLayout* fileViewLayout=projectLayout->createRight(fileViewWnd, 0.55f);
	setSceneFileView(fileViewWnd);

	gearScenePreview* previewWnd = new gearScenePreview();
	previewWnd->create(m_pRendererGL10, NULL, 0, 0, 400, 250);
	m_cGUIManager.appendWindow(previewWnd);
	geLayout* previewLayout=propertyEditorLayout->createBottom(previewWnd, 0.35f);
	setScenePreview(previewWnd);
}

void EditorApp::size(int cx, int cy)
{
	if(!m_bInitialised) return;

	if(m_pRendererGL10)
	{
		m_pRendererGL10->setViewPort(cx, cy);
		m_cGUIManager.size(cx, cy);
	}
}

void EditorApp::update(float dt)
{
	if(!m_bInitialised) return;

	m_cGUIManager.update(dt);
}

void EditorApp::draw()
{
	if(!m_bInitialised) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_cGUIManager.draw();
	m_pRendererGL10->swapGLBuffer();
}

void EditorApp::MouseLButtonDown(float x, float y, int nFlag)
{
	if(!m_bInitialised) return;

	m_cGUIManager.MouseLButtonDown(x, y, nFlag);
}

void EditorApp::MouseLButtonUp(float x, float y, int nFlag)
{
	if(!m_bInitialised) return;

	m_cGUIManager.MouseLButtonUp(x, y, nFlag);
}

void EditorApp::MouseRButtonDown(float x, float y, int nFlag)
{
	if(!m_bInitialised) return;

	m_cGUIManager.MouseRButtonDown(x, y, nFlag);
}

void EditorApp::MouseRButtonUp(float x, float y, int nFlag)
{
	if(!m_bInitialised) return;

	m_cGUIManager.MouseRButtonUp(x, y, nFlag);
}

void EditorApp::MouseMove(float x, float y, int flag)
{	
	if(!m_bInitialised) return;

	m_cGUIManager.MouseMove(x, y, flag);
}

void EditorApp::MouseWheel(int zDelta, int x, int y, int flag)
{
	if(!m_bInitialised) return;

	m_cGUIManager.MouseWheel(zDelta, x, y, flag);
}


void EditorApp::DragEnter(int x, int y)
{
	m_cGUIManager.DragEnter(x, y);
}

void EditorApp::DragDrop(int x, int y, MDataObject* dropObject)
{
	m_cGUIManager.DragDrop(x, y, dropObject);
}

void EditorApp::DragLeave()
{
	m_cGUIManager.DragLeave();
}

int EditorApp::createNewProject(const char* projectDirectory)
{
	char temp_buffer[1024];
	sprintf(temp_buffer, "%s/Assets", projectDirectory);
	if(_mkdir(temp_buffer)==0)
	{
		sprintf(temp_buffer, "%s/ProjectSettings", projectDirectory);
		if(_mkdir(temp_buffer)==0)
		{
			sprintf(temp_buffer, "%s/MetaData", projectDirectory);
			if(_mkdir(temp_buffer)==0)
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

bool EditorApp::importAssetToMetaData(HWND hWnd, HINSTANCE hInst)
{
	HWND hWndprogress=CreateDialog(hInst, MAKEINTRESOURCE(IDD_ASSET_IMPORT_PROGRESS_DLG), hWnd, reinterpret_cast<DLGPROC>(Proj_AssetImportDlgProc));
	return true;
}

void EditorApp::setProjectHomeDirectory(const char* projectDir)
{
	STRCPY(g_cszProjectHomeDirectory, projectDir);
}

const char* EditorApp::getProjectHomeDirectory()
{
	return g_cszProjectHomeDirectory;
}

gearSceneFileView* EditorApp::getSceneFileView()
{
	return g_pSceneFileViewPtr;
}

gearSceneHierarchy* EditorApp::getSceneHierarchy()
{
	return g_pSceneHierarchyPtr;
}

gearScenePreview* EditorApp::getScenePreview()
{
	return g_pScenePreviewPtr;
}

gearSceneProject* EditorApp::getSceneProject()
{
	return g_pSceneProjectPtr;
}

gearScenePropertyEditor* EditorApp::getScenePropertyEditor()
{
	return g_pScenePropertyEditorPtr;
}

gearSceneWorldEditor* EditorApp::getSceneWorldEditor()
{
	return g_pSceneWorldEditorPtr;
}

gearSceneConsole* EditorApp::getSceneConsole()
{
	return g_pSceneConsolePtr;
}

gePropertyOctree* EditorApp::getPropertyOctree()
{
	return g_pPropertyOctreePtr;
}

void EditorApp::setSceneFileView(gearSceneFileView* ptr)
{
	g_pSceneFileViewPtr=ptr;
}

void EditorApp::setSceneHierarchy(gearSceneHierarchy* ptr)
{
	g_pSceneHierarchyPtr=ptr;
}

void EditorApp::setScenePreview(gearScenePreview* ptr)
{
	g_pScenePreviewPtr=ptr;
}

void EditorApp::setSceneProject(gearSceneProject* ptr)
{
	g_pSceneProjectPtr=ptr;
}

void EditorApp::setScenePropertyEditor(gearScenePropertyEditor* ptr)
{
	g_pScenePropertyEditorPtr=ptr;
}

void EditorApp::setSceneWorldEditor(gearSceneWorldEditor* ptr)
{
	g_pSceneWorldEditorPtr = ptr;
}

void EditorApp::setSceneConsole(gearSceneConsole* ptr)
{
	g_pSceneConsolePtr=ptr;
}

void EditorApp::setPropertyOctree(gePropertyOctree* ptr)
{
	g_pPropertyOctreePtr=ptr;
}

bool EditorApp::KeyDown(int charValue, int flag)
{
	if(!m_bInitialised) return false;

	return m_cGUIManager.KeyDown(charValue, flag);
}

bool EditorApp::KeyUp(int charValue, int flag)
{
	if(!m_bInitialised) return false;

	return m_cGUIManager.KeyUp(charValue, flag);
}

void EditorApp::DoCommand(int cmd)
{
	if(!m_bInitialised) return;

	m_cGUIManager.DoCommand(cmd);
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