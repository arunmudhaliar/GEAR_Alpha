#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <Windows.h>
#include <string.h>
#include "../../resource.h"
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
	static LRESULT ShowConfirmationDialog(std::string msg);
	static INT_PTR CALLBACK YesNoCancel_DlgBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif