#if !defined(AFX_ISO7816DLG_H__7E62BECF_A793_4A9A_A051_B12856762C04__INCLUDED_)
#define AFX_ISO7816DLG_H__7E62BECF_A793_4A9A_A051_B12856762C04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ISO7816Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CISO7816Dlg dialog

class CISO7816Dlg : public CDialog
{
// Construction
public:
	CISO7816Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CISO7816Dlg)
	enum { IDD = IDD_ISO7816DLG };
	CString	m_iso7816_apdu;
	CString	m_iso7816_apduresp;
	CString	m_iso7816_atr;
	CString	m_iso7816_pps;
	CString	m_iso7816_ppsresp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CISO7816Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BYTE protocol;
	BYTE CalBitNum(BYTE ch);
	BYTE GetProtocolType(const BYTE *atr);
	// Generated message map functions
	//{{AFX_MSG(CISO7816Dlg)
	afx_msg void OnIso7816ButtonReset();
	afx_msg void OnIso7816ButtonPps();
	afx_msg void OnIso7816ButtonApdu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISO7816DLG_H__7E62BECF_A793_4A9A_A051_B12856762C04__INCLUDED_)
