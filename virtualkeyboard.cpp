#include "stdafx.h"
#include "virtualkeyboard.h"



TCHAR*  kszWindowClassName = _T("SuperVirtualKeyBoard");


BOOL SetWindowForeground(HWND hWnd)
{
	BOOL result = FALSE;

	if (hWnd)
	{
		SetForegroundWindow(hWnd);
		result = AllowSetForegroundWindow(ASFW_ANY);
	}

	return result;
}

struct NormalButtonID
{
	TCHAR* szCtrlID;
	CControlUI* pCtrlUIPointer;
	int nVirtualKeyValue;
	BOOL bIsCtrlKey;
};

struct ControlOptionID
{
	TCHAR* szCtrlID;
	CControlUI* pCtrlUIPointer;
	int nVirtualKeyValue;
	BOOL bIsCtrlKey;
};


static ControlOptionID controlKeyOptionIDs[] =
{
	{ _T("ui_btn_key_capslock"),NULL,VK_CAPITAL,FALSE },	//Caps Lock
	{ _T("ui_btn_key_numlock"),NULL,VK_NUMLOCK,FALSE },		//Num Lock
	{ _T("ui_btn_key_scrolllock"),NULL,VK_SCROLL,FALSE },	//Scroll Lock
	{ _T("ui_btn_key_lalt"),NULL,VK_MENU,TRUE },			//Alt
	{ _T("ui_btn_key_ralt"),NULL,VK_MENU,TRUE },			//Alt
	{ _T("ui_btn_key_lctrl"),NULL,VK_CONTROL ,TRUE },		//Ctrl
	{ _T("ui_btn_key_rctrl"),NULL,VK_CONTROL ,TRUE },		//Ctrl
	{ _T("ui_btn_key_rshift"),NULL,VK_SHIFT ,TRUE },		//Shift
	{ _T("ui_btn_key_lshift"),NULL,VK_SHIFT ,TRUE },		//Shift
//	{ _T("ui_btn_key_fn"),NULL,5 },							//Fn
	{ _T("ui_btn_key_lwin"),NULL,VK_LWIN,TRUE },			//window
};

