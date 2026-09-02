/**************************************************************************
Filename : CUSBDlg.cpp
Language : Cpp
Description : implementation file.
Author(s) :   
Company  : ANCH Ltd.
version  : 1.0
Change Log : 2007-3-3
******************************************************************************/


#include "stdafx.h"
#include "CUSB.h"
#include "CUSBDlg.h"
#include "crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCUSBDlg dialog

CCUSBDlg::CCUSBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCUSBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCUSBDlg)
	m_inputfilepath = _T("");
	m_outputfilepath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCUSBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCUSBDlg)
	DDX_Control(pDX, IDC_USB_MY_TAB, m_tabctrl);
	DDX_Text(pDX, IDC_USB_EDIT_INPUTFILEPATH, m_inputfilepath);
	DDX_Text(pDX, IDC_USB_EDIT_OUTPUTFILEPATH, m_outputfilepath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCUSBDlg, CDialog)
	//{{AFX_MSG_MAP(CCUSBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_USB_BUTTON_CONNECT, OnUSBButtonConnect)
	ON_BN_CLICKED(IDC_USB_BUTTON_EXIT, OnUSBButtonExit)
	ON_BN_CLICKED(IDC_USB_BUTTON_INPUTFILE, OnUSBButtonInputfile)
	ON_BN_CLICKED(IDC_USB_BUTTON_OUTPUTFILE, OnUSBButtonOutputfile)
	ON_BN_CLICKED(IDC_USB_BUTTON_DISCONNECT, OnUSBButtonDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCUSBDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCUSBDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCUSBDlg message handlers

BOOL CCUSBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_tabctrl.InsertItem(0, _T("SM1"));
	m_tabctrl.InsertItem(1, _T("SM4"));
	m_tabctrl.InsertItem(2, _T("SSF33"));
	m_tabctrl.InsertItem(3, _T("AES"));
	m_tabctrl.InsertItem(4, _T("DES"));
	m_tabctrl.InsertItem(5, _T("SM2"));
	m_tabctrl.InsertItem(6, _T("SM3"));
	m_tabctrl.InsertItem(7, _T("RSA"));
	m_tabctrl.InsertItem(8, _T("HRNG"));
	m_tabctrl.InsertItem(9, _T("ISO7816"));
	m_tabctrl.Init();

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control

}

void CCUSBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCUSBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCUSBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCUSBDlg::OnUSBButtonConnect() 
{
	// TODO: Add your control notification handler code here
	if (ERROR_SUCCESS != mydevice.IniDevice())
	{
		CString error;
		error.Format("Device Invalid£¬Error Num(%d)", GetLastError());
		AfxMessageBox(error);
	}
	else
	{
		AfxMessageBox("Device Connect Successfully!");
	}
}

void CCUSBDlg::OnUSBButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	if (ERROR_SUCCESS != mydevice.CloseDevice())
	{
		CString error;
		error.Format("Can't Close Device£¬Error Num(%d)", GetLastError());
		AfxMessageBox(error);
	}
	else
	{
		AfxMessageBox("Device Closed!");
	}
}

void CCUSBDlg::OnUSBButtonExit() 
{
	// TODO: Add your control notification handler code here
	mydevice.CloseDevice();

	CDialog::OnCancel();	
}

void CCUSBDlg::OnUSBButtonInputfile() 
{
	// TODO: Add your control notification handler code here
	char Filter_file[] = "Supported Files(*.txt , *.dat)|*.txt;*.dat|All Files(*.*)|*.*||";
	CFileDialog file_Open(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,(LPCTSTR)Filter_file,NULL);

	if(IDOK == file_Open.DoModal())
	{
		m_inputfilepath = file_Open.GetPathName();
	}

	UpdateData(FALSE);	
}

void CCUSBDlg::OnUSBButtonOutputfile() 
{
	// TODO: Add your control notification handler code here
	char Filter_file[]="Supported Files(*.txt , *.dat)|*.txt;*.dat|All Files(*.*)|*.*||";
	CFileDialog file_Open(FALSE,0,"*.txt",OFN_HIDEREADONLY,(LPCTSTR)Filter_file,NULL);

	if(IDOK == file_Open.DoModal())
	{
		m_outputfilepath = file_Open.GetPathName();
	}

	UpdateData(FALSE);		
}

void CCUSBDlg::Show_Byte_HEX(CString &tempstring, BYTE data_byte)
{
	CString strtemp;
	BYTE char_2_Hex;
	char_2_Hex=data_byte>>4;
	if (char_2_Hex>9){
		char_2_Hex=char_2_Hex+'A'-10;
				}
	else char_2_Hex=char_2_Hex+'0';
	strtemp.Format("%c",char_2_Hex);
	tempstring+=strtemp;
	char_2_Hex=data_byte%16;
	if (char_2_Hex>9){
		char_2_Hex=char_2_Hex+'A'-10;
				}
	else char_2_Hex=char_2_Hex+'0';
	strtemp.Format("%c",char_2_Hex);
//	tempstring+=strtemp;
//	strtemp.Format("%c",' ');// add a space
	tempstring+=strtemp;
    
}
void CCUSBDlg::ShowReceivedData(CString &showstring,BYTE *bcontext,UINT ReceiveLength)
{
	unsigned int i;

	showstring = "";
	for(i=0;i<ReceiveLength;i++)
	{
		CCUSBDlg::Show_Byte_HEX(showstring,bcontext[i]);
	}
}

BYTE CCUSBDlg::Char2Num(unsigned char asc)
{
	if(asc>='0'&&asc<='9')
		return (asc-'0');
	else if(asc>='a'&&asc<='f')
		return (asc-'a'+10);
	else if(asc>='A'&&asc<='F')
		return (asc-'A'+10);
	else
		return -1;

}

void CCUSBDlg::StringToWords(DWORD *pword, CString str, UINT ncount, BOOL endian)
{
	UINT i, j;
	DWORD temp;

	for (i = 0; i < ncount; i++)
	{
		temp = 0;
		for (j = 0; j < 8; j++)
		{
			temp = temp << 4;
			temp |= Char2Num(str.GetAt(i*8+j));
		}
		if (endian)	// big endian
			pword[i] = temp;
		else 
			pword[ncount-1-i] = temp;
	}
}


void CCUSBDlg::StringToBytes(BYTE *pbyte, CString str, UINT ncount, BOOL endian)
{
	UINT i, j;
	BYTE temp;

	for (i = 0; i < ncount; i++)
	{
		temp = 0;
		for (j = 0; j < 2; j++)
		{
			temp = temp << 4;
			temp |= Char2Num(str.GetAt(i*2+j));
		}
		if (endian)	// big endian
			pbyte[i] = temp;
		else 
			pbyte[ncount-1-i] = temp;
	}
}

void CCUSBDlg::SwapString(BYTE *str, DWORD wordnum)
{
	ULONG i;
	DWORD *dwstr = (DWORD *) str;

	for (i = 0; i < wordnum; i++)
		dwstr[i] = SwapWord(dwstr[i]);
}
