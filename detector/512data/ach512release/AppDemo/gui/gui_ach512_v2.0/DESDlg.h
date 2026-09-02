#if !defined(AFX_DESDLG_H__4C9741FF_52A2_4853_8D3D_C8678587216C__INCLUDED_)
#define AFX_DESDLG_H__4C9741FF_52A2_4853_8D3D_C8678587216C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DESDlg.h : header file
//
#include "resource.h"
#include "CUSBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDESDlg dialog

class CDESDlg : public CDialog
{
// Construction
public:
	CDESDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDESDlg)
	enum { IDD = IDD_DESDLG };
	CString	m_des_key1;
	CString	m_des_key2;
	CString	m_des_key3;
	CString	m_des_iv;
	CString	m_des_plain;
	CString	m_des_cipher;
	CString	m_des_output;
	int		m_des_mode;
	int		m_des_keynum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDESDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int CDESDlg::des_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut);

	// Generated message map functions
	//{{AFX_MSG(CDESDlg)
	afx_msg void OnDESButtonEnc();
	afx_msg void OnDESButtonDec();
	afx_msg void OnDESButtonEncFile();
	afx_msg void OnDESButtonDecFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESDLG_H__4C9741FF_52A2_4853_8D3D_C8678587216C__INCLUDED_)
