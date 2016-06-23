#include "stdafx.h"
#include "virtualkeyboard.h"
#include <winsock.h>

HINSTANCE ghInstance;							// 当前实例

extern TCHAR*  kszWindowClassName;


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//实例句柄与渲染类关联
	CPaintManagerUI::SetInstance(hInstance);

	//Initializes the COM library on the current thread and identifies,初始化COM库, 为加载COM库提供支持
	//	HRESULT Hr = ::CoInitialize(NULL);
	//OleInitialize calls CoInitializeEx internally to initialize the COM library on the current apartment.
	//Because OLE operations are not thread-safe, OleInitialize specifies the concurrency model as single-thread apartment.

	HRESULT Hr = ::OleInitialize(NULL);
	if (FAILED(Hr))
	{
		return 0;
	}

	SetLastError(0);
	HANDLE	hMutexForSingleInstance = CreateMutex(0, 1, _T("VirtualKeyBoardRunning"));
	if (NULL == hMutexForSingleInstance)
	{
		::MessageBox(NULL, _T("创建锁失败"), _T("Err"), MB_OK);
		return 0;
	}
	else
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{

			//只允许开启一个实例
			HWND hwnd = ::FindWindow(kszWindowClassName, NULL);

			if (hwnd != NULL)
			{

				if (IsWindow(hwnd))
				{
					if (::IsIconic(hwnd))
					{
						::ShowWindow(hwnd, SW_RESTORE); //还原最小化的窗口
					}

					::ShowWindow(hwnd, SW_SHOWNA);
					//::SetForegroundWindow(hwnd);
					return 0;
				}

			}

		}
	}
#ifdef DEBUG
	HINSTANCE	hInstDll = LoadLibrary(_T("AutoToast_d"));
#else
	HINSTANCE	hInstDll = LoadLibrary(_T("AutoToast.dll"));
#endif
	{
		if (!hInstDll)
		{
			::MessageBox(NULL, _T("加载Dll失败！"), _T("提示"), MB_OK);
		}
	}
	//
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	CDYVirtualKeyBoardFrameWnd *pFrame = NULL;
	//	CPaintManagerUI::ReloadSkin();
	pFrame = new CDYVirtualKeyBoardFrameWnd();
	//pFrame->Create(NULL, kszWindowClassName, UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
	pFrame->Create(NULL, kszWindowClassName, UI_WNDSTYLE_FRAME, WS_EX_TOPMOST |WS_EX_TOOLWINDOW| WS_EX_NOACTIVATE);
	pFrame->CenterWindow();
	pFrame->ShowModal();
	::OleUninitialize();

	return 0;
}

