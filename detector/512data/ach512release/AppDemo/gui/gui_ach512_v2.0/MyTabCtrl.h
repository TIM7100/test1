#if !defined(AFX_MYTABCTRL_H__C900E789_C255_48A8_9A76_53B906856268__INCLUDED_)
#define AFX_MYTABCTRL_H__C900E789_C255_48A8_9A76_53B906856268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();
	CDialog *m_tabPages[10];

	int m_tabCurrent;
	int m_nNumberOfPages;

// Attributes
public:

// Operations
public:
	void Init();
	void SetRectangle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__C900E789_C255_48A8_9A76_53B906856268__INCLUDED_)
