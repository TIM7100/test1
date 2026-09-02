#if !defined(AFX_SM3DLG_H__E2F19623_7717_450D_B342_609B829E30F0__INCLUDED_)
#define AFX_SM3DLG_H__E2F19623_7717_450D_B342_609B829E30F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SM3Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSM3Dlg dialog

class CSM3Dlg : public CDialog
{
// Construction
public:
	CSM3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSM3Dlg)
	enum { IDD = IDD_SM3DLG };
	CString	m_sm3_pubQx;
	CString	m_sm3_pubQy;
	CString	m_sm3_id;
	CString	m_sm3_msg;
	CString	m_sm3_output;
	CString	m_sm3_inputfilepath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSM3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSM3Dlg)
	afx_msg void OnSM3ButtonHash();
	afx_msg void OnSM3ButtonHashForSm2();
	afx_msg void OnSM3ButtonHashFile();
	afx_msg void OnSM3ButtonInputfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SM3DLG_H__E2F19623_7717_450D_B342_609B829E30F0__INCLUDED_)
