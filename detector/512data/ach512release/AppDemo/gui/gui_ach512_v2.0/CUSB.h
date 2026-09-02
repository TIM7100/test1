// CUSB.h : main header file for the CUSB application
//

#if !defined(AFX_CUSB_H__67EF0706_75D6_4CA7_8110_126B17AE648B__INCLUDED_)
#define AFX_CUSB_H__67EF0706_75D6_4CA7_8110_126B17AE648B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCUSBApp:
// See CUSB.cpp for the implementation of this class
//

class CCUSBApp : public CWinApp
{
public:
	CCUSBApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCUSBApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCUSBApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSB_H__67EF0706_75D6_4CA7_8110_126B17AE648B__INCLUDED_)
