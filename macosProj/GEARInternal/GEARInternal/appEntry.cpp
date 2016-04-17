#include "appEntry.h"
#include "windowMessagePump.h"
#include <fstream>

bool g_mControlKeyPressed = false;
bool g_mAppInitialised = false;
bool g_mReInitMono = false;
//int handleAppEvents(void *userdata, SDL_Event *event);

int appEntry()
{
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        printf ("GetCurrentDir error %d", errno);
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    printf ("Info: Current working directory - %s\n", cCurrentPath);


    EditorGEARApp editorApp;
    EditorGEARApp::setAppDirectory(cCurrentPath);


    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init" << std::endl;
        return -1;
    }

//    SDL_SetEventFilter(handleAppEvents, NULL);

    // create a window
    SDL_Window * window = SDL_CreateWindow(
                                           "GEAR 3D",             // window title
                                           SDL_WINDOWPOS_CENTERED,     // x position, centered
                                           SDL_WINDOWPOS_CENTERED,     // y position, centered
                                           640,                        // width, in pixels
                                           480,                        // height, in pixels
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  | SDL_WINDOW_RESIZABLE  |  SDL_WINDOW_ALLOW_HIGHDPI    // flags
                                           );

#ifdef GEAR_WINDOWS
	// Load our menu definitions
	HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDC_GUIINTERNAL));
	// Attach the menu to the window
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if(SDL_GetWindowWMInfo(window, &info))
	{
		SetMenu(info.info.win.window, hMenu);
	}

	// This tells SDL we want to process system events
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
#endif
	
    SDL_MaximizeWindow(window);

    int window_cx=1;
    int window_cy=1;
    SDL_GetWindowSize(window, &window_cx, &window_cy);

    if( window == NULL )
    {
        std::cout << "ERROR SDL_CreateWindow" << std::endl;
        return -1;
    }

    //SDL_EventState(SDL_DROPFILE, SDL_ENABLE); //NOT YET IMPLEMENTED

    //shared context
    //http://forums.libsdl.org/viewtopic.php?t=9036&sid=3374c819e18df779e17b4ce5a49fdd15
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

//    //Create context
//    SDL_GLContext context = SDL_GL_CreateContext( window );
//    if( context == NULL )
//    {
//        std::cout << "ERROR SDL_GL_CreateContext" << std::endl;
//        SDL_DestroyWindow(window);
//        SDL_Quit();
//        return -1;
//    }

    //SDL_GL_MakeCurrent(window, context);

    std::string gear_mono_install_path="";
    std::ifstream file(".//monoinstallpath.txt");
    if(file.is_open() && std::getline(file, gear_mono_install_path))
    {
        printf("GEAR MONO INSTALATION PATH %s\n", gear_mono_install_path.c_str());
    }
    file.close();
    
#ifdef GEAR_WINDOWS
    monoWrapper::loadMonoModules(gear_mono_install_path);
#elif defined(GEAR_APPLE)
    monoWrapper::loadMonoModules(gear_mono_install_path);
#elif defined(GEAR_ANDROID)
    monoWrapper::loadMonoModules("/storage/emulated/0/gear/", "/storage/emulated/0/gear/");
#else
    #error Unknown Platform
