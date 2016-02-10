#include "geSecondryView.h"
#include "../EditorApp.h"

geSecondryView::geSecondryView(const char* name, geFontManager* fontmanager, rendererGL10* mainRenderer)
{
    primaryRenderer=mainRenderer;
	strcpy(m_szName, name);
	secondryRenderer=NULL;
    layoutManager = new geLayoutManager(fontmanager);
    this->fontManager = fontmanager;
    sdlSecondryWindow=NULL;
    extraWindowFlags = 0;

#ifdef _WIN32
	//In windows SDL window position (0, 0) starts from client space, so shifting y postion down 100px. 
	m_cPos.set(100, 100);
#endif
}

geSecondryView::~geSecondryView()
{
	GE_DELETE(secondryRenderer);
    
	//restore gl context to the EditorApp
	primaryRenderer->makeCurrent();
	//
    GX_DELETE(layoutManager);
}


void geSecondryView::createRenderer(SDL_Window* window)
{
    secondryRenderer = new rendererGL10(window);
	secondryRenderer->setupRenderer(primaryRenderer);
	layoutManager->create(secondryRenderer, 0, 0, m_cSize.x, m_cSize.y);
	previousScale.set(m_cSize.x, m_cSize.y);
    
	onCreate();
}

void geSecondryView::setSize(geVector2f& sz)
{
	m_cSize=sz;
}

void geSecondryView::setPos(geVector2i& pos)
{
	m_cPos=pos;
}

void geSecondryView::showView(int extraWindowFlags)
{
#if DEPRECATED
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
						m_cPos.x,
                        m_cPos.y, 
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
#else
    
    //createSecondryWindow();
    //SDL_Thread *thread = SDL_CreateThread( secondryThread, "testThread", (void *)this);
    //SDL_DetachThread(thread);
    //int threadReturnValue;
    //SDL_WaitThread(thread, &threadReturnValue);
    extraWindowFlags = extraWindowFlags;
    secondryThread((void *)this);
#endif
}

SDL_Window* geSecondryView::createSecondryWindow()
{
    sdlSecondryWindow = SDL_CreateWindow(
                                         m_szName,             // window title
                                         m_cPos.x,     // x position, centered
                                         m_cPos.y,     // y position, centered
                                         (int)m_cSize.x,                        // width, in pixels
                                         (int)m_cSize.y,                        // height, in pixels
                                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  | extraWindowFlags        // flags
                                         );
    
    return sdlSecondryWindow;
}

int geSecondryView::secondryThread( void *ptr )
{
    geSecondryView* view = (geSecondryView*)ptr;
    
    view->createSecondryWindow();
    
    SDL_ShowWindow(view->getSecondryWindow());
    
    view->createRenderer(view->getSecondryWindow());
    view->getRenderer()->setViewPort(view->m_cSize.x, view->m_cSize.y);
    view->setSize(view->m_cSize);
    
    bool quit=false;
    //While application is running
    while( !quit )
    {
        SDL_Event e;
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if(e.type==SDL_WINDOWEVENT)
            {
                SDL_WindowEvent* windowEvent = (SDL_WindowEvent*)&e;
                if( windowEvent->event == SDL_WINDOWEVENT_CLOSE )
                {
                    quit = true;
                }
                else
                {
                    view->processEvent(view->getSecondryWindow(), e);
                }
            }
            else
            {
                //process other events
                view->processEvent(view->getSecondryWindow(), e);
            }
        }
        
        
        view->getRenderer()->makeCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view->drawView();
        view->getRenderer()->swapGLBuffer();
		EditorApp* editor = EditorApp::g_pEditorAppInstance;
        view->primaryRenderer->makeCurrent();
		editor->draw();
        
        //It is very important in shared contexts to make sure the driver is done with all Objects before signaling other threads that they can use them!
        //GLsync fenceId = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );

        SDL_Delay(rand()%10);
    }
    
    SDL_DestroyWindow(view->getSecondryWindow());
    
    //It is very important in shared contexts to make sure the driver is done with all Objects before signaling other threads that they can use them!
    GLsync fenceId = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
    UNUSED(fenceId);
//    GLenum result;
//    while(true)
//    {
//        glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(5000000000)); //5 Second timeout
//        if(result != GL_TIMEOUT_EXPIRED) break; //we ignore timeouts and wait until all OpenGL commands are processed!
//    }
    
	//SDL_GL_MakeCurrent(NULL, NULL);
    view->destroyView();
    
    return 0;
}

