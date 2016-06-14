#include<Windows.h>
#include <CommCtrl.h>
#include"resource.h"
#include <string.h>
#include <tchar.h>

HMODULE g_hModule;
HHOOK hhkCBT = NULL;
HHOOK hhkGetMessage = NULL;
HHOOK hhkShell = NULL;
HHOOK hhkLowLevelMouseProc = NULL;
TCHAR*  kszWindowClassName = _T("SuperVirtualKeyBoard");
HWND hTFirstForm = NULL;
DWORD dLastTickCount = 0;

void StartHook();
void EndHook();
LRESULT CALLBACK HookCBRProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookGetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookShellProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);


BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void ShowVirtualBoard();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hModule = hModule;
		StartHook();
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		EndHook();
		HWND hwnd = ::FindWindow(kszWindowClassName, NULL);
		if (hwnd != NULL)
		{
			PostMessage(hwnd, WM_QUIT, 0, 0);
		}

		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

void StartHook()
{
//	hhkShell = SetWindowsHookEx(WH_SHELL, HookShellProc, g_hModule, GetCurrentThreadId());

//	hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE, HookGetMessageProc, g_hModule, GetCurrentThreadId());

	hhkLowLevelMouseProc = SetWindowsHookEx(WH_MOUSE_LL, HookLowLevelMouseProc, g_hModule, 0);

	//if (hhkShell == NULL)
	//{
	//	::MessageBox(0, _T("Error hook WH_SHELL !"), _T("DLL"), MB_OK);
	//}
	//if (hhkGetMessage == NULL)
	//{
	//	::MessageBox(0, _T("Error hook WH_GETMESSAGE !"), _T("DLL"), MB_OK);
	//}

	if (hhkLowLevelMouseProc == NULL)
	{
		::MessageBox(0, _T("Error hook WH_MOUSE_LL !"), _T("DLL"), MB_OK);
	}

}
BOOL IncreasePriority(DWORD dwPriorityClass, int nPriority)
{

	DWORD nCurrentProcessId;
	HANDLE hProcess;
	HANDLE hThread;

	nCurrentProcessId = GetCurrentProcessId();
	hProcess = OpenProcess(0x200u, 0, nCurrentProcessId);
	hThread = GetCurrentThread();
	SetPriorityClass(hProcess, dwPriorityClass);
	SetThreadPriority(hThread, nPriority);
	return CloseHandle(hProcess);
}


void EndHook()
{
	BOOL isUnHooked = TRUE;
	DWORD dLastErr = 0;

	//ChangeWindowMessageFilter()
	if (hhkShell)
	{
		SetLastError(0);
		if (!UnhookWindowsHookEx(hhkShell))
		{
			dLastErr = GetLastError();
			::MessageBox(0, _T("Error unhook WH_SHELL !"), _T("DLL"), MB_OK);
		}
	}

	if (hhkGetMessage)
	{
		SetLastError(0);
		if (!UnhookWindowsHookEx(hhkGetMessage))
		{
			dLastErr = GetLastError();
			::MessageBox(0, _T("Error unhook WH_GETMESSAGE !"), _T("DLL"), MB_OK);
		}

	}

	SetLastError(0);

	if (hhkLowLevelMouseProc)
	{
		if (!UnhookWindowsHookEx(hhkLowLevelMouseProc))
		{
			dLastErr = GetLastError();
			::MessageBox(0, _T("Error unhook WH_MOUSE_LL !"), _T("DLL"), MB_OK);
		}

	}

}

HWND GetSomeHandle()
{
	BOOL result;

	result = ((GetTickCount() - dLastTickCount) > 0x3A98);

	if (!hTFirstForm || result)
	{
		dLastTickCount = GetTickCount();
		hTFirstForm = FindWindow(_T("TFirstForm"), _T("CKeyboardFirstForm"));
	}

	return hTFirstForm;
}


LRESULT CALLBACK HookGetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = NULL;

	if (!nCode)
	{

		PMSG pMsg = (PMSG)lParam;
		switch (pMsg->message)
		{

		case WM_INPUTLANGCHANGEREQUEST:
			hwnd = GetSomeHandle();
			if (hwnd)
				PostMessage(hwnd, 0x496u, lParam, pMsg->lParam);
			break;
		case 0x0497:
			if (pMsg->lParam == 1514874)
			{
				ActivateKeyboardLayout((HKL)pMsg->wParam, 0);
				hwnd = GetSomeHandle();
				if (hwnd)
					PostMessage(hwnd, 0x498u, 0, 0);
			}
			break;
		case 0x4BA:
			hwnd = 0;
			break;
		}
	}

	return CallNextHookEx(hhkGetMessage, nCode, wParam, lParam);
}



