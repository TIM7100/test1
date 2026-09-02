#if !defined(AFX_SM4DLG_H__F4FA6B44_F087_4408_A597_112E328BB86D__INCLUDED_)
#define AFX_SM4DLG_H__F4FA6B44_F087_4408_A597_112E328BB86D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SM4Dlg.h : header file
//
#include "CUSBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSM4Dlg dialog

class CSM4Dlg : public CDialog
{
// Construction
public:
	CSM4Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSM4Dlg)
	enum { IDD = IDD_SM4DLG };
	CString	m_sm4_key;
	CString	m_sm4_iv;
	CString	m_sm4_plain;
	CString	m_sm4_cipher;
	CString	m_sm4_output;
	int		m_sm4_mode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSM4Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CSM4Dlg::sm4_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut);

	// Generated message map functions
	//{{AFX_MSG(CSM4Dlg)
	afx_msg void OnSM4ButtonEnc();
	afx_msg void OnSM4ButtonDec();
	afx_msg void OnSM4ButtonEncFile();
	afx_msg void OnSM4ButtonDecFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SM4DLG_H__F4FA6B44_F087_4408_A597_112E328BB86D__INCLUDED_)
