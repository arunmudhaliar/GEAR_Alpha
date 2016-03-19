#include "appEntry.h"
#include "windowMessagePump.h"
#include <fstream>

bool g_mControlKeyPressed = false;
bool g_mAppInitialised = false;
bool g_mReInitMono = false;

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

    // create a window
    SDL_Window * window = SDL_CreateWindow(
                                           "GEAR 3D",             // window title
                                           SDL_WINDOWPOS_CENTERED,     // x position, centered
                                           SDL_WINDOWPOS_CENTERED,     // y position, centered
                                           640,                        // width, in pixels
                                           480,                        // height, in pixels
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN  | SDL_WINDOW_RESIZABLE        // flags
                                           );

#ifdef _WIN32
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
    
#ifdef _WIN32
    monoWrapper::loadMonoModules(gear_mono_install_path+"/lib", gear_mono_install_path+"/etc");
#elif defined(__APPLE__)
    monoWrapper::loadMonoModules(gear_mono_install_path+"/lib/", gear_mono_install_path+"/etc");
#else
    monoWrapper::loadMonoModules("/storage/emulated/0/gear/", "/storage/emulated/0/gear/");
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
#ifdef __APPLE__
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

void processEvent(SDL_Window * window, SDL_Event& e, void* userdata)
{
    EditorApp& editorApp = (EditorApp&)*userdata;
    
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
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		g_mControlKeyPressed = (keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL);
        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
        {
            //if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT])
            //    break;
            bool bShiftKeyPressed=(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]);
            UNUSED(bShiftKeyPressed);
//            char ch=MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
//            if(ch>=0x41 && ch<=0x5A)
//            {
//                if(!(shift_keystate&0x8000))
//                    ch=ch+32;
//            }
//            else if(ch>=0x31 && ch<=0x39)
//            {
//                if((shift_keystate&0x8000))
//                    ch=ch-16;
//            }
            
            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyDown(keyBoardEvent->keysym.scancode, 0/*lParam*/);   //TODO: need to check if lparam is used somewere in the code or not.
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
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		g_mControlKeyPressed = !((keyBoardEvent->keysym.scancode == SDL_SCANCODE_LCTRL) || (keyBoardEvent->keysym.scancode == SDL_SCANCODE_RCTRL));

        if(geTextBox::g_pCurrentlyActiveTextBoxPtr)
        {
            //if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT])
            //    break;
            bool bShiftKeyPressed=(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT]);
            UNUSED(bShiftKeyPressed);
            //            char ch=MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
            //            if(ch>=0x41 && ch<=0x5A)
            //            {
            //                if(!(shift_keystate&0x8000))
            //                    ch=ch+32;
            //            }
            //            else if(ch>=0x31 && ch<=0x39)
            //            {
            //                if((shift_keystate&0x8000))
            //                    ch=ch-16;
            //            }
            
            bool bCaptured=geTextBox::g_pCurrentlyActiveTextBoxPtr->KeyUp(keyBoardEvent->keysym.scancode, 0/*lParam*/);   //TODO: need to check if lparam is used somewere in the code or not.
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
                editorApp.MouseMButtonDown(mouse_x, mouse_y, MK_MBUTTON);
                //DEBUG_PRINT("Middle Mouse Down");
                //geTestDlg* view = new geTestDlg(&geFontManager::g_cFontManager, editorApp.rendererGL10);
                //view->showView();
                //showMenu();
                //cpp_createMenu();
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
                geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;
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
#ifdef __APPLE__
    else if(e.type==EditorApp::g_iAppSpecificEventType_MenuItemCmd)
    {
        editorApp.DoCommand(e.user.code);
    }
#endif

#ifdef _WIN32
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
						object3d* cam=monoWrapper::mono_engine_getWorld(0)->getActiveCamera();
						EditorGEARApp::getSceneWorldEditor()->selectedObject3D(cam);
						EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject(cam);
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

								//update the currentscene file
								root_dir = EditorGEARApp::getProjectHomeDirectory();
                                //check if ProjectSettings dir exist or not
                                if(!EditorApp::isDirecoryExist(root_dir+"/ProjectSettings"))
                                {
                                    EditorApp::createDirectory(root_dir+"/ProjectSettings");
                                }
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
									SetWindowText(EditorGEARApp::getMainWindowHandle(), wndTitle.c_str());
								}
								//
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

								//update the currentscene file
								root_dir = EditorGEARApp::getProjectHomeDirectory();
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
									SetWindowText(EditorGEARApp::getMainWindowHandle(), wndTitle.c_str());
								}
								//
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
