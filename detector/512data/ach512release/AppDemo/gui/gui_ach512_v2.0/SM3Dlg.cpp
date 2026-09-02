// SM3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "SM3Dlg.h"
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
#define CMD_SM3_HASH               0x01
#define CMD_SM3_HASHFILE           0x02
#define CMD_SM3_HASHFORSM2         0x03

#define SM3_SUCCESS                0x00

#define SM3_DIGISTLEN              256
#define SM3_DIGISTLEN_INBYTE       (SM3_DIGISTLEN/8)
#define SM3_DIGISTLEN_INWORD       (SM3_DIGISTLEN/32)
#define SM2_PUBKEYLEN_INBYTE       (256/8)
#define SM2_PUBKEYLEN_INWORD       (256/32)

#define SM3_MAX_MSG_LEN            4096

/////////////////////////////////////////////////////////////////////////////
// CSM3Dlg dialog


CSM3Dlg::CSM3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSM3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSM3Dlg)
	m_sm3_pubQx = _T("");
	m_sm3_pubQy = _T("");
	m_sm3_id = _T("");
	m_sm3_msg = _T("");
	m_sm3_output = _T("");
	m_sm3_inputfilepath = _T("");
	//}}AFX_DATA_INIT
}


void CSM3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSM3Dlg)
	DDX_Text(pDX, IDC_SM3_EDIT_PUBX, m_sm3_pubQx);
	DDX_Text(pDX, IDC_SM3_EDIT_PUBY, m_sm3_pubQy);
	DDX_Text(pDX, IDC_SM3_EDIT_ID, m_sm3_id);
	DDX_Text(pDX, IDC_SM3_EDIT_MSG, m_sm3_msg);
	DDX_Text(pDX, IDC_SM3_EDIT_OUTPUT, m_sm3_output);
	DDX_Text(pDX, IDC_SM3_EDIT_INPUTFILEPATH, m_sm3_inputfilepath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSM3Dlg, CDialog)
	//{{AFX_MSG_MAP(CSM3Dlg)
	ON_BN_CLICKED(IDC_SM3_BUTTON_HASH, OnSM3ButtonHash)
	ON_BN_CLICKED(IDC_SM3_BUTTON_HASHFORSM2, OnSM3ButtonHashForSm2)
	ON_BN_CLICKED(IDC_SM3_BUTTON_HASHFILE, OnSM3ButtonHashFile)
	ON_BN_CLICKED(IDC_SM3_BUTTON_INPUTFILE, OnSM3ButtonInputfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSM3Dlg message handlers

void CSM3Dlg::OnSM3ButtonHash() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE bCryptBuf[SM3_MAX_MSG_LEN];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *msg;
	BYTE *digist;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	if(0 == m_sm3_msg.GetLength())
	{
		AfxMessageBox("SM3 msg should not be empty!");
		return;
	}

	datalen = 0;

	msg = bSendBuf;
	templen = m_sm3_msg.GetLength();
	memcpy(msg, m_sm3_msg, templen);
	datalen += templen;

	crypt_req.IP_code = SM3;
	crypt_req.cmd_code = CMD_SM3_HASH;
	crypt_req.key_length = 0;
	crypt_req.crypt_length = m_sm3_msg.GetLength();

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	datalen = SM3_DIGISTLEN_INBYTE;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	digist = bRecvBuf;
	pUSBDlg->ShowReceivedData(m_sm3_output, digist, SM3_DIGISTLEN_INBYTE);

	UpdateData(FALSE);
}

