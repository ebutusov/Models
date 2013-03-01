#pragma once
#include "stdafx.h"
#include "resource.h"
#include "SimController.h"
#include "SetOptionsPg.h"

class CSettingsSheet : public CPropertySheetImpl<CSettingsSheet>
{
public:
	CSettingsSheet(ATL::_U_STRINGorID title = (LPCTSTR)NULL, 
		UINT uStartPage = 0, HWND hWndParent = NULL);
	virtual ~CSettingsSheet(void);
	//LRESULT OnSaveData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	int DoModal(HWND parent);

	BEGIN_MSG_MAP(CSettingsSheet)
		//MESSAGE_HANDLER(WM_PSSAVEDATA, OnSaveData)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(ID_APPLY_NOW, OnApply)

		CHAIN_MSG_MAP(CPropertySheetImpl<CSettingsSheet>)
	END_MSG_MAP()

	void InitPages(SSettings *settings);

	CSetOptionsPg m_pgOptions;

private:
	BOOL m_bModal;	
	CPropertyPage<IDD_ABOUTBOX> m_pgAbout;
};