static NormalButtonID normalButtonIDs[] =
{
	{ _T("ui_btn_numpad0"),NULL,VK_NUMPAD0,FALSE },
	{ _T("ui_btn_numpad1"),NULL,VK_NUMPAD1,FALSE },
	{ _T("ui_btn_numpad2"),NULL,VK_NUMPAD2,FALSE },
	{ _T("ui_btn_numpad3"),NULL,VK_NUMPAD3,FALSE },
	{ _T("ui_btn_numpad4"),NULL,VK_NUMPAD4,FALSE },
	{ _T("ui_btn_numpad5"),NULL,VK_NUMPAD5,FALSE },
	{ _T("ui_btn_numpad6"),NULL,VK_NUMPAD6,FALSE },
	{ _T("ui_btn_numpad7"),NULL,VK_NUMPAD7,FALSE },
	{ _T("ui_btn_numpad8"),NULL,VK_NUMPAD8,FALSE },
	{ _T("ui_btn_numpad9"),NULL,VK_NUMPAD9,FALSE },

	{ _T("ui_btn_key_multiply"),NULL,VK_MULTIPLY,FALSE},
	{ _T("ui_btn_key_add"),NULL,VK_ADD,FALSE},
	{ _T("ui_btn_key_separator"),NULL,VK_SEPARATOR,FALSE},//小键盘 Enter
	{ _T("ui_btn_key_subtract"),NULL,VK_SUBTRACT,FALSE},
	{ _T("ui_btn_key_decimal"),NULL,VK_DECIMAL,FALSE},
	{ _T("ui_btn_key_divide"),NULL,VK_DIVIDE,FALSE},

	{ _T("ui_btn_key_1"),NULL,'1',FALSE },
	{ _T("ui_btn_key_2"),NULL,'2',FALSE },
	{ _T("ui_btn_key_3"),NULL,'3',FALSE },
	{ _T("ui_btn_key_4"),NULL,'4',FALSE },
	{ _T("ui_btn_key_5"),NULL,'5',FALSE },
	{ _T("ui_btn_key_6"),NULL,'6',FALSE },
	{ _T("ui_btn_key_7"),NULL,'7',FALSE },
	{ _T("ui_btn_key_8"),NULL,'8',FALSE },
	{ _T("ui_btn_key_9"),NULL,'9',FALSE },
	{ _T("ui_btn_key_0"),NULL,'0',FALSE },
	{ _T("ui_btn_key_a"),NULL,'A',FALSE },
	{ _T("ui_btn_key_b"),NULL,'B',FALSE },
	{ _T("ui_btn_key_c"),NULL,'C',FALSE },
	{ _T("ui_btn_key_d"),NULL,'D',FALSE },
	{ _T("ui_btn_key_e"),NULL,'E',FALSE },
	{ _T("ui_btn_key_f"),NULL,'F',FALSE },
	{ _T("ui_btn_key_g"),NULL,'G',FALSE },
	{ _T("ui_btn_key_h"),NULL,'H',FALSE },
	{ _T("ui_btn_key_i"),NULL,'I',FALSE },
	{ _T("ui_btn_key_j"),NULL,'J',FALSE },
	{ _T("ui_btn_key_k"),NULL,'K',FALSE },
	{ _T("ui_btn_key_l"),NULL,'L',FALSE },
	{ _T("ui_btn_key_m"),NULL,'M',FALSE },
	{ _T("ui_btn_key_n"),NULL,'N',FALSE },
	{ _T("ui_btn_key_o"),NULL,'O',FALSE },
	{ _T("ui_btn_key_p"),NULL,'P',FALSE },
	{ _T("ui_btn_key_q"),NULL,'Q',FALSE },
	{ _T("ui_btn_key_r"),NULL,'R',FALSE },
	{ _T("ui_btn_key_s"),NULL,'S',FALSE },
	{ _T("ui_btn_key_t"),NULL,'T',FALSE },
	{ _T("ui_btn_key_u"),NULL,'U',FALSE },
	{ _T("ui_btn_key_v"),NULL,'V',FALSE },
	{ _T("ui_btn_key_w"),NULL,'W',FALSE },
	{ _T("ui_btn_key_x"),NULL,'X',FALSE },
	{ _T("ui_btn_key_y"),NULL,'Y',FALSE },
	{ _T("ui_btn_key_z"),NULL,'Z',FALSE },
	{ _T("ui_btn_key_delete"),NULL,VK_DELETE,FALSE },
	{ _T("ui_btn_key_tab"),NULL,VK_TAB,FALSE },
	{ _T("ui_btn_key_enter"),NULL,VK_RETURN,FALSE },
	{ _T("ui_btn_key_backspace"),NULL,VK_BACK,FALSE },
	{ _T("ui_btn_key_oem_minus"),NULL,VK_OEM_MINUS,FALSE },  // '-' any country
	{ _T("ui_btn_key_oem_plus"),NULL,VK_OEM_PLUS,FALSE },    // '+' any country
	{ _T("ui_btn_key_oem_1"),NULL,VK_OEM_1,FALSE },         // ';:' for US
	{ _T("ui_btn_key_oem_2"),NULL,VK_OEM_2,FALSE }, // '/?' for US
	{ _T("ui_btn_key_oem_3"),NULL,VK_OEM_3,FALSE },// '`~' for US
	{ _T("ui_btn_key_oem_comma"),NULL,VK_OEM_COMMA,FALSE },  // ',' any country
	{ _T("ui_btn_key_oem_period"),NULL,VK_OEM_PERIOD,FALSE },  // '.' any country

	{ _T("ui_btn_key_oem_4"),NULL,VK_OEM_4,FALSE },// '[{' for US
	{ _T("ui_btn_key_oem_5"),NULL,VK_OEM_5,FALSE },// '\|' for US
	{ _T("ui_btn_key_oem_6"),NULL,VK_OEM_6,FALSE },// ']}' for US
	{ _T("ui_btn_key_oem_7"),NULL,VK_OEM_7,FALSE },// ''"' for US
	{ _T("ui_btn_key_space"),NULL,VK_SPACE,FALSE },
	
	//{ _T(""),NULL,,FALSE },
};

LPCTSTR CDYVirtualKeyBoardFrameWnd::GetWindowClassName() const
{
	return kszWindowClassName;
}

