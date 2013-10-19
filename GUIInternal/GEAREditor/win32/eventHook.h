#ifndef EVENTHOOK_H
#define EVENTHOOK_H

#include <Windows.h>
#include <process.h>
#include <oleacc.h>
#pragma comment (lib, "oleacc.lib")
#include <stdio.h>
#include "GEAREditor\EditorApp.h"

namespace eventHook
{
EditorApp* g_pEditorAppPtr=NULL;

void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	IAccessible* pAcc = NULL;
	VARIANT varChild;

	HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);  

	if ((hr == S_OK) && (pAcc != NULL))
	{
		//VARIANT varResult = {};
		//pAcc->get_accRole(varChild, &varResult);

		//if (varResult.lVal == ROLE_SYSTEM_PAGETAB || varResult.lVal == NULL)
		{
			//BSTR bstrName;
			//pAcc->get_accName(varChild, &bstrName);

			if (event == EVENT_SYSTEM_FOREGROUND) 
			{
				printf("EVENT_SYSTEM_FOREGROUND\n");
				g_pEditorAppPtr->importAssetToMetaData(hwnd, GetModuleHandle(NULL));
			}
			//printf("%S\n", bstrName);
			//SysFreeString(bstrName);
		}
		//pAcc->Release();
	}
}

unsigned __stdcall hookEvent(HWND hWindow)
{
	//HWND hWindow = FindWindow(TEXT("Notepad++"), NULL);

	if (hWindow != NULL)
	{
		DWORD ProcessId, ThreadId;
		ThreadId = GetWindowThreadProcessId(hWindow, &ProcessId);

		//CoInitialize(NULL);
		HWINEVENTHOOK hHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, HandleWinEvent, ProcessId, ThreadId, WINEVENT_OUTOFCONTEXT);

		//MSG msg;
		//while (GetMessage(&msg, NULL, 0, 0) > 0);
		
		//UnhookWinEvent(hHook);
		//CoUninitialize();
	}
	return 0;
}
}
#endif