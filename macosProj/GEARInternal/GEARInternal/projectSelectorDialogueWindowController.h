//
//  projectSelectorDialogueWindowController.m
//  GEARInternal
//
//  Created by Mudaliar, Arun on 6/25/15.
//  Copyright (c) 2015 ea. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <stdio.h>
#include "EditorApp.h"
#include "appEntry.h"
#include "../../../GUIInternal/GEAREditor/OSSpecific/ConfirmationDialog.h"

@interface projectSelectorDialogueWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate>
{
    std::vector<std::string> projectsArray;
}

@property (weak) IBOutlet NSTableColumn *columnHandle;
@property (weak) IBOutlet NSTableView *tableView;
@end

@implementation projectSelectorDialogueWindowController

- (id)init
{
    self = [super initWithWindowNibName:@"projectSelectorDialogue"];
    
    if(self!=nil)
    {
        //read the recent dialog
        FILE* fp = fopen("recentProjects", "r");
        if(fp)
        {
            char temp_buffer[1024];
            while (fscanf(fp, "%s\n", temp_buffer) != EOF)
            {
                projectsArray.push_back(temp_buffer);
            }
            fclose(fp);
        }
    }
    
    return self;
}

//this is a simple override of -showWindow: to ensure the window is always centered
-(IBAction)showWindow:(id)sender
{
    [super showWindow:sender];
    
    [_columnHandle setTitle:@"My Projects"];
    
    [_tableView setDataSource:self];
    [_tableView setDelegate:self];
    
    [[self window] center];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return projectsArray.size();
}

- (NSView *)tableView:(NSTableView *)tableView
   viewForTableColumn:(NSTableColumn *)tableColumn
                  row:(NSInteger)row {
    
    // Get an existing cell with the MyView identifier if it exists
    NSTableCellView *result = [tableView makeViewWithIdentifier:@"projectTableCellView" owner:self];
    NSString* projectPath = [NSString stringWithUTF8String:projectsArray[row].c_str()];
    [result.textField setStringValue:projectPath];
    
    return result;
    
}

-(IBAction) newProject:(id)sender
{
//    char* project_directory=browseFolder(hWndDlg, _T("Select an empty folder to create project."));
//    if(EditorGEARApp::createNewProject(project_directory)!=0)
//    {
//        MessageBox(hWndDlg, "Project creation failed", "Error.", MB_OK | MB_ICONERROR);
//        GE_DELETE_ARY(project_directory);
//        return true;
//    }
//    EditorGEARApp::setProjectHomeDirectory(project_directory);
//    GE_DELETE_ARY(project_directory);
//    EndDialog(hWndDlg, 1);
    
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Set array of file types
    //NSArray *fileTypesArray;
    //fileTypesArray = [NSArray arrayWithObjects:@"gearscene", nil];
    
    // Enable options in the dialog.
    [openDlg setCanChooseDirectories:YES];
    [openDlg setCanCreateDirectories:YES];
    [openDlg setCanChooseFiles:NO];
    //[openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:FALSE];
    
    //std::string root_dir = EditorGEARApp::getProjectHomeDirectory();
    //root_dir+="/Assets";
    //[openDlg setDirectoryURL: [NSURL fileURLWithPath:[NSString stringWithUTF8String:root_dir.c_str()]]];
    
    // Display the dialog box.  If the OK pressed,
    // process the files.
    if ( [openDlg runModal] == NSModalResponseOK )
    {
        // Gets list of all files selected
        NSURL *theFile = [openDlg URL];
        char output_buffer[FILENAME_MAX];
        strcpy(output_buffer, [[theFile path] UTF8String]);
        if(EditorGEARApp::createNewProject(output_buffer)!=0)
        {
            //MessageBox(hWndDlg, "Project creation failed", "Error.", MB_OK | MB_ICONERROR);
            ConfirmationDialog::ShowMessageDialog("Project creation failed");
            return;
        }
        EditorGEARApp::setProjectHomeDirectory(output_buffer);
        //[[self window] close];
        macos_main();
    }
}

-(IBAction) otherProject:(id)sender
{
    
}

-(IBAction) openProject:(id)sender
{
    // Get current selection index in listbox
    int itemIndex = (int)[_tableView selectedRow];
    if (itemIndex == -1)
    {
        ConfirmationDialog::ShowMessageDialog("Select atleast one recent project");
        //MessageBox(hWndDlg, "Select atleast one recent project", "GEAR.", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        // Get actual text in buffer
        //char temp_buffer[1024];
        //SendDlgItemMessage(hWndDlg, IDC_PROJ_DLG_RECENT_PROJECT_LIST, LB_GETTEXT, (WPARAM) itemIndex, (LPARAM) temp_buffer );
        EditorGEARApp::setProjectHomeDirectory(projectsArray[itemIndex].c_str());
        //EndDialog(hWndDlg, 2);
        //[[self window] close];
        macos_main();
    }
}

@end