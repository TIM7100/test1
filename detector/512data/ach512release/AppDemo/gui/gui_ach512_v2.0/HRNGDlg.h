#if !defined(AFX_HRNGDLG_H__F4F35A67_D57B_4D5C_BB84_8CEC215C1A01__INCLUDED_)
#define AFX_HRNGDLG_H__F4F35A67_D57B_4D5C_BB84_8CEC215C1A01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HRNGDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHRNGDlg dialog

class CHRNGDlg : public CDialog
{
// Construction
public:
	CHRNGDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHRNGDlg)
	enum { IDD = IDD_HRNGDLG };
	CString	m_hrng_outputfilepath;
	CString	m_hrng_regaddr;
	CString	m_hrng_regvalue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHRNGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHRNGDlg)
	afx_msg void OnHRNGButtonReadReg();
	afx_msg void OnHRNGButtonGetHrngData();
	afx_msg void OnHRNGButtonOutputfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HRNGDLG_H__F4F35A67_D57B_4D5C_BB84_8CEC215C1A01__INCLUDED_)