void  CDYVirtualKeyBoardFrameWnd::InitWindow()
{
	//TODO init button type ,is it ctrl button

	for (int idx = 0; idx < sizeof(normalButtonIDs) / sizeof(normalButtonIDs[0]); idx++)
	{
		normalButtonIDs[idx].pCtrlUIPointer = FindControl(normalButtonIDs[idx].szCtrlID);

	}


	for (int idx = 0; idx < sizeof(controlKeyOptionIDs) / sizeof(controlKeyOptionIDs[0]); idx++)
	{
		controlKeyOptionIDs[idx].pCtrlUIPointer = FindControl(controlKeyOptionIDs[idx].szCtrlID);

	}

	//Comctl32.lib
	INITCOMMONCONTROLSEX initcomex;
	initcomex.dwSize = sizeof(initcomex);
	initcomex.dwICC = ICC_BAR_CLASSES;
	::InitCommonControlsEx(&initcomex);

	
	hTooltip_ = ::CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_BALLOON, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, GetSafeHwnd(), NULL, GetPaintManager()->GetInstance(), NULL);

	toolInfo_.cbSize = sizeof(TOOLINFO);
	toolInfo_.hwnd = GetSafeHwnd();
	//toolInfo_.uId =0x222222;
	toolInfo_.uFlags = TTF_IDISHWND;//TTF_IDISHWND 表示uId成员是工具的窗口句柄。如果没有设置这个标志，uid是该工具的标识符。
	toolInfo_.lpszText = _T("将焦点集中到您想键入的应用程序窗口");
	toolInfo_.hinst = GetPaintManager()->GetInstance();
	::SendMessage(hTooltip_, TTM_ADDTOOL, 0, (LPARAM)&toolInfo_);

	SetCtrlKeyandLightKeyState();
	//在主框架上绑定定时器
	CControlUI* pFrame = dynamic_cast<CControlUI*>(FindControl(_T("ui_mainframe")));
	if (pFrame)
	{
		GetPaintManager()->SetTimer(pFrame, E_REFLESHKEYSTATE/*nid*/, 200/*time ms*/);//定时刷新用
	}


}


CDYVirtualKeyBoardFrameWnd::CDYVirtualKeyBoardFrameWnd()
{
	hTooltip_ = NULL;
};

LRESULT CDYVirtualKeyBoardFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;
	return TRUE;
}


void CDYVirtualKeyBoardFrameWnd::Notify(TNotifyUI& msg)
{
	CDuiString strName = msg.pSender->GetName();

	if (msg.sType == DUI_MSGTYPE_TIMER)
	{

		if (strName == _T("ui_mainframe"))
		{
			if (msg.wParam == E_TOOLTIPTIMER)
			{
				//隐藏tooltip提示
				::SendMessage(hTooltip_, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&toolInfo_);
				//关闭定时器
				GetPaintManager()->KillTimer(msg.pSender, E_TOOLTIPTIMER);
			}
			else if (msg.wParam == E_REFLESHKEYSTATE)
			{
				SetCtrlKeyandLightKeyState();
			}
		}
	}
	else if (msg.sType==DUI_MSGTYPE_CLICK)
	{
		if (strName==_T("ui_btn_hide"))
		{
			ShowWindow(FALSE);
		}
	}
	__super::Notify(msg);
}

//************************************
// Method:    ProcessButtonDown
// FullName:  CDYVirtualKeyBoardFrameWnd::ProcessButtonDown
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CControlUI * pControl：当前点击按钮的指针
// Parameter: BOOL isButtonDown 控件按钮是否被按下 ture为按下false为抬起
//************************************
void CDYVirtualKeyBoardFrameWnd::ProcessButtonDown(CControlUI* pControl, BOOL isButtonDown)
{
	if (pControl)
	{

		if (_tcscmp(pControl->GetClass(), _T("ButtonUI")) == 0)
		{

			for (int idx = 0; idx < sizeof(normalButtonIDs) / sizeof(normalButtonIDs[0]); idx++)
			{
				if (pControl == normalButtonIDs[idx].pCtrlUIPointer)
				{
					//模拟按键按下
					int nCurVirtualKeyValue = normalButtonIDs[idx].nVirtualKeyValue;

					if (isButtonDown)
					{
						keybd_event(nCurVirtualKeyValue, 0, 0, 0);
					}
					else
					{
						keybd_event(nCurVirtualKeyValue, 0, KEYEVENTF_KEYUP, 0);
					}

					//VK_0 thru VK_9(0x30 - 0x39)  数字键
					//VK_A thru VK_Z(0x41 - 0x5a)  字母键

					//if ((nCurVirtualKeyValue > 0x2F && nCurVirtualKeyValue < 0x3A) || (nCurVirtualKeyValue > 0x40 && nCurVirtualKeyValue < 0x5B))
					{
						keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
						keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
						keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
						keybd_event(5, 0, KEYEVENTF_KEYUP, 0);
						SetCtrlKeyandLightKeyState();
					}

				}
			}

		}
		else if (_tcscmp(pControl->GetClass(), _T("OptionUI")) == 0)
		{

			if (isButtonDown != FALSE)
			{
				for (int idx = 0; idx < sizeof(controlKeyOptionIDs) / sizeof(controlKeyOptionIDs[0]); idx++)
				{
					if (pControl == controlKeyOptionIDs[idx].pCtrlUIPointer)
					{

						SHORT sKeyState = 0;

						if (controlKeyOptionIDs[idx].bIsCtrlKey == FALSE) //lock 状态锁 
						{
							keybd_event(controlKeyOptionIDs[idx].nVirtualKeyValue, 0, 0, 0);
							keybd_event(controlKeyOptionIDs[idx].nVirtualKeyValue, 0, KEYEVENTF_KEYUP, 0);
						}
						else //ctrl 功能键 
						{
							sKeyState = GetKeyState(controlKeyOptionIDs[idx].nVirtualKeyValue);
							if (sKeyState & 0x8000)//键是DOWN状态
							{

								keybd_event(controlKeyOptionIDs[idx].nVirtualKeyValue, 0, KEYEVENTF_KEYUP, 0);
							}
							else
							{
								keybd_event(controlKeyOptionIDs[idx].nVirtualKeyValue, 0, 0, 0);
							}
						}


					}
				}
			}

		}

	}

}



