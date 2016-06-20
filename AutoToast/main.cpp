#include<Windows.h>
#include <CommCtrl.h>
#include"resource.h"
#include <string.h>
#include <tchar.h>
#include <thread> 

//�������������ֲ��ܳ���8���ַ�������ᱻ�ض�
#pragma data_seg("GlobleValue")
//�������� ���������֮������ݶ����Ա����м������dll�Ľ��̹���
HHOOK hhkCBT = NULL;
HHOOK hhkShell = NULL;
HHOOK hhkLowLevelMouse = NULL;
HHOOK hhkKeyBoard = NULL;
HMODULE g_hModule = NULL;
int nDLLLoadCount = 0;
int nDLLUnLoadCount = 100;
BOOL isFristLoad = FALSE;
#pragma data_seg()   

#pragma comment(linker,"/SECTION:GlobleValue,RWS")    //������������shared���ǿɶ���д�����ǹ���� 





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
	// ����ȫ�ּ��̹���

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

						POINT point;//���λ�� 
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
							//::ShowWindow(hVirtualKeyBoard, SW_MINIMIZE); //��С���Ĵ���

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
	//�ڹ����ӳ��е��õõ�����Ȩ�Ĺ��Ӻ�������ɶ���Ϣ�Ĵ���������Ҫ����Ϣ�������ݣ���ô�������������һ�� SDK�е�API����CallNextHookEx������������ִ�й���������ָ����һ�������ӳ̡���������ɹ�ʱ���ع���������һ�����ӹ��̵ķ���ֵ�� ����ֵ�����������ڹ��ӵ�����
	return CallNextHookEx(hhkCBT, nCode, wParam, lParam);
}


// ���̹�����Ϣ�������
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	TCHAR szBuf[128];
	// ���λΪ0(lParam>0)����ʾVM_KEYDOWN��Ϣ
	if ((nCode == HC_ACTION) && (lParam > 0))
	{
		WCHAR KeyName[10] = { 0 };
		GetKeyNameText((LONG)lParam, KeyName, 50);

		swprintf_s(szBuf, _T("KeyName:%s load:%d unload:%d"), KeyName, nDLLLoadCount, 100 - nDLLUnLoadCount);
		MessageBox(NULL, szBuf, L"ȫ�ּ��̹���", MB_OK);
	}

	// ����������Ϣ
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