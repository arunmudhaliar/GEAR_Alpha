//
//  AppDelegate.m
//  testMenu
//
//  Created by Mudaliar, Arun on 4/8/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#import "AppDelegate.h"


//#include <iostream>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "../../../GUIInternal/GEAREditor/OSSpecific/MenuCPPInterface.h"

#include <stdlib.h>

@interface AppDelegate ()

//@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    
    putenv("ANDROID_HOME=/Users/amudaliar/adt-bundle-mac-x86_64-20131030/sdk");
    putenv("NDK_HOME=/Users/amudaliar/android-ndk-r9c");
    //ANT_HOME
    //putenv("ANT_HOME=");
    const char* android_home = getenv("ANDROID_HOME");
    const char* ndk_home = getenv("NDK_HOME");
    
    macos_main();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

-(IBAction) newScene:(id)sender
{

}

-(IBAction) saveScene:(id)sender
{
    std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    root_dir+="/Assets";
#ifdef _WIN32
    std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
#endif
    
    // Set the default name for the file and show the panel.
    NSSavePanel*    panel = [NSSavePanel savePanel];
    [panel setNameFieldStringValue:@"NewScene"];
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"gearscene", nil];
    
    // Enable options in the dialog.
    [panel setAllowedFileTypes:fileTypesArray];
    
    [panel setDirectoryURL: [NSURL fileURLWithPath:[NSString stringWithUTF8String:root_dir.c_str()]]];
    [panel beginSheetModalForWindow:nil completionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton)
        {
            NSURL*  theFile = [panel URL];
///////////////////////////////////////////
            {
                char output_buffer[FILENAME_MAX];
                strcpy(output_buffer, [[theFile path] UTF8String]);
                //if(EditorGEARApp::showSaveCommonDlg(hWnd, output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
                {
                    if(EditorGEARApp::getSceneHierarchy()->saveCurrentScene(output_buffer))
                    {
                        EditorGEARApp::getSceneProject()->populateProjectView();
                        EditorGEARApp::getSceneFileView()->populateFileView();
                        
                        //update the currentscene file
                        std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
                        root_dir+="/ProjectSettings/currentscene";
                        gxFile currenSceneFile;
                        if(currenSceneFile.OpenFile(root_dir.c_str(), gxFile::FILE_w))
                        {
                            const char* relativepath=AssetImporter::relativePathFromProjectHomeDirectory_AssetFolder(output_buffer);
                            char unix_path[FILENAME_MAX];
                            memset(unix_path, 0, FILENAME_MAX);
                            strcpy(unix_path, relativepath);
                            geUtil::convertPathToUnixFormat(unix_path);
                            currenSceneFile.Write(unix_path);
                            currenSceneFile.CloseFile();
                            
                            std::string wndTitle ="GEAR Alpha [";
                            wndTitle+=relativepath;
                            wndTitle+=+"]";
                            //SetWindowText(hWnd, wndTitle.c_str());
                        }
                        //
                    }
                }
            }
//////////////////////////////////////////
        }
    }];
}

-(IBAction) openScene:(id)sender
{
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];

    // Set array of file types
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"gearscene", nil];
    
    // Enable options in the dialog.
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:FALSE];
    
    std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    root_dir+="/Assets";
    [openDlg setDirectoryURL: [NSURL fileURLWithPath:[NSString stringWithUTF8String:root_dir.c_str()]]];

    // Display the dialog box.  If the OK pressed,
    // process the files.
    if ( [openDlg runModal] == NSModalResponseOK )
    {
        // Gets list of all files selected
        NSURL *theFile = [openDlg URL];

        // Loop through the files and process them.
        //for(int i = 0; i < [files count]; i++ )
        //if([files count])
        {
            // Do something with the filename.
            //NSLog(@"File path: %@", [[files objectAtIndex:0] path]);
#ifdef _WIN32
            std::replace( root_dir.begin(), root_dir.end(), '/', '\\');
#endif
            char output_buffer[FILENAME_MAX];
            strcpy(output_buffer, [[theFile path] UTF8String]);
            //if(EditorGEARApp::showOpenCommonDlg(hWnd, output_buffer, MAX_PATH, "GEAR Scene (*.gearscene)\0*.gearscene\0", "gearscene", root_dir.c_str()))
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
                        char unix_path[FILENAME_MAX];
                        memset(unix_path, 0, FILENAME_MAX);
                        strcpy(unix_path, relativepath);
                        geUtil::convertPathToUnixFormat(unix_path);
                        currenSceneFile.Write(unix_path);
                        currenSceneFile.CloseFile();
                        
                        std::string wndTitle ="GEAR Alpha [";
                        wndTitle+=relativepath;
                        wndTitle+=+"]";
                        //SetWindowText(hWnd, wndTitle.c_str());
                    }
                    //
                }
            }
        }
    }
}

-(IBAction) openCameraProperty:(id)sender
{
    object3d* cam=monoWrapper::mono_engine_getWorld(0)->getActiveCamera();
    EditorGEARApp::getSceneWorldEditor()->selectedObject3D(cam);
    EditorGEARApp::getScenePropertyEditor()->populatePropertyOfObject(cam);
}

-(IBAction) openOctreeProperty:(id)sender
{
    EditorGEARApp::getScenePropertyEditor()->populatePropertyOfOctree();
}

-(IBAction) openLayersProperty:(id)sender
{
    EditorGEARApp::getScenePropertyEditor()->populatePropertyOfLayers();
}

-(IBAction) buildForAndroid:(id)sender
{
    printf("\n================ANDROID BUILD ENGINE===============\n");
    char inputbuffer[1024*6];

    sprintf(inputbuffer, "mono %s//AndroidProjectMaker.exe %s %s macos", EditorApp::getAppDirectory().c_str(), EditorApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory());
    if(monoWrapper::exec_cmd(inputbuffer)!=0)
    {
        printf("\nERROR\n");
    }
    printf("\n======================================================\n");

}

@end
