#include "StdAfx.h"
#include "SetOptionsPg.h"

CSetOptionsPg::CSetOptionsPg(void)
{
	m_bChanged = FALSE;
}

CSetOptionsPg::~CSetOptionsPg(void)
{
}

//void 
//CSetOptionsPg::EventChkDescChanged()
//{
//	CButton chk;
//	chk.Attach(GetDlgItem(IDC_CHKDESC));
//	BOOL enabled = chk.GetCheck() > 0;
//	chk.Attach(GetDlgItem(IDC_CHKFPS));
//	chk.EnableWindow(enabled);
//	chk.Attach(GetDlgItem(IDC_CHK_TT_TEXT));
//	chk.EnableWindow(enabled);
//  chk.Detach();
//}


LRESULT
CSetOptionsPg::OnClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bChanged = TRUE;
	//if(wID == IDC_CHKDESC)
	//	EventChkDescChanged();
	SetModified(1);
	return 0;
}

LRESULT 
CSetOptionsPg::OnTextChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bChanged = TRUE;
	SetModified(1);
	return 0;
}

BOOL
CSetOptionsPg::OnApply()
{
	if(!m_pPrefs)
		return 1;
	
	if(m_bChanged)
	{
		if(!DoDataExchange(DDX_SAVE))
		{
			DisplayError();
			return 1;
		}
		m_pPrefs->BoxesCountInCircle = m_numBoxes;
		m_pPrefs->ForceOfBall = m_forceOfBall;
		m_bChanged = FALSE;
		SetModified(0);
	}
	return 1;
}

BOOL
CSetOptionsPg::OnKillActive()
{
	if(!DoDataExchange(DDX_SAVE))
	{
		DisplayError();
		return FALSE;
	}
	else
		return TRUE;
}

void
CSetOptionsPg::DisplayError()
{
	::Beep(440, 100); ::Beep(600, 50);
	MessageBox(L"You've entered incorrect value!", L"Error", MB_OK|MB_ICONERROR);
}


LRESULT
CSetOptionsPg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_numBoxes = m_pPrefs->BoxesCountInCircle;
	m_forceOfBall = m_pPrefs->ForceOfBall;
	DoDataExchange(DDX_LOAD);
	//EventChkDescChanged();
	return 0;
}
