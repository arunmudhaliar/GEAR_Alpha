#pragma comment(lib,"GEAREngine.lib")

#include "GUIInternal.h"
#include "../macosProj/GEARInternal/GEARInternal/appEntry.h"

#include "stdafx.h"
#include <WindowsX.h>
#include <ShlObj.h>

#ifdef _DEBUG
#define ENABLE_MEMORY_CHECK
#include <crtdbg.h>
#endif

LRESULT CALLBACK projectSelector_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
char* browseFolder(HWND hWndParent, const char* title, const char* root_dir=NULL);
LPITEMIDLIST convertPathToLpItemIdList(const char *pszPath);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
#if defined(_DEBUG) && defined(ENABLE_MEMORY_CHECK)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	//SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif

	int return_val = 0;
	if (DialogBox(hInstance, MAKEINTRESOURCE(IDD_PROJ_DLG), NULL, reinterpret_cast<DLGPROC>(projectSelector_DlgProc)) != 0)
	{
		monoWrapper::initDebugConsole();
		return_val = appEntry();
		monoWrapper::destroyDebugConsole();
	}

#if defined(_DEBUG) && defined(ENABLE_MEMORY_CHECK)
	SymCleanup(GetCurrentProcess());
#endif

	return return_val;
}

LRESULT CALLBACK projectSelector_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
	{
		//read the recent dialog
		FILE* fp = fopen("recentProjects", "r");
		if(fp)
		{
			char temp_buffer[1024];
			while (fscanf(fp, "%s\n", temp_buffer) != EOF)
			{
				SendDlgItemMessage(hWndDlg, IDC_PROJ_DLG_RECENT_PROJECT_LIST, LB_ADDSTRING, NULL, (LPARAM)temp_buffer);
			}
			fclose(fp);
		}

		return TRUE;
	}
	break;

	case WM_COMMAND:
		switch(wParam)
		{
		case ID_PROJ_DLG_OPEN_NEW_PROJECT:
		{
			char* project_directory=browseFolder(hWndDlg, _T("Select an empty folder to create project."));
			if(EditorGEARApp::createNewProject(project_directory)!=0)
			{
				MessageBox(hWndDlg, "Project creation failed", "Error.", MB_OK | MB_ICONERROR);
				GE_DELETE_ARY(project_directory);
				return true;
			}
			EditorGEARApp::setProjectHomeDirectory(project_directory);
			GE_DELETE_ARY(project_directory);
			EndDialog(hWndDlg, 1);
			return true;
		}
		break;

		case ID_PROJ_DLG_OPEN_RECENT_PROJECT:
		{
			// Get current selection index in listbox
			int itemIndex = (int) SendDlgItemMessage(hWndDlg, IDC_PROJ_DLG_RECENT_PROJECT_LIST, LB_GETCURSEL, (WPARAM)0, (LPARAM) 0);
			if (itemIndex == LB_ERR)
			{
				MessageBox(hWndDlg, "Select atleast one recent project", "GEAR.", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				// Get actual text in buffer
				char temp_buffer[1024];
				SendDlgItemMessage(hWndDlg, IDC_PROJ_DLG_RECENT_PROJECT_LIST, LB_GETTEXT, (WPARAM) itemIndex, (LPARAM) temp_buffer );
				EditorGEARApp::setProjectHomeDirectory(temp_buffer);
				EndDialog(hWndDlg, 2);
			}
			return true;
		}
		break;
		}
		break;

	case WM_CLOSE:
	{
		return EndDialog(hWndDlg, 0);
	}
	break;
	//NOTE: Commented this default block, since the SHBrowseForFolder [New project option] wont work
	//default:
	//	return DefWindowProc(hWndDlg, Msg, wParam, lParam);
	//
	}

	return FALSE;
}

char* browseFolder(HWND hWndParent, const char* title, const char* root_dir)
{
	char* return_buffer=NULL;
	BROWSEINFO bi = { 0 };
	bi.hwndOwner=hWndParent;
	bi.lpszTitle = title;
	if(root_dir)
	{
		bi.pidlRoot = convertPathToLpItemIdList(root_dir);
	}

	bi.ulFlags=BIF_USENEWUI;// | BIF_RETURNONLYFSDIRS;

	HRESULT r=OleInitialize(NULL);
	LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
	if ( pidl != 0 )
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if ( SHGetPathFromIDList ( pidl, path ) )
		{
			_tprintf ( _T("Selected Folder: %s\n"), path );
			return_buffer = new char[strlen(path)+1];
			strcpy(return_buffer, path);
			return_buffer[strlen(path)]='\0';
		}

		// free memory used
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( pidl );
			imalloc->Release ( );
		}
	}

	OleUninitialize();

	if(return_buffer)
	{
		//win32 to unix style path
		geUtil::convertPathToUnixFormat(return_buffer);
	}

	return return_buffer;
}

