#include "stdafx.h"
#include "WinImplBase.h"

//#include <winuser.h>
#include <windows.h>

namespace DuiLib
{

	LPBYTE WindowImplBase::m_lpResourceZIPBuffer=NULL;

	DUI_BEGIN_MESSAGE_MAP(WindowImplBase,CNotifyPump)
		DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
		DUI_END_MESSAGE_MAP()

	WindowImplBase::WindowImplBase()
	{
		m_dwWindowPosState = SIZE_MINIMIZED;
	};
	WindowImplBase::~WindowImplBase()
	{

	};
	void WindowImplBase::OnFinalMessage( HWND hWnd )
	{
		m_PaintManager.RemovePreMessageFilter(this);
		m_PaintManager.RemoveNotifier(this);
		m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
	}

	LRESULT WindowImplBase::ResponseDefaultKeyEvent(WPARAM wParam)
	{
		if (wParam == VK_RETURN)
		{
			return FALSE;
		}
		else if (wParam == VK_ESCAPE)
		{
			Close();
			return TRUE;
		}

		return FALSE;
	}

	UINT WindowImplBase::GetClassStyle() const
	{
		return CS_DBLCLKS;
	}

	UILIB_RESOURCETYPE WindowImplBase::GetResourceType() const
	{
		return UILIB_FILE;
	}

	CDuiString WindowImplBase::GetZIPFileName() const
	{
		return _T("");
	}

	LPCTSTR WindowImplBase::GetResourceID() const
	{
		return _T("");
	}

	CControlUI* WindowImplBase::CreateControl(LPCTSTR pstrClass)
	{
		return NULL;
	}
	CPaintManagerUI* WindowImplBase::GetPaintManager()
	{
		return &m_PaintManager;
	}

	CControlUI* WindowImplBase::FindControl(POINT pt)
	{
		return m_PaintManager.FindControl(pt);
	}

	CControlUI* WindowImplBase::FindControl(LPCTSTR pstrName)
	{
		return m_PaintManager.FindControl(pstrName);
	}

	LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				bHandled = !!ResponseDefaultKeyEvent(wParam); //�޸���һ��ESC���¶�����ڹرյ�����
				return 0;
			default:
				break;
			}
		}
		return FALSE;
	}

	LRESULT WindowImplBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

