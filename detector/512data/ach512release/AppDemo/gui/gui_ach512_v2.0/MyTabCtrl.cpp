// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyTabCtrl.h"

#include "SM1Dlg.h"
#include "SM4Dlg.h"
#include "SSF33Dlg.h"
#include "AESDlg.h"
#include "DESDlg.h"
#include "SM2Dlg.h"
#include "SM3Dlg.h"
#include "RSADlg.h"
#include "HRNGDlg.h"
#include "ISO7816Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_tabPages[0]=new CSM1Dlg;
	m_tabPages[1]=new CSM4Dlg;
	m_tabPages[2]=new CSSF33Dlg;
	m_tabPages[3]=new CAESDlg;
	m_tabPages[4]=new CDESDlg;
	m_tabPages[5]=new CSM2Dlg;
	m_tabPages[6]=new CSM3Dlg;
	m_tabPages[7]=new CRSADlg;
	m_tabPages[8]=new CHRNGDlg;
	m_tabPages[9]=new CISO7816Dlg;

	m_nNumberOfPages=10;
}

CMyTabCtrl::~CMyTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++)
	{
		delete m_tabPages[nCount];
	}
}

void CMyTabCtrl::Init()
{
	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_SM1DLG, this);
	m_tabPages[1]->Create(IDD_SM4DLG, this);
	m_tabPages[2]->Create(IDD_SSF33DLG, this);
	m_tabPages[3]->Create(IDD_AESDLG, this);
	m_tabPages[4]->Create(IDD_DESDLG, this);
	m_tabPages[5]->Create(IDD_SM2DLG, this);
	m_tabPages[6]->Create(IDD_SM3DLG, this);
	m_tabPages[7]->Create(IDD_RSADLG, this);
	m_tabPages[8]->Create(IDD_HRNGDLG, this);
	m_tabPages[9]->Create(IDD_ISO7816DLG, this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);
	m_tabPages[4]->ShowWindow(SW_HIDE);
	m_tabPages[5]->ShowWindow(SW_HIDE);
	m_tabPages[6]->ShowWindow(SW_HIDE);
	m_tabPages[7]->ShowWindow(SW_HIDE);
	m_tabPages[8]->ShowWindow(SW_HIDE);
	m_tabPages[9]->ShowWindow(SW_HIDE);
	
//	m_tabPages[1]->CheckRadioButton(IDC_RADIO_ECB,IDC_RADIO_CBC,IDC_RADIO_ECB);
//	m_tabPages[1]->CheckRadioButton(IDC_RADIO_KEY1,IDC_RADIO_KEY3,IDC_RADIO_KEY1);
//	m_tabPages[0]->CheckRadioButton(IDC_RADIO_EXPMOD,IDC_RADIO_MULMOD,IDC_RADIO_EXPMOD);
		
	SetRectangle();

}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers

void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
}


