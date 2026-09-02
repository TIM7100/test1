#if !defined(AFX_SM1DLG_H__D93CEC98_A06C_4903_A089_798D95768191__INCLUDED_)
#define AFX_SM1DLG_H__D93CEC98_A06C_4903_A089_798D95768191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SM1Dlg.h : header file
//
#include "resource.h"
#include "CUSBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSM1Dlg dialog

class CSM1Dlg : public CDialog
{
// Construction
public:
	CSM1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSM1Dlg)
	enum { IDD = IDD_SM1DLG };
	CString	m_sm1_ek;
	CString	m_sm1_ak;
	CString	m_sm1_sk;
	CString	m_sm1_iv;
	CString	m_sm1_plain;
	CString	m_sm1_cipher;
	CString	m_sm1_output;
	int		m_sm1_mode;
	int		m_sm1_skmode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSM1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CSM1Dlg::sm1_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut);

	// Generated message map functions
	//{{AFX_MSG(CSM1Dlg)
	afx_msg void OnSM1ButtonEnc();
	afx_msg void OnSM1ButtonDec();
	afx_msg void OnSM1ButtonEncFile();
	afx_msg void OnSM1ButtonDecFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SM1DLG_H__D93CEC98_A06C_4903_A089_798D95768191__INCLUDED_)