LPITEMIDLIST convertPathToLpItemIdList(const char *pszPath)
{
	LPITEMIDLIST  pidl = NULL;
	LPSHELLFOLDER pDesktopFolder = NULL;
	OLECHAR       olePath[MAX_PATH];
	ULONG         chEaten;
	HRESULT       hr;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszPath, -1, 
			olePath, MAX_PATH);
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, 
			olePath, &chEaten, &pidl, NULL);
		pDesktopFolder->Release();
		return pidl;
	}
	return NULL;
}



#if DEPRECATED
#include "MDragGropInterface.h"
#include "GEAREditor\win32\MDropSource.h"
#include "GEAREditor\win32\eventHook.h"

#include "GEAREditor\secondryViews\geColorDlg.h"
#include <direct.h>
#include <Commdlg.h>

#ifdef _DEBUG
#define ENABLE_MEMORY_CHECK
#include <crtdbg.h>
#endif

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

EditorApp m_cEditorApp;
//#if DEPRECATED
MDragDropInterface* m_cDropTargetInterfacePtr=NULL;	//must not delete this pointer
//#endif
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Proj_DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

char* browseFolder(HWND hWndParent, const char* title, const char* root_dir=NULL);
LPITEMIDLIST convertPathToLpItemIdList(const char *pszPath);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

