#include "geSecondryView.h"
#include <Windows.h>
#include "../EditorApp.h"

geSecondryView::geSecondryView(const char* name)
{
	strcpy(m_szName, name);
	m_pSecondryRenderer=NULL;
}

geSecondryView::~geSecondryView()
{
	GE_DELETE(m_pSecondryRenderer);
	//restore gl context to the EditorApp
	EditorApp::getMainRenderer()->makeCurrent();
	//
}

void geSecondryView::createRenderer(HWND hwnd)
{
	m_pSecondryRenderer = new rendererGL10(hwnd);
	m_pSecondryRenderer->setupRenderer(EditorApp::getMainRenderer());

	m_cLayoutManager.create(m_pSecondryRenderer, 0, 0, 1184, 567);
	//m_pRootLayout = new geLayout();
	//m_pRootLayout->create(m_pSecondryRenderer, NULL, 0, 0, m_cSize.x, m_cSize.y);
	//m_pRootLayout->setLayoutDirection(geLayout::LAYOUT_PARENT);

	m_cPrevScale.set(m_cSize.x, m_cSize.y);
	onCreate();
}

void geSecondryView::setSize(geVector2f& sz)
{
	m_cSize=sz;
}

void geSecondryView::setPos(geVector2f& pos)
{
	m_cPos=pos;
}

void geSecondryView::showView(HWND parentHWND)
{
	static TCHAR szAppName2[] = TEXT ("childWnd");
	DWORD dwStyle = WS_CHILD | WS_OVERLAPPEDWINDOW | WS_POPUPWINDOW;
	DWORD dwExStyle = WS_EX_TOOLWINDOW;
	//dwStyle&=~WS_MAXIMIZEBOX;
	//dwStyle&=~WS_MINIMIZEBOX;

	HWND child = CreateWindowEx(
						dwExStyle,
						szAppName2,       
                        TEXT(m_szName),
                        dwStyle,
						(int)m_cPos.x,       
                        (int)m_cPos.y, 
                        (int)m_cSize.x,       
                        (int)m_cSize.y,       
                        parentHWND,              
                        NULL,              
                        GetModuleHandle(NULL),       
                        this);
 
	DWORD err=GetLastError();

    ShowWindow(child, SW_NORMAL);
    UpdateWindow(child);

	//MSG msg;
	//HACCEL hAccelTable = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GUIINTERNAL));
	//while (GetMessage (&msg, NULL, 0, 0))
	//{

	//	if (!TranslateAccelerator (child, hAccelTable, &msg))
	//	{
	//		TranslateMessage (&msg) ;
	//		DispatchMessage  (&msg) ;
	//	}
	//}
}

void geSecondryView::drawView()
{
	m_cLayoutManager.draw();
	onDraw();
}

void geSecondryView::sizeView(float cx, float cy)
{
	if(cx<=1.0f)
		cx=1.0f;
	if(cy<=1.0f)
		cy=1.0f;

	m_cLayoutManager.setSize(cx, cy);

	setSize(geVector2f(cx, cy));
	onSize(cx, cy);

	m_cPrevScale.set(cx, cy);
}

void geSecondryView::destroyView()
{
	onDestroy();
	delete this;
}

bool geSecondryView::mouseLButtonDown(float x, float y, int nFlag)
{
	return onMouseLButtonDown(x, y, nFlag);
}

void geSecondryView::mouseLButtonUp(float x, float y, int nFlag)
{
	onMouseLButtonUp(x, y, nFlag);
}

bool geSecondryView::mouseRButtonDown(float x, float y, int nFlag)
{
	return onMouseRButtonDown(x, y, nFlag);
}

void geSecondryView::mouseRButtonUp(float x, float y, int nFlag)
{
	onMouseRButtonUp(x, y, nFlag);
}

bool geSecondryView::mouseMove(float x, float y, int flag)
{
	return onMouseMove(x, y, flag);
}

void geSecondryView::mouseWheel(int zDelta, int x, int y, int flag)
{
	onMouseWheel(zDelta, x, y, flag);
}

bool geSecondryView::onMouseLButtonDown(float x, float y, int nFlag)
{
	return m_cLayoutManager.MouseLButtonDown(x, y, nFlag);
}
void geSecondryView::onMouseLButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseLButtonUp(x, y, nFlag);
}
bool geSecondryView::onMouseRButtonDown(float x, float y, int nFlag)
{
	return m_cLayoutManager.MouseRButtonDown(x, y, nFlag);
}
void geSecondryView::onMouseRButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseRButtonUp(x, y, nFlag);
}
bool geSecondryView::onMouseMove(float x, float y, int flag)
{
	return m_cLayoutManager.MouseMove(x, y, flag);
}
void geSecondryView::onMouseWheel(int zDelta, int x, int y, int flag)
{
	m_cLayoutManager.MouseWheel(zDelta, x, y, flag);
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
			viewPtr->getRenderer()->makeCurrent();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			viewPtr->drawView();
			viewPtr->getRenderer()->swapGLBuffer();
			EditorApp::getMainRenderer()->makeCurrent();
			UpdateWindow(GetParent(hWndDlg));
			return 0;
		}
		break;

	case WM_MOUSEMOVE:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			viewPtr->mouseMove(xx, yy, nFlags);
		}
		break;

	case WM_MOUSEWHEEL:
		{
			//int nFlags=LOWORD(wParam);
			//int xx=LOWORD(lParam);
			//int yy=HIWORD(lParam);
			//xx = GET_X_LPARAM(lParam); 
			//yy = GET_Y_LPARAM(lParam); 
			//int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			//m_cEditorApp.MouseWheel(zDelta, xx, yy, nFlags);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
			geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			viewPtr->mouseLButtonDown(xx, yy, nFlags);
		}
		break;
	case WM_LBUTTONUP:
		{
			geSecondryView* viewPtr = (geSecondryView*)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
			int nFlags=LOWORD(wParam);
			int xx=LOWORD(lParam);
			int yy=HIWORD(lParam);
			viewPtr->mouseLButtonUp(xx, yy, nFlags);
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
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWndDlg, Msg, wParam, lParam);
	}

	return FALSE;
}
