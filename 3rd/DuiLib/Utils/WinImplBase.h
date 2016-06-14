#ifndef WIN_IMPL_BASE_HPP
#define WIN_IMPL_BASE_HPP
#include <sdkddkver.h>

namespace DuiLib
{

	enum UILIB_RESOURCETYPE
	{
		UILIB_FILE=1,				    // ���Դ����ļ�
		UILIB_ZIP,						// ���Դ���zipѹ����
		UILIB_RESOURCE,			        // ������Դ
		UILIB_ZIPRESOURCE,	            // ������Դ��zipѹ����
	};

	class UILIB_API WindowImplBase
		: public CWindowWnd
		, public CNotifyPump
		, public INotifyUI
		, public IMessageFilterUI
		, public IDialogBuilderCallback
	{
	public:
		WindowImplBase();
		virtual ~WindowImplBase();
		virtual void InitWindow(){};
		virtual void OnFinalMessage( HWND hWnd );
		virtual void Notify(TNotifyUI& msg);

		DUI_DECLARE_MESSAGE_MAP()
	    virtual void OnClick(TNotifyUI& msg);

	protected:
		virtual CDuiString GetSkinFolder() = 0;
		virtual CDuiString GetSkinFile() = 0;
		virtual LPCTSTR GetWindowClassName(void) const = 0 ;
		virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

		CPaintManagerUI m_PaintManager;
		static LPBYTE m_lpResourceZIPBuffer;
	public:
		CPaintManagerUI* GetPaintManager();
		CControlUI* FindControl(POINT pt);
		CControlUI* FindControl(LPCTSTR pstrName);

	public:
		virtual UINT GetClassStyle() const;
		virtual UILIB_RESOURCETYPE GetResourceType() const;
		virtual CDuiString GetZIPFileName() const;
		virtual LPCTSTR GetResourceID() const;
		virtual CControlUI* CreateControl(LPCTSTR pstrClass);
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
		virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

#if defined(WIN32) && !defined(UNDER_CE)
		virtual LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnMouseWheel(UINT /*uMsg*/,UINT fwKeys,int ndelta,CPoint point,BOOL& bHandled);
		virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnKeyDown( UINT /*uMsg*/,UINT nChar, UINT nRepCnt, UINT nFlags,BOOL& bHandled );
		virtual LRESULT OnKeyUp(UINT /*uMsg*/, UINT nChar, UINT nRepCnt, UINT nFlags ,BOOL& bHandled);
		virtual LRESULT OnSysKeyDown( UINT /*uMsg*/,UINT nChar, UINT nRepCnt, UINT nFlags,BOOL& bHandled );
		virtual LRESULT OnSysKeyUp(UINT /*uMsg*/, UINT nChar, UINT nRepCnt, UINT nFlags ,BOOL& bHandled);

		virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnRButtonDown(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled);
		virtual LRESULT OnRButtonUp(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled);
		virtual LRESULT OnMouseMove(UINT /*uMsg*/, UINT nFlags, CPoint point ,BOOL& bHandled);

        virtual LRESULT OnPointerDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
        virtual LRESULT OnPointerUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);


#if(WINVER >= 0x0601)
		virtual	LRESULT OnTouch(UINT /*uMsg*/, UINT cInputs,HTOUCHINPUT hTouchInput,BOOL& bHandled);
#endif
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LONG GetStyle();

	protected:
		
		WPARAM	m_dwWindowPosState;
    public:
        int DuiMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);

	};
}

#endif // WIN_IMPL_BASE_HPP