#endif
    
    monoWrapper::reInitMono(EditorGEARApp::getProjectHomeDirectory());
    monoWrapper::mono_engine_test_function_for_mono();
    Timer::init();

    editorApp.init(window);
    editorApp.size(window_cx, window_cy);

    //load the current scene
    std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    root_dir+="/ProjectSettings/currentscene";
    gxFile currenSceneFile;
    if(currenSceneFile.OpenFile(root_dir.c_str()))
    {
        const char* relativepath = currenSceneFile.ReadString();
        currenSceneFile.CloseFile();

        root_dir = EditorGEARApp::getProjectHomeDirectory();
        root_dir+="/Assets";
        root_dir+=relativepath;
        if(EditorGEARApp::getSceneHierarchy()->loadScene(root_dir.c_str()))
        {
            std::string wndTitle ="GEAR Alpha [";
            wndTitle+=relativepath;
            wndTitle+=+"]";
            SDL_SetWindowTitle(window, wndTitle.c_str());
            EditorGEARApp::getSceneProject()->populateProjectView();
            EditorGEARApp::getSceneFileView()->populateFileView();
        }
        GX_DELETE_ARY(relativepath);
    }
    //
    
    std::function<void(SDL_Window*, SDL_Event&, void* userdata)> func = processEvent;
    windowMessagePump::getInstance().appendMessagePump(window, nullptr, func);
    
    
    
    bool quit=false;
    //While application is running
    while( !quit )
    {
        if(g_mReInitMono)
        {
            if(monoWrapper::mono_isSimulationRunning())
            {
                EditorGEARApp::getSceneWorldEditor()->stopSimulationAndReloadUpdatedScripts();
            }
            else
            {
                monoWrapper::reInitMono(EditorGEARApp::getProjectHomeDirectory());
                EditorGEARApp::loadSceneFromTempFolder();
            }
            g_mReInitMono = false;
        }
        
        SDL_Event e;
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else
            {
                Uint32 event_wndID=e.window.windowID;
                
                if (e.type==SDL_DROPFILE)
                {
                    SDL_DropEvent* dropEvent = (SDL_DropEvent*)&e;
                    MDropData* dropData = (MDropData*)dropEvent->file;
                    event_wndID=dropData->getSourceWindowID();
                    UNUSED(dropData);
                }
#ifdef GEAR_APPLE
                else if(e.type==EditorApp::g_iAppSpecificEventType_MenuItemCmd)
                {
                    event_wndID = e.user.windowID;
                }
#endif
                //process other events
                std::function<void(SDL_Window*, SDL_Event&, void* userdata)>* pump = windowMessagePump::getInstance().getMessageLoopFunction(SDL_GetWindowFromID(event_wndID));
                if(pump)
                {
                    std::function<void(SDL_Window*, SDL_Event&, void* userdata)>& func2 = *pump;
                    func2(SDL_GetWindowFromID(event_wndID), e, (void*)&editorApp);
                }
            }
        }

        //rendering loop
        auto wndMsgLoops = windowMessagePump::getInstance().getWindowMessageLoops();
        for (auto it : *wndMsgLoops)
        {
            if(it.second->secondryView==nullptr)
            {
                //update the editor
                Timer::update();
                editorApp.update(Timer::getDtinSec());
                editorApp.draw();
            }
            else
            {
                it.second->secondryView->drawView();
            }
        }

        //Clear released Ref objects from the auto-release pool for this loop
        GEAR::Memory::AutoReleasePool::getInstance().clearPool();
        
        SDL_Delay(rand()%10);
        
        //swapbuffer
        //SDL_GL_SwapWindow( window );  //SwapBuffer will be called by renderer. So no need to call it here.
    }

    monoWrapper::destroyMono();

    EditorGEARApp::deleteTempFolder();
    
    //clean up
    //SDL_GL_DeleteContext(context);  //DeleteContext will be called by renderer. So no need to call it here.
    SDL_DestroyWindow(window);
    SDL_Quit();
        
    return 0;
}

/*
int handleAppEvents(void *userdata, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_APP_TERMINATING:
//            Terminate the app.
//            Shut everything down before returning from this function.
            printf("SDL_APP_TERMINATING");
            return 0;
        case SDL_APP_LOWMEMORY:
//            You will get this when your app is paused and iOS wants more memory.
//            Release as much memory as possible.
            printf("SDL_APP_LOWMEMORY");
            return 0;
        case SDL_APP_WILLENTERBACKGROUND:
//            Prepare your app to go into the background.  Stop loops, etc.
//            This gets called when the user hits the home button, or gets a call.
            printf("SDL_APP_WILLENTERBACKGROUND");
            return 0;
        case SDL_APP_DIDENTERBACKGROUND:
//            This will get called if the user accepted whatever sent your app to the background.
//            If the user got a phone call and canceled it, you'll instead get an    SDL_APP_DIDENTERFOREGROUND event and restart your loops.
//            When you get this, you have 5 seconds to save all your state or the app will be terminated.
//            Your app is NOT active at this point.
            printf("SDL_APP_DIDENTERBACKGROUND");
            return 0;
        case SDL_APP_WILLENTERFOREGROUND:
//            This call happens when your app is coming back to the foreground.
//            Restore all your state here.
            printf("SDL_APP_WILLENTERFOREGROUND");
            return 0;
        case SDL_APP_DIDENTERFOREGROUND:
//            Restart your loops here.
//            Your app is interactive and getting CPU again.
            printf("SDL_APP_DIDENTERFOREGROUND");
            return 0;
        default:
//            No special processing, add it to the event queue
            return 1;
    }
}
*/

