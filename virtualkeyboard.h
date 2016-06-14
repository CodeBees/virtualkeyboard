#pragma once

#include "resource.h"
#include "stdafx.h"



enum TimerID
{
	E_TOOLTIPTIMER = 1,
	E_REFLESHKEYSTATE= 2,
};



class CDYVirtualKeyBoardFrameWnd : public WindowImplBase
{

public:

	CDYVirtualKeyBoardFrameWnd();
	virtual ~CDYVirtualKeyBoardFrameWnd() {};

#ifdef _DEBUG

	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_FILE;
	};

#else

	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIP_SKIN);
	};
	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
	};

#endif

	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile() { return _T("skin.xml"); }
	virtual CDuiString GetSkinFolder() { return _T("Skin"); }
	virtual void  InitWindow();
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void OnFinalMessage(HWND hWnd);


	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
public:

	void SetCtrlKeyandLightKeyState();
	void ProcessButtonDown(CControlUI* pControl, BOOL isButtonDown);
public:
	HWND hTooltip_;
	TOOLINFO toolInfo_;

};