#if defined(WIN32) && !defined(UNDER_CE)
	LRESULT WindowImplBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if( ::IsIconic(*this) ) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT WindowImplBase::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPRECT pRect=NULL;

		if ( wParam == TRUE)
		{
			LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
			pRect=&pParam->rgrc[0];
		}
		else
		{
			pRect=(LPRECT)lParam;
		}

		if ( ::IsZoomed(m_hWnd))
		{	// ���ʱ�����㵱ǰ��ʾ�����ʺϿ�߶�
			MONITORINFO oMonitor = {};
			oMonitor.cbSize = sizeof(oMonitor);
			::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
			CDuiRect rcWork = oMonitor.rcWork;
			CDuiRect rcMonitor = oMonitor.rcMonitor;
			rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
			
			pRect->top = pRect->left = 0;
			pRect->right = pRect->left + rcWork.GetWidth();
			pRect->bottom = pRect->top + rcWork.GetHeight();
			//���ʱ�޶���󴰿ڵķ�Χ
			SIZE sz = m_PaintManager.GetMaxInfo();
			if(sz.cx != 0 && sz.cy!= 0)
			{
				pRect->right = pRect->right>sz.cx?sz.cx:pRect->right;
				pRect->bottom = pRect->bottom>sz.cy?sz.cy:pRect->bottom;
			}

			return WVR_REDRAW;
		}

		return 0;
	}

	LRESULT WindowImplBase::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT WindowImplBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		if( !::IsZoomed(*this) )
		{
			RECT rcSizeBox = m_PaintManager.GetSizeBox();
			if( pt.y < rcClient.top + rcSizeBox.top )
			{
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
				return HTTOP;
			}
			else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
			{
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}

			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
		}

		RECT rcCaption = m_PaintManager.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
				//�����˵Ŀؼ����ᱻduilib�����Ƿǿͻ�����һ����
				if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcsicmp(pControl->GetClass(), _T("TextUI"))   != 0 &&  
					_tcsicmp(pControl->GetClass(), _T("SliderUI")) != 0 &&  
					_tcsicmp(pControl->GetClass(), _T("EditUI"))   != 0)
				{
					return HTCAPTION;
				}
		}

		return HTCLIENT;
	}

	LRESULT WindowImplBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		CDuiRect rcMonitor = oMonitor.rcMonitor;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		// �������ʱ����ȷ��ԭ������
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;

		lpMMI->ptMaxTrackSize.x =rcWork.GetWidth();
		lpMMI->ptMaxTrackSize.y =rcWork.GetHeight();

		lpMMI->ptMinTrackSize.x =m_PaintManager.GetMinInfo().cx;
		lpMMI->ptMinTrackSize.y =m_PaintManager.GetMinInfo().cy;

		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnMouseWheel(UINT /*uMsg*/,UINT fwKeys,int ndelta,CPoint point,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
#endif

	LRESULT WindowImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
		if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
#endif
		// �л���󻯰�ť�ͻ�ԭ��ť��״̬
		if (wParam != m_dwWindowPosState && (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED) && GetPaintManager()->GetRoot())
		{
			CControlUI* pbtnMax = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // ��󻯰�ť
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // ��ԭ��ť
			if (pbtnMax && pbtnRestore){
				if (wParam == SIZE_MAXIMIZED){
					pbtnMax->SetVisible(false);
					pbtnRestore->SetVisible(true);
				}
				else{
					pbtnMax->SetVisible(true);
					pbtnRestore->SetVisible(false);
				}
			}
		}
		//m_dwWindowPosState��������һ�εĴ���״̬, ֻ�е�״̬��ͬʱ�Ż��л���ť.
		m_dwWindowPosState = wParam;


		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_CLOSE)
		{
			bHandled = TRUE;
			SendMessage(WM_CLOSE);
			return 0;
		}
#if defined(WIN32) && !defined(UNDER_CE)
		//BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		/*
		if( ::IsZoomed(*this) != bZoomed )
		{
			CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));       // ��󻯰�ť
			CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));   // ��ԭ��ť

			// �л���󻯰�ť�ͻ�ԭ��ť��״̬
			if (pbtnMax && pbtnRestore)
			{
				pbtnMax->SetVisible(TRUE == bZoomed);       // �˴��ñ��ʽ��Ϊ�˱��������BOOLת���ľ���
				pbtnRestore->SetVisible(FALSE == bZoomed);
			}

		}
		*/
