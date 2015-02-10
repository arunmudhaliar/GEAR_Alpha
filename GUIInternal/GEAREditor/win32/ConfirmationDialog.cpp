#include "ConfirmationDialog.h"

std::string ConfirmationDialog::g_ConfirmationDlgMessage ="";


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