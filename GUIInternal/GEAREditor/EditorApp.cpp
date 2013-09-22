#include "EditorApp.h"
#include <direct.h>

#include "../mono/src/monoWrapper.h"

#include "scenes\gearSceneProject.h"
#include "scenes\gearSceneHierarchy.h"
#include "scenes\gearScenePropertyEditor.h"
#include "scenes\gearSceneWorldEditor.h"
#include "scenes\gearSceneFileView.h"
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


EditorApp::EditorApp()
{
	m_bInitialised=false;
	rendererGL10::g_pRendererGL10 = NULL;
}

EditorApp::~EditorApp()
{
	GE_DELETE(rendererGL10::g_pRendererGL10);
}

void EditorApp::init(HWND hWnd, HINSTANCE hInst)
{
	g_hWnd=hWnd;
	m_bInitialised=true;
	rendererGL10::g_pRendererGL10 = new rendererGL10(hWnd);

	rendererGL10::g_pRendererGL10->setupRenderer();
	rendererGL10::g_pRendererGL10->loadDefaultRenderState();

	m_cGUIManager.init();

	//import all assets to the metadata folder
	importAssetToMetaData(hWnd, hInst);

	gearSceneWorldEditor* worldEditorWnd = new gearSceneWorldEditor();
	worldEditorWnd->create(NULL, 0, 0, 300, 200, true);
	m_cGUIManager.appendWindow(worldEditorWnd);
	geLayout* worldEditorLayout=m_cGUIManager.getLayoutManager()->getRootLayout()->createAsParent(worldEditorWnd);
	setSceneWorldEditor(worldEditorWnd);

	gearSceneProject* projectWindow = new gearSceneProject();
	projectWindow->create(NULL, 0, 0, 300, 200, true);
	m_cGUIManager.appendWindow(projectWindow);
	geLayout* projectLayout=worldEditorLayout->createRight(projectWindow, 0.5f);
	setSceneProject(projectWindow);

	gearScenePropertyEditor* propertyEditorWnd = new gearScenePropertyEditor();
	propertyEditorWnd->create(NULL, 0, 300, 200, 250);
	m_cGUIManager.appendWindow(propertyEditorWnd);
	geLayout* propertyEditorLayout=projectLayout->createRight(propertyEditorWnd);	
	setScenePropertyEditor(propertyEditorWnd);

	gearSceneHierarchy* hierarchyWnd = new gearSceneHierarchy();
	hierarchyWnd->create(NULL, 0, 0, 400, 250, true);
	m_cGUIManager.appendWindow(hierarchyWnd);
	geLayout* hierarchyLayout=projectLayout->createTop(hierarchyWnd, 0.45f);
	setSceneHierarchy(hierarchyWnd);

	gearSceneFileView* fileViewWnd = new gearSceneFileView();
	fileViewWnd->create(NULL, 0, 0, 400, 250, true);
	m_cGUIManager.appendWindow(fileViewWnd);
	geLayout* fileViewLayout=projectLayout->createRight(fileViewWnd, 0.55f);
	setSceneFileView(fileViewWnd);

	gearScenePreview* previewWnd = new gearScenePreview();
	previewWnd->create(NULL, 0, 0, 400, 250);
	m_cGUIManager.appendWindow(previewWnd);
	geLayout* previewLayout=propertyEditorLayout->createBottom(previewWnd, 0.35f);
	setScenePreview(previewWnd);
}

void EditorApp::size(int cx, int cy)
{
	if(!m_bInitialised) return;

	if(rendererGL10::g_pRendererGL10)
	{
		rendererGL10::g_pRendererGL10->setViewPort(cx, cy);
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

	//draw here
	m_cGUIManager.draw();

	////glDisable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat lightpos[] = {10, 10, 10, 1.0f};
	//glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	//
	//float colorWhite[]  = {1.0f,1.0f,1.0f,1.0f};
	//float colorGray[]   = {0.3f,0.3f,0.3f,1.0f};
	//glLightfv(GL_LIGHT0, GL_AMBIENT, colorGray );
 //   glLightfv(GL_LIGHT0, GL_DIFFUSE, colorGray );
 //   glLightfv(GL_LIGHT0, GL_SPECULAR, colorGray );

	////glDisable(GL_DEPTH_TEST);
	//glViewport(0, rendererGL10::g_pRendererGL10->getViewPortSz().y-200, 200, 200);	
	////glViewport(0, 0, 200, 200);	
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	////gluOrtho2D(0.0f, 200, 200, 0.0f);
	////
	//gluPerspective(45, 1.0f, 1.0f, 1000.0f);

	////glFrustum(0, 200, 200, 0, -100, 1000);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(0, 0, -10);
	//glRotatef(-90, 1.0f, 0.0f, 0.0f);
	//glRotatef(-45, 0.0f, 0.0f, 1.0f);


	//glColor4f(1,0,0,1);
	//glPushMatrix();
	////glRotatef(10, 1.0f, 0.0f, 0.0f);
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	//engine_render();
	//
	//glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//drawGizmo(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);

	//glPopMatrix();

	//glDisable(GL_LIGHTING);
	////glDisable(GL_DEPTH_TEST);
	//drawGizmo(3.0f);
	////glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);

	//angle+=3.0f;
	//if(angle>360.0f)
	//	angle=0.0f;

	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);

	rendererGL10::g_pRendererGL10->swapGLBuffer();
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

LRESULT CALLBACK	Proj_AssetImportDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
bool EditorApp::importAssetToMetaData(HWND hWnd, HINSTANCE hInst)
{
	HWND hWndprogress=CreateDialog(hInst, MAKEINTRESOURCE(IDD_ASSET_IMPORT_PROGRESS_DLG), hWnd, reinterpret_cast<DLGPROC>(Proj_AssetImportDlgProc));
	//ShowWindow(hWndprogress, SW_SHOW);
	//AssetImporter assetImporter;
	//assetImporter.importAssets(EditorApp::getProjectHomeDirectory(), hWndprogress, IDC_ASSET_PROGRESS_BAR, IDC_ASSET_FILE_STATIC);

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
