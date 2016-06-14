// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#define _CRT_SECURE_NO_DEPRECATE



// Windows 头文件:
#include <windows.h>
#include <windowsx.h>
#include <objbase.h>
#include <Commdlg.h>

#include "OleIdl.h"
#include "ShObjIdl.h"

#include <vector>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
//
// TODO: 在此处引用程序需要的其他头文件



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