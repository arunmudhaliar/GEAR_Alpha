#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <string.h>
#ifdef _WIN32
#include "../../resource.h"
#endif
#ifndef GEAR2D
#include "../EditorApp.h"
#else
#include "../../../../GEAR2D_Alpha/GEAR2D/app/Editor2DApp.h"
#endif

class ConfirmationDialog
{
private:
	static std::string g_ConfirmationDlgMessage;

public:
    enum ConfirmationDialogButton
    {
        BTN_NO,
        BTN_YES,
        BTN_CANCEL,
        BTN_INVALID
    };
#if DEPRECATED
	static LRESULT ShowConfirmationDialog(std::string msg);
	static INT_PTR CALLBACK YesNoCancel_DlgBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
#else
    static ConfirmationDialogButton ShowConfirmationDialog(std::string msg);
#endif
};

#endif