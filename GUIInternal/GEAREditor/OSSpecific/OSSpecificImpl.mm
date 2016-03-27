#import "OSSpecificImpl.h"
#include "../EditorApp.h"

@implementation OSSpecificImpl

void cpp_saveAnimationFile(std::string& rootDir)
{
    [OSSpecificImpl saveAnimationFile: rootDir];
}

+ (void) saveAnimationFile: (std::string) rootDir
{    
    // Set the default name for the file and show the panel.
    NSSavePanel*    panel = [NSSavePanel savePanel];
    [panel setNameFieldStringValue:@"Animation"];
    NSArray *fileTypesArray;
    fileTypesArray = [NSArray arrayWithObjects:@"anim", nil];
    
    // Enable options in the dialog.
    [panel setAllowedFileTypes:fileTypesArray];
    
    [panel setDirectoryURL: [NSURL fileURLWithPath:[NSString stringWithUTF8String:rootDir.c_str()]]];
    
    [panel beginSheetModalForWindow:nil completionHandler:^(NSInteger result)
     {
         if (result == NSFileHandlingPanelOKButton)
         {
             NSURL*  theFile = [panel URL];
             std::string output_buffer([[theFile path] UTF8String]);
             
             gxFile file;
             file.OpenFile(output_buffer, gxFile::FILE_w);
             file.CloseFile();
             EditorGEARApp::getSceneFileView()->populateFileView();
        }
     }];
}

@end
