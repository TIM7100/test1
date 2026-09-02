#if !defined(AFX_SSF33DLG_H__309AE248_6113_4765_9988_AD8F3D57E1DE__INCLUDED_)
#define AFX_SSF33DLG_H__309AE248_6113_4765_9988_AD8F3D57E1DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SSF33Dlg.h : header file
//
#include "resource.h"
#include "CUSBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSSF33Dlg dialog

class CSSF33Dlg : public CDialog
{
// Construction
public:
	CSSF33Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSSF33Dlg)
	enum { IDD = IDD_SSF33DLG };	
	CString	m_ssf33_para;
	CString	m_ssf33_key;
	CString	m_ssf33_iv;
	CString	m_ssf33_plain;
	CString	m_ssf33_cipher;
	CString	m_ssf33_output;	
	int		m_ssf33_mode;
	int		m_ssf33_skmode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSF33Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CSSF33Dlg::ssf33_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut);

	// Generated message map functions
	//{{AFX_MSG(CSSF33Dlg)
	afx_msg void OnSSF33ButtonEnc();
	afx_msg void OnSSF33ButtonDec();
	afx_msg void OnSSF33ButtonEncFile();
	afx_msg void OnSSF33ButtonDecFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSF33DLG_H__309AE248_6113_4765_9988_AD8F3D57E1DE__INCLUDED_)
