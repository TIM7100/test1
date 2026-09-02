// CUSBDlg.h : header file
//
//{{AFX_INCLUDES()
#include "MyTabCtrl.h"
//}}AFX_INCLUDES

#if !defined(AFX_CUSBDLG_H__D07A1FAD_5099_41A2_B43A_17FC7644CCD3__INCLUDED_)
#define AFX_CUSBDLG_H__D07A1FAD_5099_41A2_B43A_17FC7644CCD3__INCLUDED_

#include "UsbDevice.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCUSBDlg dialog

class CCUSBDlg : public CDialog
{
// Construction
public:
	void StringToWords(DWORD *pword, CString str, UINT ncount, BOOL endian);
	void StringToBytes(BYTE *pbyte, CString str, UINT ncount, BOOL endian);
	void SwapString(BYTE *str, DWORD wordnum);
	int num;
	BYTE Char2Num(unsigned char asc);
	void Show_Byte_HEX(CString &tempstring,BYTE data_byte);
	void ShowReceivedData(CString &showstring,BYTE *bcontext,UINT ReceiveLength);

	CUsbDevice mydevice;
	CCUSBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCUSBDlg)
	enum { IDD = IDD_CUSB_DIALOG };
	CMyTabCtrl	m_tabctrl;
	CString	m_inputfilepath;
	CString	m_outputfilepath;	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCUSBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCUSBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUSBButtonConnect();
	afx_msg void OnUSBButtonExit();
	afx_msg void OnUSBButtonInputfile();
	afx_msg void OnUSBButtonOutputfile();
	afx_msg void OnUSBButtonDisconnect();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSBDLG_H__D07A1FAD_5099_41A2_B43A_17FC7644CCD3__INCLUDED_)
