#include "stdafx.h"
#include "virtualkeyboard.h"
#include <winsock.h>

HINSTANCE ghInstance;							// ��ǰʵ��

extern TCHAR*  kszWindowClassName;


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//ʵ���������Ⱦ�����
	CPaintManagerUI::SetInstance(hInstance);

	//Initializes the COM library on the current thread and identifies,��ʼ��COM��, Ϊ����COM���ṩ֧��
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
		::MessageBox(NULL, _T("������ʧ��"), _T("Err"), MB_OK);
		return 0;
	}
	else
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{

			//ֻ������һ��ʵ��
			HWND hwnd = ::FindWindow(kszWindowClassName, NULL);

			if (hwnd != NULL)
			{

				if (IsWindow(hwnd))
				{
					if (::IsIconic(hwnd))
					{
						::ShowWindow(hwnd, SW_RESTORE); //��ԭ��С���Ĵ���
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
			::MessageBox(NULL, _T("����Dllʧ�ܣ�"), _T("��ʾ"), MB_OK);
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

