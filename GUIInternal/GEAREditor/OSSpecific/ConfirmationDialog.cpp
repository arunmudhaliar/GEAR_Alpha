#include "ConfirmationDialog.h"

std::string ConfirmationDialog::g_ConfirmationDlgMessage ="";

#if DEPRECATED
LRESULT ConfirmationDialog::ShowConfirmationDialog(std::string msg)
{
	g_ConfirmationDlgMessage=msg;
#ifndef GEAR2D
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNOCANCEL_ITERATECHILDS_BOX), EditorApp::getMainWindowHandle(), reinterpret_cast<DLGPROC>(YesNoCancel_DlgBox));
#else
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_YESNOCANCEL_ITERATECHILDS_BOX), Editor2DApp::getMainWindowHandle(), reinterpret_cast<DLGPROC>(YesNoCancel_DlgBox));
#endif
}

INT_PTR CALLBACK ConfirmationDialog::YesNoCancel_DlgBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(GetDlgItem(hDlg, IDC_STATIC_MSG), g_ConfirmationDlgMessage.c_str());
			return (INT_PTR)TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDYES)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDNO)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		break;
	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}
	return (INT_PTR)FALSE;
}
#else
ConfirmationDialog::ConfirmationDialogButton ConfirmationDialog::ShowConfirmationDialog(std::string msg)
{
    g_ConfirmationDlgMessage=msg;
    
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, BTN_NO, "no" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, BTN_YES, "yes" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, BTN_CANCEL, "cancel" },
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Confirmation", /* .title */
        g_ConfirmationDlgMessage.c_str(), /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return BTN_INVALID;
    }
    
    if (buttonid == -1) {
        SDL_Log("no selection");
    } else {
        SDL_Log("selection was %s", buttons[buttonid].text);
    }
    
    return (ConfirmationDialogButton)buttonid;
}
#endif