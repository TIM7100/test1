// HRNGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "HRNGDlg.h"
#include "UsbDevice.h"	// Added by ClassView
#include "CUSBDlg.h"	// Added by ClassView
#include "crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//cmd_code definition
//bits[7:0] means operation.
#define CMD_HRNG_READ_REG          0x01
#define CMD_HRNG_GET_HRNGDATA      0x02

#define HRNG_SUCCEED               0x00
#define HRNG_FAILED                0xFF

#define HRNG_MAX_DATA_NUM          4096
#define HRNG_OUTPUTFILE_LEN        (128*1024)

/////////////////////////////////////////////////////////////////////////////
// CHRNGDlg dialog


CHRNGDlg::CHRNGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHRNGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHRNGDlg)
	m_hrng_outputfilepath = _T("");
	m_hrng_regaddr = _T("");
	m_hrng_regvalue = _T("");
	//}}AFX_DATA_INIT
}


void CHRNGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHRNGDlg)
	DDX_Text(pDX, IDC_HRNG_EDIT_OUTPUTFILEPATH, m_hrng_outputfilepath);
	DDX_Text(pDX, IDC_HRNG_EDIT_REGADDR, m_hrng_regaddr);
	DDX_Text(pDX, IDC_HRNG_EDIT_REGVALUE, m_hrng_regvalue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHRNGDlg, CDialog)
	//{{AFX_MSG_MAP(CHRNGDlg)
	ON_BN_CLICKED(IDC_HRNG_BUTTON_READREG, OnHRNGButtonReadReg)
	ON_BN_CLICKED(IDC_HRNG_BUTTON_GETHRNGDATA, OnHRNGButtonGetHrngData)
	ON_BN_CLICKED(IDC_HRNG_BUTTON_OUTPUTFILE, OnHRNGButtonOutputfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHRNGDlg message handlers

void CHRNGDlg::OnHRNGButtonReadReg() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	DWORD tempLen;
	UINT  OutputLength;
	BYTE  bSendBuf[4];
	BYTE  bRecvBuf[4];

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	UpdateData(TRUE);

	// verify addr is under rule or not
	if(m_hrng_regaddr.GetLength() != 8)
	{
		AfxMessageBox("Register addr must be 4bytes in Length!");
		return;
	}
	else
	{
		tempLen = 4;
		pUSBDlg->StringToBytes(bSendBuf, m_hrng_regaddr, tempLen, TRUE);
	}

	crypt_req.IP_code = HRNG;
	crypt_req.cmd_code = CMD_HRNG_READ_REG;
	crypt_req.key_length = 0;
	crypt_req.crypt_length = tempLen;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, tempLen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// read register value
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, tempLen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	pUSBDlg->ShowReceivedData(m_hrng_regvalue, bRecvBuf, tempLen);

	UpdateData(FALSE);
}

void CHRNGDlg::OnHRNGButtonGetHrngData() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT  datalen, OutputLength;
	CFile m_outputfile;
	BYTE  bRecvBuf[HRNG_MAX_DATA_NUM];
	int i;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	UpdateData(TRUE);

	m_outputfile.Abort();
	m_outputfile.Open(m_hrng_outputfilepath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);

	crypt_req.IP_code = HRNG;
	crypt_req.cmd_code = CMD_HRNG_GET_HRNGDATA;
	crypt_req.key_length = 0;
	crypt_req.crypt_length = HRNG_MAX_DATA_NUM;
	
	datalen = crypt_req.crypt_length;
	for(i = 0; i < HRNG_OUTPUTFILE_LEN; i += datalen)
	{
		// write req
		if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, 0, NULL, &OutputLength))
		{
			CString error;
			error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
			AfxMessageBox(error);
			return;
		}

		// read hrng data
		if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
		{
			CString error;
			error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
			AfxMessageBox(error);
			return;
		}

		// write file
		m_outputfile.Write(bRecvBuf, datalen);
	}

	m_outputfile.Close();

	CString result;
	result.Format("Get %dK Bytes hrng data Completed!", HRNG_OUTPUTFILE_LEN/1024);
	AfxMessageBox(result);

	UpdateData(FALSE);
}

void CHRNGDlg::OnHRNGButtonOutputfile() 
{
	// TODO: Add your control notification handler code here
	char Filter_file[] = "Supported Files(*.bin)|*.bin||";
	CFileDialog file_Open(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,(LPCTSTR)Filter_file,NULL);

	if(IDOK == file_Open.DoModal())
	{
		m_hrng_outputfilepath = file_Open.GetPathName();
	}

	UpdateData(FALSE);
}
