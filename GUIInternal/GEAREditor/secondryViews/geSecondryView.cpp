#include "geSecondryView.h"
#include <Windows.h>
#include "../EditorApp.h"

geSecondryView::geSecondryView(const char* name)
{
	strcpy(m_szName, name);
	m_pSecondryRenderer=NULL;
	m_pRootLayout=NULL;
}

geSecondryView::~geSecondryView()
{
	GE_DELETE(m_pRootLayout);
	GE_DELETE(m_pSecondryRenderer);
	//restore gl context to the EditorApp
	EditorApp::getMainRenderer()->makeCurrent();
	//
}

void geSecondryView::createRenderer(HWND hwnd)
{
	m_pSecondryRenderer = new rendererGL10(hwnd);
	m_pSecondryRenderer->setupRenderer(EditorApp::getMainRenderer());

	m_pRootLayout = new geLayout();
	m_pRootLayout->create(m_pSecondryRenderer, NULL, 0, 0, 1, 1);
	m_pRootLayout->setLayoutDirection(geLayout::LAYOUT_PARENT);

	m_cPrevScale.set(1.0f, 1.0f);
	onCreate();
}

void geSecondryView::showView(HWND parentHWND)
{
	static TCHAR szAppName2[] = TEXT ("childWnd");

    HWND child = CreateWindow(szAppName2,       
                        TEXT(m_szName),
                        WS_CHILD | WS_VISIBLE | WS_CAPTION
                        | WS_OVERLAPPEDWINDOW | WS_POPUP,
                        //| ~WS_MINIMIZEBOX | ~WS_MAXIMIZEBOX, 
						//| WS_POPUP,
                        CW_USEDEFAULT,     
                        CW_USEDEFAULT,     
                        300,       
                        300,       
                        parentHWND,              
                        NULL,              
                        GetModuleHandle(NULL),       
                        this);
 
	DWORD err=GetLastError();

    ShowWindow(child, SW_NORMAL);
    UpdateWindow(child);

	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GUIINTERNAL));
	while (GetMessage (&msg, NULL, 0, 0))
	{

		if (!TranslateAccelerator (child, hAccelTable, &msg))
		{
			TranslateMessage (&msg) ;
			DispatchMessage  (&msg) ;
		}
	}
}

void geSecondryView::drawView()
{
	m_pRootLayout->draw();
	onDraw();
}

void geSecondryView::sizeView(float cx, float cy)
{
	if(cx<=1.0f)
		cx=1.0f;
	if(cy<=1.0f)
		cy=1.0f;

	if(m_pRootLayout)
	{
		m_pRootLayout->resize(cx/m_cPrevScale.x, cy/m_cPrevScale.y);
	}

	onSize(cx, cy);

	m_cPrevScale.set(cx, cy);
}

void geSecondryView::destroyView()
{
	onDestroy();
}

void geSecondryView::onCreate()
{
}

void geSecondryView::onSize(float cx, float cy)
{
}

void geSecondryView::onDraw()
{
}

void geSecondryView::onDestroy()
{
}

LRESULT CALLBACK geSecondryView::SecondryView_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:
		{
			geSecondryView* viewPtr = (geSecondryView*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtr(hWndDlg, GWLP_USERDATA, (LONG)viewPtr);
			viewPtr->createRenderer(hWndDlg);
			glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
			return 0;
		}
		break;

	case WM_SIZE:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
			viewPtr->getRenderer()->setViewPort(LOWORD(lParam), HIWORD(lParam));
			viewPtr->sizeView(LOWORD(lParam), HIWORD(lParam));
			//UpdateWindow(hWndDlg);
			return 0;
		}
		break;

	case WM_PAINT:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewPtr->drawView();
			viewPtr->getRenderer()->swapGLBuffer();
			return 0;
		}
		break;

	case WM_CLOSE:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLong(hWndDlg, GWLP_USERDATA);
			viewPtr->destroyView();
			return DestroyWindow(hWndDlg);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWndDlg, Msg, wParam, lParam);
	}

	return FALSE;
}