LRESULT CALLBACK HookShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = NULL;
	switch (nCode)
	{
	case HSHELL_WINDOWACTIVATED:
		hwnd = GetSomeHandle();
		if (hwnd)
			PostMessage(hwnd, 0x4A1u, wParam, lParam);
		break;
	case HSHELL_LANGUAGE:
		hwnd = GetSomeHandle();
		if (hwnd)
			PostMessage(hwnd, 0x496u, wParam, lParam);
		break;
	case HSHELL_WINDOWREPLACED:
		hwnd = GetSomeHandle();
		if (hwnd)
			PostMessage(hwnd, 0x4A1u, lParam, 0);
		break;
	}
	return CallNextHookEx(hhkShell, nCode, wParam, lParam);
}


LRESULT CALLBACK HookLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	MSLLHOOKSTRUCT* lpMSLLHOOKSTRUCT = (MSLLHOOKSTRUCT*)lParam;

	if (nCode == HC_ACTION)
	{

		if ((wParam == WM_LBUTTONDOWN) || (wParam == WM_LBUTTONUP) || (wParam == WM_RBUTTONDOWN) || (wParam == WM_RBUTTONUP))
		{

			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				ShowVirtualBoard();
				break;
			case WM_LBUTTONUP:
				break;
			case WM_RBUTTONDOWN:
				break;
			case WM_RBUTTONUP:
				break;
			default:
				break;
			}
		}


	}

	return	CallNextHookEx(hhkLowLevelMouseProc, nCode, wParam, lParam);

}


void ShowVirtualBoard()
{

	//SendMessageTimeout;

	GUITHREADINFO guiThreadInfo;
	guiThreadInfo.cbSize = sizeof(guiThreadInfo);

	HWND hForegroundWindow = GetForegroundWindow();
	if (IsWindow(hForegroundWindow))
	{
		DWORD ntWindowThreadProcessId = GetWindowThreadProcessId(hForegroundWindow, 0);
		if (GetGUIThreadInfo(ntWindowThreadProcessId, &guiThreadInfo))
		{

			HWND hVirtualKeyBoard = ::FindWindow(kszWindowClassName, NULL);

			if (guiThreadInfo.hwndCaret != nullptr)
			{

				if (hVirtualKeyBoard != NULL)
				{

					if (IsWindow(hVirtualKeyBoard))
					{
						if (::IsIconic(hVirtualKeyBoard))
						{
							::ShowWindow(hVirtualKeyBoard, SW_RESTORE); //还原最小化的窗口
						}

						::ShowWindow(hVirtualKeyBoard, SW_SHOW);

						POINT point;//光标位置 
						point.x = guiThreadInfo.rcCaret.left;
						point.y = guiThreadInfo.rcCaret.top;
						::ClientToScreen(guiThreadInfo.hwndCaret, &point);
						//::SetWindowPos(hVirtualKeyBoard, nullptr, point.x+40, point.y+40, 0, 0, SWP_NOSIZE | SWP_NOZORDER);		
					}

				}
				else
				{
					ShellExecute(NULL, _T("open"), _T("virtualkeyboard.exe"), NULL, NULL, SW_SHOWNORMAL);
				}

			}
			else
			{

				if (hVirtualKeyBoard != NULL)
				{

					if (IsWindow(hVirtualKeyBoard))
					{

						//::ShowWindow(hVirtualKeyBoard, SW_HIDE);
						::ShowWindow(hVirtualKeyBoard, SW_MINIMIZE); //最小化的窗口

					}

				}

			}
		}

	}


}


LRESULT CALLBACK HookCBRProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		//CWPSTRUCT* cwps = (CWPSTRUCT*)lParam;

		if (nCode == HCBT_MOVESIZE)
		{
			OutputDebugString(L"HCBT_MOVESIZE called \n");
		}
		else if (nCode == HCBT_SYSCOMMAND)
		{
			OutputDebugString(L"HCBT_SYSCOMMAND called \n");
		}
		else if (nCode == HCBT_SETFOCUS)
		{
			//MessageBox(nullptr, L"ssss", nullptr, MB_OK);

			//Specifies the handle to the window gaining the keyboard focus.
			HWND hSetfocus = (HWND)wParam;

			//TCHAR szClassName[256];



			//if (0!= GetClassName(hSetfocus, szClassName, 256 - 1))
			//{
			//	if (_tcscmp(szClassName, WC_EDIT) == 0)

		}

	}
	//在钩子子程中调用得到控制权的钩子函数在完成对消息的处理后，如果想要该消息继续传递，那么它必须调用另外一个 SDK中的API函数CallNextHookEx来传递它，以执行钩子链表所指的下一个钩子子程。这个函数成功时返回钩子链中下一个钩子过程的返回值， 返回值的类型依赖于钩子的类型
	return CallNextHookEx(hhkCBT, nCode, wParam, lParam);
}


BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
	}

	return FALSE;
}


//just for exporting a function
__declspec(dllexport) void EmptyFunc()
{
	//Do nothing
}