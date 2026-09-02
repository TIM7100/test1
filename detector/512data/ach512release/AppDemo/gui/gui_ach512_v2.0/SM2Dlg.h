#if !defined(AFX_SM2DLG_H__8454259E_3707_42BF_B115_1B077533AFD0__INCLUDED_)
#define AFX_SM2DLG_H__8454259E_3707_42BF_B115_1B077533AFD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SM2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSM2Dlg dialog

class CSM2Dlg : public CDialog
{
// Construction
public:
	CSM2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSM2Dlg)
	enum { IDD = IDD_SM2DLG };
	CString	m_sm2_prik;
	CString	m_sm2_pubQx;
	CString	m_sm2_pubQy;
	CString	m_sm2_id;
	CString	m_sm2_msg;
	CString	m_sm2_signr;
	CString	m_sm2_signs;
	CString	m_sm2_cipher;	
	CString	m_sm2_plain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSM2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSM2Dlg)
	afx_msg void OnSM2ButtonKeyGen();
	afx_msg void OnSM2ButtonSign();
	afx_msg void OnSM2ButtonVerify();
	afx_msg void OnSM2ButtonEnc();
	afx_msg void OnSM2ButtonDec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SM2DLG_H__8454259E_3707_42BF_B115_1B077533AFD0__INCLUDED_)