void geSecondryView::processEvent(SDL_Window * window, SDL_Event& e)
{
    if(e.type==SDL_WINDOWEVENT)
    {
        SDL_WindowEvent* windowEvent = (SDL_WindowEvent*)&e;
        
        switch (windowEvent->event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                int window_cx=1;
                int window_cy=1;
                SDL_GetWindowSize(window, &window_cx, &window_cy);
                //editorApp.size(window_cx, window_cy);
                getRenderer()->setViewPort(window_cx, window_cy);
                sizeView(window_cx, window_cy);
            }
                break;
                
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEBUTTONDOWN)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        
        SDL_MouseButtonEvent* mouseBtnEvent = (SDL_MouseButtonEvent*)&e;
        switch (mouseBtnEvent->button) {
            case SDL_BUTTON_LEFT:
            {
                //DEBUG_PRINT("Left Mouse Down");
                geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
                int nFlags=MK_LBUTTON;
                mouseLButtonDown(mouse_x, mouse_y, nFlags);
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
                //DEBUG_PRINT("Middle Mouse Down");
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
                //DEBUG_PRINT("Right Mouse Down");
            }
                break;
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEBUTTONUP)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        
        SDL_MouseButtonEvent* mouseBtnEvent = (SDL_MouseButtonEvent*)&e;
        switch (mouseBtnEvent->button) {
            case SDL_BUTTON_LEFT:
            {
                //DEBUG_PRINT("Left Mouse Up");
                geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
                int nFlags=MK_LBUTTON;
                mouseLButtonUp(mouse_x, mouse_y, nFlags);
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
                //DEBUG_PRINT("Middle Mouse Up");
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
                //DEBUG_PRINT("Right Mouse Up");
            }
                break;
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEMOTION)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        
        //        DEBUG_PRINT("m_x=%d, m_y%d", mouse_x, mouse_y);
        SDL_MouseMotionEvent* mouseMotionEvent = (SDL_MouseMotionEvent*)&e;
        switch (mouseMotionEvent->state) {
            case 1:
                mouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_LBUTTON);
                break;
            case 2:
                mouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_MBUTTON);
                break;
            case 4:
                mouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_RBUTTON);
                break;
                
            default:
                break;
        }
    }
    else if(e.type==SDL_MOUSEWHEEL)
    {
        /*
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        
        int nFlags=0;
        SDL_MouseWheelEvent* mouseWheelEvent = (SDL_MouseWheelEvent*)&e;
        //editorApp.MouseWheel(mouseWheelEvent->y, mouse_x, mouse_y, nFlags);
         */
    }
}

void geSecondryView::drawView()
{
	layoutManager->draw();
	onDraw();
}

void geSecondryView::sizeView(float cx, float cy)
{
	if(cx<=1.0f)
		cx=1.0f;
	if(cy<=1.0f)
		cy=1.0f;

	layoutManager->setSize(cx, cy);

    geVector2f tmp(cx, cy);
	setSize(tmp);
	onSize(cx, cy);

	previousScale.set(cx, cy);
}

void geSecondryView::destroyView()
{
	onDestroy();
    primaryRenderer->makeCurrent();
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
	return layoutManager->MouseLButtonDown(x, y, nFlag);
}
void geSecondryView::onMouseLButtonUp(float x, float y, int nFlag)
{
	layoutManager->MouseLButtonUp(x, y, nFlag);
}
bool geSecondryView::onMouseRButtonDown(float x, float y, int nFlag)
{
	return layoutManager->MouseRButtonDown(x, y, nFlag);
}
void geSecondryView::onMouseRButtonUp(float x, float y, int nFlag)
{
	layoutManager->MouseRButtonUp(x, y, nFlag);
}
bool geSecondryView::onMouseMove(float x, float y, int flag)
{
	return layoutManager->MouseMove(x, y, flag);
}
void geSecondryView::onMouseWheel(int zDelta, int x, int y, int flag)
{
	layoutManager->MouseWheel(zDelta, x, y, flag);
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

#if DEPRECATED
LRESULT CALLBACK geSecondryView::SecondryView_DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CREATE:
		{
			geSecondryView* viewPtr = (geSecondryView*)((CREATESTRUCT*)lParam)->lpCreateParams;
			SetWindowLongPtr(hWndDlg, GWLP_USERDATA, (LONG)viewPtr);
			viewPtr->createRenderer(hWndDlg);
			//glClearColor(0.15f, 0.15f, 0.15f, 1.0f);	//no need for this clear color here
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
			EditorGEARApp::getMainRenderer()->makeCurrent();
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
#endif
