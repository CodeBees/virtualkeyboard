// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#define _CRT_SECURE_NO_DEPRECATE



// Windows ͷ�ļ�:
#include <windows.h>
#include <windowsx.h>
#include <objbase.h>
#include <Commdlg.h>

#include "OleIdl.h"
#include "ShObjIdl.h"

#include <vector>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
//
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�



#ifdef Z_EXPORT_API
#define Z_EXPORT_API __declspec(dllexport)
#else
#define Z_EXPORT_API __declspec(dllimport)
#endif


#include "./3rd/DuiLib/UIlib.h"

using namespace DuiLib;
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, ".\\lib\\DuiLib_ud.lib")
#   else
#      pragma comment(lib, ".\\lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, ".\\lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, ".\\lib\\DuiLib.lib")
#   endif
#endif


extern "C" {
	extern HINSTANCE                       ghInstance;
}





#if defined _M_IX86  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#elif defined _M_IA64  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#elif defined _M_X64  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#else  
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  
#endif  