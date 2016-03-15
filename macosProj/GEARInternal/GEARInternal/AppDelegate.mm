#import "AppDelegate.h"

@interface AppDelegate ()

//@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    
    putenv((char*)"ANDROID_HOME=/Users/amudaliar/adt-bundle-mac-x86_64-20131030/sdk");
    putenv((char*)"ANDROID_ROOT=/Users/amudaliar/adt-bundle-mac-x86_64-20131030/sdk");
    putenv((char*)"NDK_HOME=/Users/amudaliar/android-ndk-r9c");
    //ANT_HOME
    putenv((char*)"ANT_HOME=/Users/amudaliar/apache-ant-1.9.5");
    
    const char* android_home = getenv("ANDROID_HOME");
    const char* android_root = getenv("ANDROID_ROOT");
    const char* ndk_home = getenv("NDK_HOME");
    const char* ant_home = getenv("ANT_HOME");
    
    int a;
    UNUSED(a);
    UNUSED(android_home);
    UNUSED(android_root);
    UNUSED(ndk_home);
    UNUSED(ant_home);
    
    m_projectSelectorDialogueWindowController = [[projectSelectorDialogueWindowController alloc] init];
    [m_projectSelectorDialogueWindowController showWindow:self];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    // Insert code here to tear down your application
}

-(IBAction) newScene:(id)sender
{
    char inputbuffer[1024*6];
    
    sprintf(inputbuffer, "sh /Users/amudaliar/adt-bundle-mac-x86_64-20131030/sdk//tools//android create project --target 2 --name gearApp --path /Users/amudaliar/Library/Developer/Xcode/DerivedData/GEARWorkSpace-gvjuxaxjnihvzyagexqhevujmvoi/Build/Products/Debug//TempAndroid --activity MainActivity --package com.gear.gearapp");
    if(monoWrapper::exec_cmd(inputbuffer)!=0)
    {
        
    }
/*
 sh /Users/amudaliar/adt-bundle-mac-x86_64-20131030/sdk//tools//android create project --target 2 --name gearApp --path /Users/amudaliar/Library/Developer/Xcode/DerivedData/GEARWorkSpace-gvjuxaxjnihvzyagexqhevujmvoi/Build/Products/Debug//TempAndroid --activity MainActivity --package com.gear.gearapp
 */
}

-(IBAction) saveScene:(id)sender
{
    std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    root_dir+="/Assets";
    
    // Set the default name for the file and show the panel.
    NSSavePanel*    panel = [NSSavePanel savePanel];
    [panel setNameFieldStringValue:@"NewScene"];
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"gearscene", nil];
    
    // Enable options in the dialog.
    [panel setAllowedFileTypes:fileTypesArray];
    
    [panel setDirectoryURL: [NSURL fileURLWithPath:[NSString stringWithUTF8String:root_dir.c_str()]]];
    [panel beginSheetModalForWindow:nil completionHandler:^(NSInteger result)
    {
        if (result == NSFileHandlingPanelOKButton)
        {
            NSURL*  theFile = [panel URL];
            std::string output_buffer([[theFile path] UTF8String]);
            if(!EditorGEARApp::saveScene(output_buffer))
            {
                DEBUG_PRINT("Error saving scene : %s", output_buffer.c_str());
            }
            else
            {
                EditorGEARApp::updateCurrentSceneFile(output_buffer);
            }
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

    if ( [openDlg runModal] == NSModalResponseOK )
    {
        NSURL *theFile = [openDlg URL];
        
        std::string output_buffer([[theFile path] UTF8String]);
        if(!EditorGEARApp::loadScene(output_buffer))
        {
            DEBUG_PRINT("Error loading scene : %s", output_buffer.c_str());
        }
        else
        {
            EditorGEARApp::updateCurrentSceneFile(output_buffer);
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

    std::string root_dir = "/Users/amudaliar/TEMP_PROJ/GEARREPO/dev/GUIInternal";
    sprintf(inputbuffer, "mono %s//AndroidProjectMaker.exe %s %s macos", EditorApp::getAppDirectory().c_str(), root_dir.c_str()/*EditorApp::getAppDirectory().c_str()*/, EditorGEARApp::getProjectHomeDirectory());
    if(monoWrapper::exec_cmd(inputbuffer)!=0)
    {
        printf("\nERROR\n");
    }
    printf("\n======================================================\n");

}

@end
