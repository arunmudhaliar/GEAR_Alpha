#pragma once

#ifdef GEAR_WINDOWS
#include <Windows.h>
#endif
#include <string.h>
#ifdef GEAR_WINDOWS
#include "../../resource.h"
#endif
#ifndef GEAR2D
#include "../EditorApp.h"
#else
#include "../../../../GEAR2D_Alpha/GEAR2D/app/Editor2DApp.h"
#endif

class ConfirmationDialog {
private:
    static std::string g_ConfirmationDlgMessage;

public:
    enum ConfirmationDialogButton {
        BTN_NO,
        BTN_YES,
        BTN_CANCEL,
        BTN_INVALID
    };

    static ConfirmationDialogButton ShowConfirmationDialog(std::string msg);
    static void ShowMessageDialog(std::string msg);
};
