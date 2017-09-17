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
    
    if (self != nil) {
        // read the recent dialog
        FILE* fp = fopen("recentProjects", "r");
        if (fp) {
            char temp_buffer[1024];
            while (fscanf(fp, "%s\n", temp_buffer) != EOF) {
                projectsArray.push_back(temp_buffer);
            }
            fclose(fp);
        }
    }
    
    return self;
}

// this is a simple override of -showWindow: to ensure the window is always centered
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
    // Get an existing cell with the projectTableCellView identifier if it exists
    NSTableCellView *result = [tableView makeViewWithIdentifier:@"projectTableCellView" owner:self];
    NSString* projectPath = [NSString stringWithUTF8String:projectsArray[row].c_str()];
    [result.textField setStringValue:projectPath];
    
    return result;
}

-(IBAction) newProject:(id)sender
{
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    
    // Enable options in the dialog.
    [openDlg setCanChooseDirectories:YES];
    [openDlg setCanCreateDirectories:YES];
    [openDlg setCanChooseFiles:NO];
    // [openDlg setAllowedFileTypes:fileTypesArray];
    [openDlg setAllowsMultipleSelection:FALSE];
    
    // Display the dialog box.  If the OK pressed,
    if ( [openDlg runModal] == NSModalResponseOK ) {
        // Gets list of all files selected
        NSURL *theFile = [openDlg URL];
        char output_buffer[FILENAME_MAX];
        strcpy(output_buffer, [[theFile path] UTF8String]);     // NOLINT
        if (EditorGEARApp::createNewProject(output_buffer) != 0) {
            ConfirmationDialog::ShowMessageDialog("Project creation failed");
            return;
        }
        EditorGEARApp::setProjectHomeDirectory(output_buffer);
        
        appEntry();
    }
}

-(IBAction) otherProject:(id)sender
{
}

-(IBAction) openProject:(id)sender
{
    // Get current selection index in listbox
    int itemIndex = static_cast<int>([_tableView selectedRow]);
    if (itemIndex == -1) {
        ConfirmationDialog::ShowMessageDialog("Select atleast one recent project");
    } else {
        EditorGEARApp::setProjectHomeDirectory(projectsArray[itemIndex].c_str());
        appEntry();
    }
}

@end
