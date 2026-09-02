#if !defined(AFX_AESDLG_H__7D2A0322_74B1_47F3_98AD_D3F0AFE73D20__INCLUDED_)
#define AFX_AESDLG_H__7D2A0322_74B1_47F3_98AD_D3F0AFE73D20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AESDlg.h : header file
//
#include "CUSBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAESDlg dialog

class CAESDlg : public CDialog
{
	// Construction
public:
	CAESDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAESDlg)
	enum { IDD = IDD_AESDLG };
	CString	m_aes_key;
	CString	m_aes_iv;
	CString	m_aes_plain;
	CString	m_aes_cipher;
	CString	m_aes_output;
	int		m_aes_mode;
	int		m_aes_keymode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAESDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CAESDlg::aes_crypt(CCUSBDlg* pUSBDlg, BYTE operation, BYTE* bCryptIn, DWORD nBytesCryptIn, BYTE* bCryptOut, DWORD* nBytesCryptOut);

	// Generated message map functions
	//{{AFX_MSG(CAESDlg)
	afx_msg void OnAESButtonEnc();
	afx_msg void OnAESButtonDec();
	afx_msg void OnAESButtonEncfile();
	afx_msg void OnAESButtonDecfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AESDLG_H__7D2A0322_74B1_47F3_98AD_D3F0AFE73D20__INCLUDED_)