// VkKeyScan(__in CHAR ch) 根据字符得到虚拟键盘码 最高位是虚拟键盘膜，低位是控制键的状态


LRESULT CDYVirtualKeyBoardFrameWnd::OnLButtonDown(UINT uMsg/*uMsg*/, WPARAM wParam /*wParam*/, LPARAM lParam /*lParam*/, BOOL& bHandled)
{
	POINT ptClient = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };


	if (GetForegroundWindow() == GetSafeHwnd())
	{
		POINT ptScreen = { ptClient.x, ptClient.y };
		::ClientToScreen(m_hWnd, &ptScreen);
		::SendMessage(hTooltip_, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(ptScreen.x + 10, ptScreen.y - 20));
		::SendMessage(hTooltip_, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&toolInfo_);

		//在主框架上绑定定时器
		CControlUI* pFrame = dynamic_cast<CControlUI*>(FindControl(_T("ui_mainframe")));
		if (pFrame)
		{
			GetPaintManager()->SetTimer(pFrame, E_TOOLTIPTIMER/*nid*/, 1000/*time ms*/);//定时刷新用
			GetPaintManager()->SetTimer(pFrame, E_REFLESHKEYSTATE/*nid*/, 500/*time ms*/);//定时刷新用
		}
	}
	else
	{

		CControlUI* pControl = FindControl(ptClient);
		ProcessButtonDown(pControl, TRUE);
	}

	bHandled = false;
	return 0;
}


#ifndef SUPPORT_TOUCH
LRESULT CDYVirtualKeyBoardFrameWnd::OnLButtonUp(UINT uMsg/*uMsg*/, WPARAM wParam/*wParam*/, LPARAM lParam/*lParam*/, BOOL& bHandled)
#else
LRESULT CDYVirtualKeyBoardFrameWnd::OnPointerUp(UINT uMsg/*uMsg*/, WPARAM wParam /*wParam*/, LPARAM lParam /*lParam*/, BOOL& bHandled)
#endif
{
	POINT ptClient = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI* pControl = FindControl(ptClient);
	ProcessButtonDown(pControl, FALSE);

	GetPaintManager()->ReleaseCapture();
	bHandled = FALSE;
	return 0;
}



void CDYVirtualKeyBoardFrameWnd::SetCtrlKeyandLightKeyState()
{

	SHORT sKeyState = 0;

	for (int idx = 0; idx < sizeof(controlKeyOptionIDs) / sizeof(controlKeyOptionIDs[0]); idx++)
	{
		if (controlKeyOptionIDs[idx].pCtrlUIPointer != NULL)
		{
			if (controlKeyOptionIDs[idx].bIsCtrlKey == FALSE) //lock 
			{

				sKeyState = GetKeyState(controlKeyOptionIDs[idx].nVirtualKeyValue);

				if (sKeyState & 1)//键是DOWN状态
				{
					((COptionUI*)controlKeyOptionIDs[idx].pCtrlUIPointer)->Selected(true);
				}
				else
				{
					((COptionUI*)controlKeyOptionIDs[idx].pCtrlUIPointer)->Selected(false);
				}
			}
			else //ctrl 
			{
				sKeyState = GetKeyState(controlKeyOptionIDs[idx].nVirtualKeyValue);
				if (sKeyState & 0x8000)//键是DOWN状态
				{

					((COptionUI*)controlKeyOptionIDs[idx].pCtrlUIPointer)->Selected(true);
				}
				else
				{
					((COptionUI*)controlKeyOptionIDs[idx].pCtrlUIPointer)->Selected(false);
				}
			}
		}
	}

}



CControlUI* CDYVirtualKeyBoardFrameWnd::CreateControl(LPCTSTR pstrClass)
{


	return NULL;
}


void CDYVirtualKeyBoardFrameWnd::OnFinalMessage(HWND hWnd)
{

}

LRESULT CDYVirtualKeyBoardFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//屏蔽双击放大
	if (WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}