#include<Windows.h>
#include <CommCtrl.h>
#include"resource.h"
#include <string.h>
#include <tchar.h>
#include <thread> 

//给共享段起的名字不能超过8个字符，否则会被截断
#pragma data_seg("GlobleValue")
//共享数据 ，再这个段之间的数据都可以被所有加载这个dll的进程共享
HHOOK hhkCBT = NULL;
HHOOK hhkShell = NULL;
HHOOK hhkLowLevelMouse = NULL;
HHOOK hhkKeyBoard = NULL;
HMODULE g_hModule = NULL;
int nDLLLoadCount = 0;
int nDLLUnLoadCount = 100;
BOOL isFristLoad = FALSE;
#pragma data_seg()   

#pragma comment(linker,"/SECTION:GlobleValue,RWS")    //告诉链接器，shared段是可读可写并且是共享的 





TCHAR*  kszWindowClassName = _T("SuperVirtualKeyBoard");



LRESULT CALLBACK HookCBRProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookShellProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void ShowVirtualBoard();


__declspec(dllexport)  void StartHook();
__declspec(dllexport)  void EndHook();


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{

		if (!isFristLoad)
		{
			g_hModule = hModule;
			StartHook();
		}

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		--nDLLUnLoadCount;
		--nDLLLoadCount;
		if (nDLLLoadCount==0)
		{
			EndHook();
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
	
	//hhkShell = SetWindowsHookEx(WH_SHELL, HookShellProc, g_hModule, GetCurrentThreadId());
	//if (hhkShell == NULL)
	//{
	//	::MessageBox(0, _T("Error hook WH_SHELL !"), _T("Error"), MB_OK);
	//}

	++nDLLLoadCount;

	if (hhkLowLevelMouse == NULL)
	{
		hhkLowLevelMouse = SetWindowsHookEx(WH_MOUSE_LL, HookLowLevelMouseProc, g_hModule, 0);
		if (hhkLowLevelMouse == NULL)
		{
			::MessageBox(0, _T("Error hook WH_MOUSE_LL !"), _T("Error"), MB_OK);
		}

	}

	/*
	// 设置全局键盘钩子

	if (hhkKeyBoard == NULL)
	{
		hhkKeyBoard = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, g_hModule, 0);
		if (hhkKeyBoard == NULL)
		{
			DWORD dwErrorCode = GetLastError();
			::MessageBox(0, _T("Error hook WH_KEYBOARD !"), _T("Error"), MB_OK);
		}
	}
	*/
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
			::MessageBox(0, _T("Error unhook WH_SHELL !"), _T("Error"), MB_OK);
		}
		hhkShell = NULL;
	}

	if (hhkLowLevelMouse)
	{
		SetLastError(0);
		if (!UnhookWindowsHookEx(hhkLowLevelMouse))
		{
			dLastErr = GetLastError();
			::MessageBox(0, _T("Error unhook WH_MOUSE_LL !"), _T("Error"), MB_OK);
		}

		hhkLowLevelMouse = NULL;
	}

	if (hhkKeyBoard)
	{
		SetLastError(0);
		if (!UnhookWindowsHookEx(hhkKeyBoard))
		{
			dLastErr = GetLastError();
			::MessageBox(0, _T("Error unhook WH_KEYBOARD!"), _T("Error"), MB_OK);
		}

		hhkKeyBoard = NULL;
	}

}



LRESULT CALLBACK HookShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = NULL;
	switch (nCode)
	{
	case HSHELL_WINDOWACTIVATED:

		break;
	case HSHELL_LANGUAGE:

		break;
	case HSHELL_WINDOWREPLACED:

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

	return	CallNextHookEx(hhkLowLevelMouse, nCode, wParam, lParam);

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
							//Sets the show state of a window without waiting for the operation to complete.
							//::ShowWindowAsync(hVirtualKeyBoard, SW_RESTORE);
						}

						//SetFocus(guiThreadInfo.hwndCaret);
						::ShowWindow(hVirtualKeyBoard, SW_SHOWNA);

						POINT point;//光标位置 
						point.x = guiThreadInfo.rcCaret.left;
						point.y = guiThreadInfo.rcCaret.top;
						::ClientToScreen(guiThreadInfo.hwndCaret, &point);
						//::SetWindowPos(hVirtualKeyBoard, nullptr, point.x+40, point.y+40, 0, 0, SWP_NOSIZE | SWP_NOZORDER);		
					}

				}
				else
				{
					//ShellExecute(NULL, _T("open"), _T("virtualkeyboard.exe"), NULL, NULL, SW_SHOWNORMAL);
				}

			}
			else
			{

				if (hVirtualKeyBoard != NULL)
				{
					if (guiThreadInfo.hwndActive != hVirtualKeyBoard)
					{
						if (IsWindow(hVirtualKeyBoard))
						{
							//::ShowWindow(hVirtualKeyBoard, SW_HIDE);
							//::ShowWindow(hVirtualKeyBoard, SW_MINIMIZE); //最小化的窗口

						}
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


// 键盘钩子消息处理过程
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	TCHAR szBuf[128];
	// 最高位为0(lParam>0)，表示VM_KEYDOWN消息
	if ((nCode == HC_ACTION) && (lParam > 0))
	{
		WCHAR KeyName[10] = { 0 };
		GetKeyNameText((LONG)lParam, KeyName, 50);

		swprintf_s(szBuf, _T("KeyName:%s load:%d unload:%d"), KeyName, nDLLLoadCount, 100 - nDLLUnLoadCount);
		MessageBox(NULL, szBuf, L"全局键盘钩子", MB_OK);
	}

	// 继续传递消息
	return CallNextHookEx(hhkKeyBoard, nCode, wParam, lParam);
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