void CSM3Dlg::OnSM3ButtonHashForSm2() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE bCryptBuf[2*SM2_PUBKEYLEN_INBYTE+SM3_MAX_MSG_LEN];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *pubKeyX;
	BYTE *pubKeyY;
	BYTE *id;
	BYTE *msg;
	BYTE *digist;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	if(SM2_PUBKEYLEN_INBYTE != (m_sm3_pubQx.GetLength()/2))
	{
		AfxMessageBox("SM3 pubKeyX key length error!");
		return;
	}
	if(SM2_PUBKEYLEN_INBYTE != (m_sm3_pubQy.GetLength()/2))
	{
		AfxMessageBox("SM3 pubKeyY key length error!");
		return;
	}
	if(0 == m_sm3_id.GetLength())
	{
		AfxMessageBox("SM3 id should not be empty!");
		return;
	}
	if(0 == m_sm3_msg.GetLength())
	{
		AfxMessageBox("SM3 msg should not be empty!");
		return;
	}

	pubKeyX = bSendBuf;
	pubKeyY = pubKeyX + SM2_PUBKEYLEN_INBYTE;
	pUSBDlg->StringToBytes(pubKeyX, m_sm3_pubQx, SM2_PUBKEYLEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(pubKeyY, m_sm3_pubQy, SM2_PUBKEYLEN_INBYTE, TRUE);

	datalen = 0;
	
	id = pubKeyY + SM2_PUBKEYLEN_INBYTE;
	templen = m_sm3_id.GetLength();
	memcpy(id, m_sm3_id, templen);
	datalen += templen;

	msg = id + datalen;
	templen = m_sm3_msg.GetLength();
	memcpy(msg, m_sm3_msg, templen);
	datalen += templen;

	crypt_req.IP_code = SM3;
	crypt_req.cmd_code = CMD_SM3_HASHFORSM2;
	crypt_req.key_length = 2*SM2_PUBKEYLEN_INBYTE;
	crypt_req.crypt_length = (m_sm3_id.GetLength() << 16) + m_sm3_msg.GetLength(); //high 16bit store id length, low 16bit store msg length

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	datalen = SM3_DIGISTLEN_INBYTE;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	digist = bRecvBuf;
	pUSBDlg->ShowReceivedData(m_sm3_output, digist, SM3_DIGISTLEN_INBYTE);

	UpdateData(FALSE);
}

void CSM3Dlg::OnSM3ButtonHashFile() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT  datalen, OutputLength;
	CFile m_inputfile;
	BYTE bCryptBuf[SM3_MAX_MSG_LEN];
	BYTE  *bSendBuf;
	BYTE  *bRecvBuf;
	BYTE  *digist;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;
	
	UpdateData(TRUE);

	m_inputfile.Abort();
	m_inputfile.Open(m_sm3_inputfilepath, CFile::modeRead | CFile::shareDenyWrite);

	crypt_req.IP_code = SM3;
	crypt_req.cmd_code = CMD_SM3_HASHFILE;
	crypt_req.key_length = 0;
	
	while(1)
	{
		// Read file
		datalen = m_inputfile.Read(bSendBuf, SM3_MAX_MSG_LEN);

		crypt_req.crypt_length = datalen;

		// Hash
		if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
		{
			CString error;
			error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
			AfxMessageBox(error);
			return;
		}

		if(datalen < SM3_MAX_MSG_LEN)
			break;
	}

	m_inputfile.Close();

	CString result;
	result.Format("File Hash Completed!");
	AfxMessageBox(result);

	datalen = SM3_DIGISTLEN_INBYTE;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	digist = bRecvBuf;
	pUSBDlg->ShowReceivedData(m_sm3_output, digist, SM3_DIGISTLEN_INBYTE);
	
	UpdateData(FALSE);
}

void CSM3Dlg::OnSM3ButtonInputfile() 
{
	// TODO: Add your control notification handler code here
	char Filter_file[] = "Supported Files(*.txt, *.dat)|*.txt;*.dat|All Files(*.*)|*.*||";
	CFileDialog file_Open(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,(LPCTSTR)Filter_file,NULL);

	if(IDOK == file_Open.DoModal())
	{
		m_sm3_inputfilepath = file_Open.GetPathName();
	}

	UpdateData(FALSE);
}
