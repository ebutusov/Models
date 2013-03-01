#include "StdAfx.h"
#include "SettingsSheet.h"
#include "SetOptionsPg.h"

CSettingsSheet::CSettingsSheet(ATL::_U_STRINGorID title, UINT uStartPage, HWND hWndParent) :
	CPropertySheetImpl<CSettingsSheet>(title, uStartPage, hWndParent)
{
	this->SetTitle(L"Settings");
	m_bModal = FALSE;
}

CSettingsSheet::~CSettingsSheet(void)
{
}

void CSettingsSheet::InitPages(SSettings *settings)
{
	m_pgOptions.m_pPrefs = settings;
	AddPage(m_pgOptions);
	AddPage(m_pgAbout);
}


int
CSettingsSheet::DoModal(HWND parent)
{
	m_bModal = TRUE;
	return CPropertySheetImpl<CSettingsSheet>::DoModal(parent);
}


LRESULT
CSettingsSheet::OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Apply();
	//SaveSettings();
	return 0;
}

//BOOL
//CSettingsSheet::SaveSettings()
//{
//	/*BOOL ok = TRUE;
//	try
//	{
//		CXFile file;
//		file.Open(SETTINGS_FILE,                  
//    GENERIC_WRITE | GENERIC_READ,       
//    FILE_SHARE_READ | FILE_SHARE_WRITE,    
//    NULL,                  
//    CREATE_ALWAYS,              
//    FILE_ATTRIBUTE_NORMAL,          
//    NULL);
//		CXArchive ar(&file, CXArchive::store);
//		m_sPrefs.Serialize(ar);
//		ar.Close();
//	}
//	catch(CXException &e)
//	{
//#ifdef _DEBUG
//		MessageBox(e.GetErrorDesc().c_str(), "Error", MB_OK|MB_ICONERROR);
//#endif
//		ok = FALSE;
//	}*/
//	return m_sPrefs.Save();
//}

//BOOL
//CSettingsSheet::LoadSettings()
//{
//	BOOL ok = TRUE;
////	try
////	{
////		CXFile file;
////		file.Open(SETTINGS_FILE,                  
////    GENERIC_WRITE | GENERIC_READ,       
////    FILE_SHARE_READ | FILE_SHARE_WRITE,    
////    NULL,                  
////    OPEN_EXISTING,              
////    FILE_ATTRIBUTE_NORMAL,          
////    NULL);
////		CXArchive ar(&file, CXArchive::load);
////		m_sPrefs.Serialize(ar);
////		ar.Close();
////	}
////	catch(CXException &e)
////	{
////#ifdef _DEBUG
////		MessageBox(e.GetErrorDesc().c_str(), "Error", MB_OK|MB_ICONERROR);
////#endif
////		ok = FALSE;
////  }
//  m_sPrefs.Load();
//	return ok;
//}