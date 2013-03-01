#pragma once
#include "stdafx.h"
#include "resource.h"
#include "SimController.h"

class CSetOptionsPg : public CPropertyPageImpl<CSetOptionsPg>,
	public CWinDataExchange<CSetOptionsPg>
{
public:
	CSetOptionsPg(void);
	enum { IDD = IDD_OPTIONS_PG };
	~CSetOptionsPg(void);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTextChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	BOOL OnKillActive();
	BOOL OnApply();

	BEGIN_MSG_MAP(CSetOptionsPg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_RANGE_HANDLER(IDC_CHKFPS, IDC_CHKBUILD, OnClick)
		//COMMAND_HANDLER(IDC_EDIT_NUMBOXES, EN_CHANGE, OnTextChanged)
		COMMAND_RANGE_CODE_HANDLER(IDC_EDIT_NUMBOXES, IDC_EDIT_FORCEOFBALL, EN_CHANGE, OnTextChanged)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetOptionsPg>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetOptionsPg)
		DDX_UINT_RANGE(IDC_EDIT_NUMBOXES, m_numBoxes, 1, 50)
		DDX_FLOAT_RANGE(IDC_EDIT_FORCEOFBALL, m_forceOfBall, 1.0f, 100.0f)
		//DDX_UINT_RANGE(IDC_EDITEXITAFTER, m_exitAfter, 0, 60) // exit rendering mode control
		//DDX_CHECK(IDC_CHKFPS, m_bShowFps)
	END_DDX_MAP()

	SSettings *m_pPrefs;

private:
	int m_numBoxes;
	float m_forceOfBall;
	BOOL m_bChanged;
	void DisplayError();
	//void EventChkDescChanged();
};
