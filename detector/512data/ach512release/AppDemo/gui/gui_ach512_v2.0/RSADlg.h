#if !defined(AFX_RSADLG_H__61935479_3E30_4C5A_9471_716873EBD14B__INCLUDED_)
#define AFX_RSADLG_H__61935479_3E30_4C5A_9471_716873EBD14B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSADlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRSADlg dialog

class CRSADlg : public CDialog
{
// Construction
public:
	CRSADlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRSADlg)
	enum { IDD = IDD_RSADLG };
	CString	m_rsa_n;
	DWORD	m_rsa_e;
	CString	m_rsa_d;
	CString	m_rsa_p;	
	CString	m_rsa_q;
	CString	m_rsa_dp;
	CString	m_rsa_dq;
	CString	m_rsa_qinv;
	CString	m_rsa_plain;
	CString	m_rsa_cipher;
	CString	m_rsa_output;
	int		m_rsa_nlength;
	BOOL	m_rsa_crt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRSADlg)
	afx_msg void OnRSAButtonKeyGen();
	afx_msg void OnRSAButtonEnc();
	afx_msg void OnRSAButtonDec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSADLG_H__61935479_3E30_4C5A_9471_716873EBD14B__INCLUDED_)