void processEvent(SDL_Window * window, SDL_Event& e, void* userdata)
{
	EditorApp& editorApp = *(EditorApp*)userdata;
    
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
                editorApp.size(window_cx, window_cy);
            }
                break;
            
            case SDL_WINDOWEVENT_FOCUS_LOST:
            {
                printf("SDL_WINDOWEVENT_FOCUS_LOST\n");
                if(g_mAppInitialised)
                {
                    if(!monoWrapper::mono_isSimulationRunning())
                    {
                        EditorGEARApp::saveSceneToTempFolder();
                    }
                }
            }
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            {
                printf("SDL_WINDOWEVENT_FOCUS_GAINED\n");

                if(g_mAppInitialised)
                {
                    g_mReInitMono = true;
                    printf("ReInit mono after one loop\n");
                }
                else
                {
                    g_mAppInitialised = true;
                }
            }
                break;
                
            case SDL_WINDOWEVENT_SHOWN:
            {
                printf("SDL_WINDOWEVENT_SHOWN\n");
            }
                break;

            case SDL_WINDOWEVENT_EXPOSED:
            {
                printf("SDL_WINDOWEVENT_EXPOSED\n");
            }
                break;

            default:
                break;
        }
    }
    else if(e.type==SDL_KEYDOWN)
    {
        SDL_KeyboardEvent* keyBoardEvent = (SDL_KeyboardEvent*)&e;
        //const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		g_mControlKeyPressed = (keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL);
        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
        {
            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyDown(keyBoardEvent->keysym.scancode, keyBoardEvent->keysym.mod);
            UNUSED(bCaptured);
        }
        else
        {
            editorApp.KeyDown(keyBoardEvent->keysym.scancode, 0/*lParam*/);
        }
    }
    else if(e.type==SDL_KEYUP)
    {
        SDL_KeyboardEvent* keyBoardEvent = (SDL_KeyboardEvent*)&e;
        //const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if(g_mControlKeyPressed)
        {
            if((keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL))
                g_mControlKeyPressed = false;
        }

        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
        {
            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyUp(keyBoardEvent->keysym.scancode, keyBoardEvent->keysym.mod);
            UNUSED(bCaptured);
        }
        else
        {
            editorApp.KeyUp(keyBoardEvent->keysym.scancode, 0/*lParam*/);
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
				editorApp.MouseLButtonDown(mouse_x, mouse_y, MK_LBUTTON | ((g_mControlKeyPressed)?MK_CONTROL:0));
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
                //DEBUG_PRINT("Middle Mouse Down");
                editorApp.MouseMButtonDown(mouse_x, mouse_y, MK_MBUTTON);
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
                //DEBUG_PRINT("Right Mouse Down");
                editorApp.MouseRButtonDown(mouse_x, mouse_y, MK_RBUTTON);
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
                //geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
				editorApp.MouseLButtonUp(mouse_x, mouse_y, MK_LBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
            }
                break;
            case SDL_BUTTON_MIDDLE:
            {
                //DEBUG_PRINT("Middle Mouse Up");
                editorApp.MouseMButtonUp(mouse_x, mouse_y, MK_MBUTTON);
            }
                break;
            case SDL_BUTTON_RIGHT:
            {
                //DEBUG_PRINT("Right Mouse Up");
                editorApp.MouseRButtonUp(mouse_x, mouse_y, MK_RBUTTON);
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

        //DEBUG_PRINT("m_x=%d, m_y%d", mouse_x, mouse_y);
        SDL_MouseMotionEvent* mouseMotionEvent = (SDL_MouseMotionEvent*)&e;
        switch (mouseMotionEvent->state) {
            case 1:
				editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_LBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;
            case 2:
				editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_MBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;
            case 4:
				editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, MK_RBUTTON | ((g_mControlKeyPressed) ? MK_CONTROL : 0));
                break;

            default:
                editorApp.MouseMove(mouseMotionEvent->x, mouseMotionEvent->y, 0);
                break;
        }
    }
    else if(e.type==SDL_MOUSEWHEEL)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );

		int nFlags = (g_mControlKeyPressed) ? MK_CONTROL : 0;
        SDL_MouseWheelEvent* mouseWheelEvent = (SDL_MouseWheelEvent*)&e;
        editorApp.MouseWheel(mouseWheelEvent->y, mouse_x, mouse_y, nFlags);
    }
    else if (e.type==SDL_DROPFILE)
    {
        int mouse_x = 0, mouse_y = 0;
        SDL_GetMouseState( &mouse_x, &mouse_y );
        SDL_DropEvent* dropEvent = (SDL_DropEvent*)&e;
        MDropData* dropData = (MDropData*)dropEvent->file;
        editorApp.DragDrop(mouse_x, mouse_y, dropData);
        GE_DELETE(dropData);
        //SDL_FlushEvent(SDL_MOUSEMOTION);  //FlushEvent is called inside geGUIBase.
    }