#if defined(_DEBUG) && defined(ENABLE_MEMORY_CHECK)
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);

	//SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GUIINTERNAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, SW_SHOWMAXIMIZED/*nCmdShow*/))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIINTERNAL));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIINTERNAL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GUIINTERNAL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	static TCHAR szAppName2[] = TEXT ("childWnd");
	WNDCLASSEX childWndClass;
	childWndClass.cbSize = sizeof(WNDCLASSEX);
 
    childWndClass.style				= CS_HREDRAW | CS_VREDRAW;
    childWndClass.lpfnWndProc		= geSecondryView::SecondryView_DlgProc;
    childWndClass.cbClsExtra		= 0;
    childWndClass.cbWndExtra		= 0;
    childWndClass.hInstance			= hInstance;
    childWndClass.hIcon				= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIINTERNAL));
    childWndClass.hCursor			= LoadCursor (NULL, IDC_ARROW);
    childWndClass.hbrBackground		= (HBRUSH) GetStockObject (WHITE_BRUSH);
    childWndClass.lpszMenuName		= NULL;
    childWndClass.lpszClassName		= szAppName2;
	childWndClass.hIconSm			= LoadIcon(childWndClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassEx(&childWndClass))
    {
        MessageBox (NULL, TEXT ("This program requires Windows 95/98/NT"),
                    szAppName2, MB_ICONERROR);
		return NULL;
    }

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message)
	{
	case WM_CREATE:
		{
			char current_working_directory[1024];
			GetCurrentDirectory(sizeof(current_working_directory), current_working_directory);
			geUtil::convertPathToUnixFormat(current_working_directory);

			EditorApp::setAppDirectory(current_working_directory);

			monoWrapper::initDebugConsole();
			monoWrapper::loadMonoModules();

			m_cDropTargetInterfacePtr=NULL;
			if(DialogBox(hInst, MAKEINTRESOURCE(IDD_PROJ_DLG), hWnd, reinterpret_cast<DLGPROC>(Proj_DlgProc))==0)
			{
				PostQuitMessage(0);
			}
			else
			{
				monoWrapper::reInitMono(EditorApp::getProjectHomeDirectory());
				monoWrapper::mono_engine_test_function_for_mono();

				m_cEditorApp.init(hWnd, hInst);
				OleInitialize(NULL);

				MDragDropInterface* dropTarget = new MDragDropInterface(&m_cEditorApp);
				// acquire a strong lock
				CoLockObjectExternal(dropTarget, TRUE, FALSE);
				RegisterDragDrop(hWnd, dropTarget);
				m_cDropTargetInterfacePtr=dropTarget;

				Timer::init();
				//eventHook::g_pEditorAppPtr=&m_cEditorApp;
				//eventHook::hookEvent(hWnd);

				//load the current scene
				std::string root_dir = EditorApp::getProjectHomeDirectory();
				root_dir+="/ProjectSettings/currentscene";
				gxFile currenSceneFile;
				if(currenSceneFile.OpenFile(root_dir.c_str()))
				{
					const char* relativepath = currenSceneFile.ReadString();
					currenSceneFile.CloseFile();

					root_dir = EditorApp::getProjectHomeDirectory();
					root_dir+="/Assets";
					root_dir+=relativepath;
					if(EditorApp::getSceneHierarchy()->loadScene(root_dir.c_str()))
					{
						std::string wndTitle ="GEAR Alpha [";
						wndTitle+=relativepath;
						wndTitle+=+"]";
						SetWindowText(hWnd, wndTitle.c_str());
						EditorApp::getSceneProject()->populateProjectView();
						EditorApp::getSceneFileView()->populateFileView();
					}
					GX_DELETE_ARY(relativepath);
				}
				//
			}
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		m_cEditorApp.DoCommand(wmId);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_TEST_COMMAND:
			{
				gxFile file;
				file.OpenFile("01_snow_planet.scene.xml.bin");
				int nActor=0;
				file.Read(nActor);
				geGUIBase* tvnode=EditorApp::getSceneHierarchy()->getSelectedTreeNode();
				if(!tvnode)
					break;
				object3d* selectedobj=(object3d*)tvnode->getUserData();
				if(!selectedobj)
					break;

				for(int x=0;x<nActor;x++)
				{
					char* actorname=file.ReadString();
					bool bPrefab=false;
					file.Read(bPrefab);
					char* prefabname=NULL;
					if(bPrefab)
						prefabname=file.ReadString();

					float rotation[3];
					float translation[3];

					file.Read(rotation[0]);
					file.Read(rotation[1]);
					file.Read(rotation[2]);

					file.Read(translation[0]);
					file.Read(translation[1]);
					file.Read(translation[2]);

					if(prefabname)
					{
						std::vector<object3d*>* list=selectedobj->getChildList();
						for(std::vector<object3d*>::iterator it = list->begin(); it != list->end(); ++it)
						{
							object3d* childobj = *it;

							if(strcmp(prefabname, childobj->getName())==0)
							{
								//apply transforms
								childobj->rotateLocalXf(rotation[0]);
								childobj->rotateLocalXf(rotation[1]);
								childobj->rotateLocalXf(rotation[2]);
								childobj->updatePositionf(translation[0], translation[2], translation[1]);

								break;
							}
						}
					}
					GX_DELETE_ARY(actorname);
					GX_DELETE_ARY(prefabname);
				}

				file.CloseFile();
			}
			break;
		case ID_PROJECT_BUILDFORANDROID:
			{
				printf("\n================ANDROID BUILD ENGINE===============\n");
				char inputbuffer[1024*6];

				sprintf(inputbuffer, "%s//AndroidProjectMaker.exe %s %s", EditorApp::getAppDirectory().c_str(), EditorApp::getAppDirectory().c_str(), EditorApp::getProjectHomeDirectory());
				if(monoWrapper::exec_cmd(inputbuffer)!=0)
				{
					printf("\nERROR\n");
				}
				printf("\n======================================================\n");
			}
			break;
		case ID_EDIT_ACTIVECAMERAPROPERTY:
			{
				object3d* cam=monoWrapper::mono_engine_getWorld(0)->getActiveCamera();
				EditorApp::getSceneWorldEditor()->selectedObject3D(cam);
				EditorApp::getScenePropertyEditor()->populatePropertyOfObject(cam);
			}
			break;
		case ID_EDIT_OCTREEPROPERTY:
			{
				EditorApp::getScenePropertyEditor()->populatePropertyOfOctree();
			}
			break;
		case ID_EDIT_LAYERSPROPERTY:
			{
				EditorApp::getScenePropertyEditor()->populatePropertyOfLayers();
			}
			break;
		case ID_EDIT_FOGSETTINGS:
			{
				EditorApp::getScenePropertyEditor()->populateSettingsOfFog();
			}
			break;
		case ID_FILE_SAVESCENE:
			{
				std::string root_dir = EditorApp::getProjectHomeDirectory();
				root_dir+="/Assets";
				std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
				char output_buffer[MAX_PATH];
				if(EditorApp::showSaveCommonDlg(hWnd, output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
				{
					if(EditorApp::getSceneHierarchy()->saveCurrentScene(output_buffer))
					{
						EditorApp::getSceneProject()->populateProjectView();
						EditorApp::getSceneFileView()->populateFileView();

						//update the currentscene file
						root_dir = EditorApp::getProjectHomeDirectory();
						root_dir+="/ProjectSettings/currentscene";
						gxFile currenSceneFile;
						if(currenSceneFile.OpenFile(root_dir.c_str(), gxFile::FILE_w))
						{
							const char* relativepath=AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(output_buffer);
							char unix_path[MAX_PATH];
							memset(unix_path, 0, MAX_PATH);
							strcpy(unix_path, relativepath);
							geUtil::convertPathToUnixFormat(unix_path);
							currenSceneFile.Write(unix_path);
							currenSceneFile.CloseFile();

							std::string wndTitle ="GEAR Alpha [";
							wndTitle+=relativepath;
							wndTitle+=+"]";
							SetWindowText(hWnd, wndTitle.c_str());
						}
						//
					}
				}
			}
			break;
		case ID_FILE_OPENSCENE:
			{
				std::string root_dir = EditorApp::getProjectHomeDirectory();
				root_dir+="/Assets";
				std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
				char output_buffer[MAX_PATH];
				if(EditorApp::showOpenCommonDlg(hWnd, output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
				{
					if(EditorApp::getSceneHierarchy()->loadScene(output_buffer))
					{
						EditorApp::getSceneProject()->populateProjectView();
						EditorApp::getSceneFileView()->populateFileView();

						//update the currentscene file
						root_dir = EditorApp::getProjectHomeDirectory();
						root_dir+="/ProjectSettings/currentscene";
						gxFile currenSceneFile;
						if(currenSceneFile.OpenFile(root_dir.c_str(), gxFile::FILE_w))
						{
							const char* relativepath=AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(output_buffer);
							char unix_path[MAX_PATH];
							memset(unix_path, 0, MAX_PATH);
							strcpy(unix_path, relativepath);
							geUtil::convertPathToUnixFormat(unix_path);
							currenSceneFile.Write(unix_path);
							currenSceneFile.CloseFile();

							std::string wndTitle ="GEAR Alpha [";
							wndTitle+=relativepath;
							wndTitle+=+"]";
							SetWindowText(hWnd, wndTitle.c_str());
						}
						//
					}
				}
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			Timer::update();

			m_cEditorApp.update(Timer::getDtinSec());
			m_cEditorApp.draw();
		}
		break;
	case WM_SIZE:
		{
			if((LOWORD(wParam)==SIZE_MAXIMIZED || LOWORD(wParam)==SIZE_RESTORED) && m_cEditorApp.isInitialized())
			{
				monoWrapper::reInitMono(EditorApp::getProjectHomeDirectory());
				m_cEditorApp.importAssetToMetaData(hWnd, hInst);
				EditorApp::getSceneProject()->populateProjectView();
				EditorApp::getSceneFileView()->populateFileView();
			}
			m_cEditorApp.size(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case WM_MOUSEMOVE:
		{
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			m_cEditorApp.MouseMove(xx, yy, nFlags);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			xx = GET_X_LPARAM(lParam); 
			yy = GET_Y_LPARAM(lParam); 
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			m_cEditorApp.MouseWheel(zDelta, xx, yy, nFlags);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			//if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
			//	geTextBox::g_pCurrentlyActiveTextBoxPtr->clearSelection();
			geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			m_cEditorApp.MouseLButtonDown(xx, yy, nFlags);
		}
		break;
	case WM_LBUTTONUP:
		{
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			m_cEditorApp.MouseLButtonUp(xx, yy, nFlags);
		}
		break;
	case WM_DESTROY:
		{
			if(m_cDropTargetInterfacePtr)
			{
				RevokeDragDrop(hWnd);

				// remove the strong lock
				CoLockObjectExternal(m_cDropTargetInterfacePtr, FALSE, TRUE);
				// release our own reference
				m_cDropTargetInterfacePtr->Release();

				OleUninitialize();
			}

			monoWrapper::destroyMono();
			monoWrapper::destroyDebugConsole();

#if defined(_DEBUG) && defined(ENABLE_MEMORY_CHECK)
			SymCleanup(GetCurrentProcess());
#endif
			PostQuitMessage(0);
		}
		break;

	case WM_KEYDOWN:
		{
			if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
			{
				if(wParam==VK_SHIFT)
					break;
				short shift_keystate=GetKeyState(VK_SHIFT);
				char ch=MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
				if(ch>=0x41 && ch<=0x5A)
				{
					if(!(shift_keystate&0x8000))
						ch=ch+32;
				}
				else if(ch>=0x31 && ch<=0x39)
				{
					if((shift_keystate&0x8000))
						ch=ch-16;
				}

				bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyDown(ch, lParam);
			}
			else
			{
				m_cEditorApp.KeyDown(wParam, lParam);
			}
		}
		break;
	case WM_KEYUP:
		{
			if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
			{
				if(wParam==VK_SHIFT)
					break;
				short shift_keystate=GetKeyState(VK_SHIFT);
				char ch=MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
				if(ch>=0x41 && ch<=0x5A)
				{
					if(!(shift_keystate&0x8000))
						ch=ch+32;
				}
				else if(ch>=0x31 && ch<=0x39)
				{
					if((shift_keystate&0x8000))
						ch=ch-16;
				}
				geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyUp(ch, lParam);
			}
			else
			{
				m_cEditorApp.KeyUp(wParam, lParam);
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//WINOLEAPI dllimport DoDragDrop(
//   IDataObject  * pDataObject,   // Pointer to the data object
//   IDropSource  * pDropSource,   // Pointer to the source
//   DWORD          dwOKEffect,    // Effects allowed by the source
//   DWORD        * pdwEffect      // Pointer to effects on the source
//   )
//{
//
//}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		break;
	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return (INT_PTR)FALSE;
}
#endif