/*********************************************************************
*  ����Ϊduilib�����Ķ�̬��ɫ��ؼ�����ȫģ�¿ṷ��QQ�ĵ�ɫ�幦��
*  ʹ����HSB����ģʽ��������Լ��о�����һ���㷨���ƽ���
*  �����ɫ����Դ���ģ�³��ṷ��ɫ�����ʽ�����ǻ��в��
*  �������λ����֪�����õ��㷨���������ƵĴ��룬���ߴ˴�����bug������ϵ��
*  By:Redrain  QQ��491646717   2014.8.19
*  ��������Ƶ����󣬿����޸������
*  thumbimage����ָ��ѡ����ɫ�Ĺ���زĵ�λ�ã��ز��Ҹ��ӵ���ѹ�����У����Լ��޸�
*  sample:<ColorPalette name="Pallet" width="506" height="220" palletheight="200" barheight="14" padding="8,5,0,0" bkcolor="#FFFFFFFF" thumbimage="UI\skin\cursor.png" />
*
*  ��ģ��������ԭ���Ĵ����������ѡ���֮�������±�д�ģ��޸����㷨����������˶�̬��ɫ������ܣ�֮�����ֽ�����һ��bug���ٴθ�л��֮����Ĵ���
*********************************************************************/


#ifndef UI_PALLET_H
#define UI_PALLET_H
#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	//const TCHAR kPalletClassName[] = _T("ColorPaletteUI");
	//const TCHAR kPalletInterface[] = _T("ColorPalette");

	class UILIB_API CColorPaletteUI : public CControlUI
	{
	public:
		CColorPaletteUI();
		virtual ~CColorPaletteUI();

		//��ȡ���ձ�ѡ�����ɫ������ֱ����������duilib����ɫ
		DWORD GetSelectColor();
		void SetSelectColor(DWORD dwColor);

		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		//����/��ȡ Pallet����ɫ�������棩�ĸ߶�
		void SetPalletHeight(int nHeight);
		int	GetPalletHeight() const;

		//����/��ȡ �·�Bar������ѡ�������ĸ߶�
		void SetBarHeight(int nHeight);
		int GetBarHeight() const;
		//����/��ȡ ѡ��ͼ���·��
		void SetThumbImage(LPCTSTR pszImage);
		LPCTSTR GetThumbImage() const;

		virtual void SetPos(RECT rc);
		virtual void DoInit();
		virtual void DoEvent(TEventUI& event);
		virtual void DoPaint(HDC hDC, const RECT& rcPaint);
		virtual void PaintPallet(HDC hDC);

	protected:
		//��������
		void UpdatePalletData();
		void UpdateBarData();

	private:
		HDC			m_MemDc;
		HBITMAP		m_hMemBitmap;
		BITMAP		m_bmInfo;
		BYTE		*m_pBits;
		UINT		m_uButtonState;
		bool		m_bIsInBar;
		bool		m_bIsInPallet;
		int			m_nCurH;
		int			m_nCurS;
		int			m_nCurB;

		int			m_nPalletHeight;
		int			m_nBarHeight;
		CPoint		m_ptLastPalletMouse;
		CPoint		m_ptLastBarMouse;
		CDuiString  m_strThumbImage;
	};


}

#endif // UI_PALLET_H