#ifdef GEAR_APPLE
    else if(e.type==EditorApp::g_iAppSpecificEventType_MenuItemCmd)
    {
        editorApp.DoCommand(e.user.code);
    }
#endif

#ifdef GEAR_WINDOWS
	else if (e.type==SDL_SYSWMEVENT)
	{
		SDL_SysWMEvent* wmEvent = (SDL_SysWMEvent*)&e;
		// event.syswm.msg contains a pointer to SDL_SysWMmsg
		// which contains the event information
        switch(wmEvent->msg->msg.win.msg)
        {
			//// The user selected the command from the menu or used
			//// a hot key
            case WM_COMMAND:
			{          
				// Find which sub command was selected
				switch(LOWORD(wmEvent->msg->msg.win.wParam))
                {
				case ID_PROJECT_BUILDFORANDROID:
					{
						printf("\n================ANDROID BUILD ENGINE===============\n");
						char inputbuffer[1024*6];

						sprintf(inputbuffer, "%s//AndroidProjectMaker.exe %s %s win32", EditorApp::getAppDirectory().c_str(), EditorApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory());
						if(monoWrapper::exec_cmd(inputbuffer)!=0)
						{
							printf("\nERROR\n");
						}
						printf("\n======================================================\n");
					}
					break;
				case ID_EDIT_ACTIVECAMERAPROPERTY:
					{
						auto cam=monoWrapper::mono_engine_getWorld(0)->getActiveCamera();
						EditorGEARApp::getSceneWorldEditor()->selectedObject3D(cam->getAttachedObject());
						EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject(cam->getAttachedObject());
					}
					break;
				case ID_EDIT_OCTREEPROPERTY:
					{
						EditorGEARApp::getScenePropertyEditor()->populatePropertyOfOctree();
					}
					break;
				case ID_EDIT_LAYERSPROPERTY:
					{
						EditorGEARApp::getScenePropertyEditor()->populatePropertyOfLayers();
					}
					break;
				case ID_EDIT_FOGSETTINGS:
					{
						EditorGEARApp::getScenePropertyEditor()->populateSettingsOfFog();
					}
					break;
				case ID_FILE_SAVESCENE:
					{
						std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
						root_dir+="/Assets";
						std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
						char output_buffer[MAX_PATH];
						if(EditorGEARApp::showSaveCommonDlg(EditorGEARApp::getMainWindowHandle(), output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
						{
							if(EditorGEARApp::getSceneHierarchy()->saveCurrentScene(output_buffer))
							{
								EditorGEARApp::getSceneProject()->populateProjectView();
								EditorGEARApp::getSceneFileView()->populateFileView();

								if (monoWrapper::mono_isSimulationRunning())
								{
									EditorGEARApp::getSceneWorldEditor()->stopSimulation(false);
								}
								if (!EditorGEARApp::saveScene(output_buffer))
								{
									DEBUG_PRINT("Error saving scene : %s", output_buffer);
								}
								else
								{
									EditorGEARApp::updateCurrentSceneFile(output_buffer);
								}

							}
						}
					}
					break;
				case ID_FILE_OPENSCENE:
					{
						std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
						root_dir+="/Assets";
						std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
						char output_buffer[MAX_PATH];
						if(EditorGEARApp::showOpenCommonDlg(EditorGEARApp::getMainWindowHandle(), output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
						{
							if(EditorGEARApp::getSceneHierarchy()->loadScene(output_buffer))
							{
								EditorGEARApp::getSceneProject()->populateProjectView();
								EditorGEARApp::getSceneFileView()->populateFileView();

								if (!EditorGEARApp::loadScene(output_buffer))
								{
									DEBUG_PRINT("Error loading scene : %s", output_buffer);
								}
								else
								{
									EditorGEARApp::updateCurrentSceneFile(output_buffer);
								}
							}
						}
					}
					break;
				default:
					{
						editorApp.DoCommand(LOWORD(wmEvent->msg->msg.win.wParam));
					}
                }
                break;
			}
			break;
        }
	}
#endif
}
