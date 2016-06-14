/*
�������ڣ�	2012/11/05 15:09:48
���ߣ�			daviyang35@gmail.com
������	FlashUI
*/
#ifndef __UIFLASH_H__
#define __UIFLASH_H__
#pragma once
// \Utils\Flash11.tlb ΪFlash11�ӿ��ļ������ַ����ڵͰ汾�����ڣ�ʹ����ע��
//#import "..\Utils/Flash11.tlb" raw_interfaces_only, named_guids
//Ϊ���ų�����IDispatchEx��: �ض��壻��ͬ�Ļ����ͣ�����IDispatchEx��: �ض��壻��ͬ�Ļ����͡����ȴ������ǽ���һ����ø�Ϊ��
#import "..\Utils/Flash11.tlb" raw_interfaces_only, named_guids, rename("IDispatchEx","IMyDispatchEx")  



using namespace ShockwaveFlashObjects;
#include "../Utils/FlashEventHandler.h"
class CActiveXCtrl;

namespace DuiLib
{
	class UILIB_API CFlashUI
		: public CActiveXUI
		//, public IOleInPlaceSiteWindowless // ͸��ģʽ��ͼ����Ҫʵ������ӿ�
		, public _IShockwaveFlashEvents
		, public ITranslateAccelerator
	{
	public:
		CFlashUI(void);
		~CFlashUI(void);

		void SetFlashEventHandler(CFlashEventHandler* pHandler);
		virtual bool DoCreateControl();
		IShockwaveFlash* m_pFlash;

	private:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface( LPCTSTR pstrName );
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( __RPC__out UINT *pctinfo );
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo );
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);
		virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr );

		virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void **ppvObject );
		virtual ULONG STDMETHODCALLTYPE AddRef( void );
		virtual ULONG STDMETHODCALLTYPE Release( void );

		HRESULT OnReadyStateChange (long newState);
		HRESULT OnProgress(long percentDone );
		HRESULT FSCommand (_bstr_t command, _bstr_t args);
		HRESULT FlashCall (_bstr_t request );

		virtual void ReleaseControl();
		HRESULT RegisterEventHandler(BOOL inAdvise);

		// ITranslateAccelerator
		// Duilib��Ϣ�ַ���WebBrowser
		virtual LRESULT TranslateAccelerator( MSG *pMsg );

	private:
		LONG m_dwRef;
		DWORD m_dwCookie;
		CFlashEventHandler* m_pFlashEventHandler;
	};
}

#endif // __UIFLASH_H__