#else
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
		return lRes;
	}

	LRESULT WindowImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
			rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AddPreMessageFilter(this);

		CDialogBuilder builder;
		WIN32_FIND_DATA FindFileData;
		HANDLE hDir;

		if (m_PaintManager.GetResourcePath().IsEmpty())
		{	// �����������Դ·������
			CDuiString strResourcePath=m_PaintManager.GetInstancePath();
			strResourcePath+=GetSkinFolder().GetData();


			hDir = FindFirstFile(strResourcePath.GetData(), &FindFileData);
			if (hDir == INVALID_HANDLE_VALUE) 
			{
				//assert(0&&"Invalid skin path \n"); //��Դ·����Ч
			} else
			{
				m_PaintManager.SetResourcePath(strResourcePath.GetData());
				FindClose(hDir);
			}
		}

		switch(GetResourceType())
		{
		case UILIB_ZIP:
			m_PaintManager.SetCompressedPacketResource(GetZIPFileName().GetData(), true);
			break;
		case UILIB_ZIPRESOURCE:
			{
				HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
				if( hResource == NULL )
					return 0L;
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
				if( hGlobal == NULL ) 
				{
#if defined(WIN32) && !defined(UNDER_CE)
					::FreeResource(hResource);
#endif
					return 0L;
				}
				dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
				if( dwSize == 0 )
					return 0L;
				m_lpResourceZIPBuffer = new BYTE[ dwSize ];
				if (m_lpResourceZIPBuffer != NULL)
				{
					::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
				}
#if defined(WIN32) && !defined(UNDER_CE)
				::FreeResource(hResource);
#endif
				m_PaintManager.SetCompressedPacketResource(m_lpResourceZIPBuffer, dwSize);
			}
			break;
		}

		CControlUI* pRoot=NULL;
		if (GetResourceType()==UILIB_RESOURCE)
		{
			STRINGorID xml(_ttoi(GetSkinFile().GetData()));
			pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
		}
		else
			pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
		ASSERT(pRoot&&"Load Resouse fail ,check fold and path ,or err in file");
		if (pRoot==NULL)
		{
			MessageBox(NULL,_T("������Դ�ļ�ʧ��"),_T("Duilib"),MB_OK|MB_ICONERROR);
			ExitProcess(1);
			return 0;
		}
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		m_PaintManager.SetBackgroundTransparent(TRUE);

		InitWindow();
		return 0;
	}

	LRESULT WindowImplBase::OnKeyDown( UINT /*uMsg*/,UINT nChar, UINT nRepCnt, UINT nFlags,BOOL& bHandled )
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT WindowImplBase::OnKeyUp(UINT /*uMsg*/, UINT nChar, UINT nRepCnt, UINT nFlags ,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnSysKeyDown( UINT /*uMsg*/,UINT nChar, UINT nRepCnt, UINT nFlags,BOOL& bHandled )
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT WindowImplBase::OnSysKeyUp(UINT /*uMsg*/, UINT nChar, UINT nRepCnt, UINT nFlags ,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT WindowImplBase::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT WindowImplBase::OnRButtonDown(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT WindowImplBase::OnRButtonUp(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	LRESULT WindowImplBase::OnMouseMove(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

    LRESULT WindowImplBase::OnPointerDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }
    LRESULT WindowImplBase::OnPointerUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }


#if(WINVER >= 0x0601)
	LRESULT WindowImplBase::OnTouch(UINT /*uMsg*/, UINT cInputs,HTOUCHINPUT hTouchInput,BOOL& bHandled)
	{
		//ʹ��ʾ��
		/*
		PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
		if (NULL != pInputs)
		{
		if (GetTouchInputInfo(hTouchInput,cInputs,pInputs,sizeof(TOUCHINPUT)))
		{
		// process pInputs
		if (!CloseTouchInputHandle(hTouchInput))
		{
		// error handling
		}

		bHandled = TRUE;

		}
		else
		{
		// GetLastError() and error handling
		}

		delete[] pInputs;
		}
		else
		{
		// error handling, presumably out of memory
		}
		*/

		bHandled = FALSE;
		return 0;
	}

#endif

	LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		CPoint pt;
		switch (uMsg)
		{
		case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
		case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_MOUSEWHEEL:	
			{
				pt.x = GET_X_LPARAM( lParam );
				pt.y = GET_Y_LPARAM( lParam );
				lRes = OnMouseWheel(uMsg,GET_KEYSTATE_WPARAM(wParam),GET_WHEEL_DELTA_WPARAM(wParam),pt,bHandled);
				break;
			}
#endif
		case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_CHAR:		    lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN:
			{
				lRes = OnKeyDown( uMsg,wParam, lParam&0xff, lParam>>16 ,bHandled);
				break;
			}
		case WM_KEYUP:
			{
				lRes = OnKeyUp(uMsg, wParam, lParam&0xff, lParam>>16,bHandled );
				break;	
			}
		case WM_SYSKEYDOWN:
			{
				lRes=OnSysKeyDown(uMsg,wParam,lParam&0xff,lParam>>16,bHandled);
				break;
			}
		case WM_SYSKEYUP:
			{
				lRes=OnSysKeyUp(uMsg,wParam,lParam&0xff,lParam>>16,bHandled);
				break;
			}
		case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
		case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
		case WM_RBUTTONDOWN:
			{
				lRes = OnRButtonDown(uMsg,wParam,lParam,bHandled);
				break;
			}
		case WM_RBUTTONUP:
			{
				lRes = OnRButtonUp(uMsg,wParam,lParam,bHandled);
				break;
			}
		case WM_MOUSEMOVE:	
			{
				pt.x = GET_X_LPARAM( lParam );
				pt.y = GET_Y_LPARAM( lParam );
				lRes = OnMouseMove(uMsg, wParam, pt, bHandled);
				break;
			}
		case WM_MOUSEHOVER:
			{
				lRes = OnMouseHover(uMsg, wParam, lParam, bHandled);
				break;
			}
#if(WINVER >= 0x0601)
		case WM_TOUCH:
			{
				UINT cInputs = LOWORD(wParam);
				HTOUCHINPUT hTouchInput=(HTOUCHINPUT)lParam;
				lRes = OnTouch(uMsg, cInputs, hTouchInput, bHandled);
				//If the application does not process the message, it must call DefWindowProc
				if (lRes==FALSE)
				{
					::DefWindowProc(*this,uMsg,wParam,lParam);
				}
				break;
			}
#endif
#if(WINVER >= 0x0602)
        case WM_POINTERUP:
            {
                lRes = OnPointerUp(uMsg,wParam,lParam,bHandled);
                break;

            }
		case WM_POINTERDOWN:
			{
                lRes = OnPointerDown(uMsg,wParam,lParam,bHandled);
                break;
			}
#endif
		default:
			{
				bHandled = FALSE; break;
			}
		}
		if (bHandled)
		{
			return lRes;
		}
		lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);

		if(bHandled)
		{
			return lRes;
		}
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	LRESULT WindowImplBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LONG WindowImplBase::GetStyle()
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;

		return styleValue;
	}

	void WindowImplBase::OnClick(TNotifyUI& msg)
	{
		CDuiString sCtrlName = msg.pSender->GetName();
		if( sCtrlName == _T("closebtn") )
		{
			Close();
			return; 
		}
		else if (sCtrlName == _T("minbtn"))
		{
			::ShowWindow(GetHWND(), SW_MINIMIZE);
			return;
		}
		else if (sCtrlName == _T("maxbtn"))
		{
			::ShowWindow(GetHWND(), SW_MAXIMIZE);
			return;
		}
		else if (sCtrlName == _T("restorebtn"))
		{
			::ShowWindow(GetHWND(), SW_RESTORE);
			return;
		}

		return;
	}



	/*
	duilib�ο���MFC��ATL/WTL����Ϣ���ƣ�
	1��DUI_BEGIN_MESSAGE_MAP������MFC��BEGIN_MESSAGE_MAP��
	2��HandleMessage������MFC��DefWindowProc��
	������Ҫע�����bHandled�����������ģ��ATL�ġ�bHandled��ATL�е��������£�
	����Ϣ��Ӧ��������ǰ��ATL����ΪTRUE������������Ϣ��Ӧ������֮����ҪATL����Ĭ�ϵ�WindowProc()�������Ϣ��
	����Խ�bHandled����ΪFALSE������MFC��ͬ�� MFC����ʽ�ص��û������Ӧ������ʵ�ֵ�Ĭ�ϵ���Ϣ����ġ�
	3������duilib���ṩ������һ����������Notify������Notify���ڴ���duilib�Լ������һ����Ϣ������Ϣ������μ�duilib��UIDefine.h����SendNotify������
	4:Ϊ�����ؼ������Ϣ����ϢΪNotify�õ���һ�ף���SendNotify����
	*/
	void WindowImplBase::Notify(TNotifyUI& msg)
	{
		return CNotifyPump::NotifyPump(msg);
	}

    int WindowImplBase::DuiMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
    {
        CDuiString strText=lpText;
        CDuiString strCaption=lpCaption;
        m_PaintManager.PaserString(strText);
        m_PaintManager.PaserString(strCaption);
        return MessageBox(hWnd,strText.GetData(),strCaption.GetData(),uType);
    }

}

