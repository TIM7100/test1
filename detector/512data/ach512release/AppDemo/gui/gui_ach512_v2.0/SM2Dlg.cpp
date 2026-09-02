// SM2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "SM2Dlg.h"
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
#define CMD_SM2_KEYGEN             0x01
#define CMD_SM2_SIGN               0x02
#define CMD_SM2_VERIFY             0x03
#define CMD_SM2_ENCRYPT            0x04
#define CMD_SM2_DECRYPT            0x05

#define SM2_SUCCESS                0x00

#define SM2_CURVELEN               256
#define SM2_CURVELEN_INBYTE        (SM2_CURVELEN/8)
#define SM2_CURVELEN_INWORD        (SM2_CURVELEN/32)

#define SM2_MAX_CRYPT_LEN          4096

static BOOL bIsSM2KeyReady = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CSM2Dlg dialog


CSM2Dlg::CSM2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSM2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSM2Dlg)
	m_sm2_prik = _T("");	
	m_sm2_pubQx = _T("");
	m_sm2_pubQy = _T("");
	m_sm2_id = _T("");
	m_sm2_msg = _T("");
	m_sm2_signr = _T("");
	m_sm2_signs = _T("");
	m_sm2_cipher = _T("");
	m_sm2_plain = _T("");
	//}}AFX_DATA_INIT
}


void CSM2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSM2Dlg)
	DDX_Text(pDX, IDC_SM2_EDIT_PRIVKEY, m_sm2_prik);
	DDX_Text(pDX, IDC_SM2_EDIT_PUBX, m_sm2_pubQx);
	DDX_Text(pDX, IDC_SM2_EDIT_PUBY, m_sm2_pubQy);
	DDX_Text(pDX, IDC_SM2_EDIT_ID, m_sm2_id);
	DDX_Text(pDX, IDC_SM2_EDIT_MSG, m_sm2_msg);
	DDX_Text(pDX, IDC_SM2_EDIT_SIGNR, m_sm2_signr);
	DDX_Text(pDX, IDC_SM2_EDIT_SIGNS, m_sm2_signs);
	DDX_Text(pDX, IDC_SM2_EDIT_CIPHER, m_sm2_cipher);
	DDX_Text(pDX, IDC_SM2_EDIT_PLAIN, m_sm2_plain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSM2Dlg, CDialog)
	//{{AFX_MSG_MAP(CSM2Dlg)
	ON_BN_CLICKED(IDC_SM2_BUTTON_KEYGEN, OnSM2ButtonKeyGen)
	ON_BN_CLICKED(IDC_SM2_BUTTON_SIGN, OnSM2ButtonSign)
	ON_BN_CLICKED(IDC_SM2_BUTTON_VERIFY, OnSM2ButtonVerify)
	ON_BN_CLICKED(IDC_SM2_BUTTON_ENC, OnSM2ButtonEnc)
	ON_BN_CLICKED(IDC_SM2_BUTTON_DEC, OnSM2ButtonDec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSM2Dlg message handlers

void CSM2Dlg::OnSM2ButtonKeyGen() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	BYTE bCryptBuf[3*SM2_CURVELEN_INBYTE];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *privKey;
	BYTE *pubKeyX;
	BYTE *pubKeyY;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	crypt_req.IP_code = SM2;
	crypt_req.cmd_code = CMD_SM2_KEYGEN;
	crypt_req.key_length = 1;  //if key_length != 0, after generated, keys should be return back, else nothing will be returned.
	crypt_req.crypt_length = 0;

	datalen = crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);

		bIsSM2KeyReady = FALSE;
		return;
	}

	datalen = 3*SM2_CURVELEN_INBYTE;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);

		bIsSM2KeyReady = FALSE;
		return;
	}
	AfxMessageBox("SM2 KeyGen Successfully!");
	bIsSM2KeyReady = TRUE;

	UpdateData(TRUE);
	privKey = bRecvBuf;
	pubKeyX = privKey + SM2_CURVELEN_INBYTE;
	pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
	pUSBDlg->ShowReceivedData(m_sm2_prik, privKey, SM2_CURVELEN_INBYTE);
	pUSBDlg->ShowReceivedData(m_sm2_pubQx, pubKeyX, SM2_CURVELEN_INBYTE);
	pUSBDlg->ShowReceivedData(m_sm2_pubQy, pubKeyY, SM2_CURVELEN_INBYTE);
	UpdateData(FALSE);
}

void CSM2Dlg::OnSM2ButtonSign() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE bCryptBuf[3*SM2_CURVELEN_INBYTE+SM2_MAX_CRYPT_LEN];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *privKey;
	BYTE *pubKeyX;
	BYTE *pubKeyY;
	BYTE *id;
	BYTE *msg;
	BYTE *signR;
	BYTE *signS;

	if(!bIsSM2KeyReady)
	{
		AfxMessageBox("SM2 Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	if(SM2_CURVELEN_INBYTE != (m_sm2_prik.GetLength()/2))
	{
		AfxMessageBox("SM2 private key length error!");
		return;
	}
	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQx.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyX key length error!");
		return;
	}
	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQy.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyY key length error!");
		return;
	}
	if(0 == m_sm2_id.GetLength())
	{
		AfxMessageBox("SM2 id should not be empty!");
		return;
	}
	if(0 == m_sm2_msg.GetLength())
	{
		AfxMessageBox("SM2 msg should not be empty!");
		return;
	}

	privKey = bSendBuf;
	pubKeyX = privKey + SM2_CURVELEN_INBYTE;
	pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
	pUSBDlg->StringToBytes(privKey, m_sm2_prik, SM2_CURVELEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(pubKeyX, m_sm2_pubQx, SM2_CURVELEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(pubKeyY, m_sm2_pubQy, SM2_CURVELEN_INBYTE, TRUE);

	datalen = 0;
	
	id = pubKeyY + SM2_CURVELEN_INBYTE;
	templen = m_sm2_id.GetLength();
	memcpy(id, m_sm2_id, templen);
	datalen += templen;

	msg = id + datalen;
	templen = m_sm2_msg.GetLength();
	memcpy(msg, m_sm2_msg, templen);
	datalen += templen;

	crypt_req.IP_code = SM2;
	crypt_req.cmd_code = CMD_SM2_SIGN;
	crypt_req.key_length = 3*SM2_CURVELEN_INBYTE;
	crypt_req.crypt_length = (m_sm2_id.GetLength() << 16) + m_sm2_msg.GetLength(); //high 16bit store id length, low 16bit store msg length

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	datalen = 2*SM2_CURVELEN_INBYTE;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	signR = bRecvBuf;
	signS = signR + SM2_CURVELEN_INBYTE;
	pUSBDlg->ShowReceivedData(m_sm2_signr, signR, SM2_CURVELEN_INBYTE);
	pUSBDlg->ShowReceivedData(m_sm2_signs, signS, SM2_CURVELEN_INBYTE);

	UpdateData(FALSE);
}

void CSM2Dlg::OnSM2ButtonVerify() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE status;
	BYTE bCryptBuf[5*SM2_CURVELEN_INBYTE+SM2_MAX_CRYPT_LEN];
	BYTE *bSendBuf;
	BYTE *pubKeyX;
	BYTE *pubKeyY;
	BYTE *id;
	BYTE *msg;
	BYTE *signR;
	BYTE *signS;

	if(!bIsSM2KeyReady)
	{
		AfxMessageBox("SM2 Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;

	UpdateData(TRUE);

	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQx.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyX key length error!");
		return;
	}
	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQy.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyY key length error!");
		return;
	}
	if(0 == m_sm2_id.GetLength())
	{
		AfxMessageBox("SM2 id should not be empty!");
		return;
	}
	if(0 == m_sm2_msg.GetLength())
	{
		AfxMessageBox("SM2 msg should not be empty!");
		return;
	}

	if(0 == m_sm2_signr.GetLength())
	{
		AfxMessageBox("SM2 msg should not be empty!");
		return;
	}
	else if(SM2_CURVELEN_INBYTE != (m_sm2_signr.GetLength()/2))
	{
		CString strtemp('0', (2*SM2_CURVELEN_INBYTE - m_sm2_signr.GetLength()));
		m_sm2_signr = strtemp + m_sm2_signr;
	}

	if(0 == m_sm2_signs.GetLength())
	{
		AfxMessageBox("SM2 msg should not be empty!");
		return;
	}
	else if(SM2_CURVELEN_INBYTE != (m_sm2_signs.GetLength()/2))
	{
		CString strtemp('0', (2*SM2_CURVELEN_INBYTE - m_sm2_signs.GetLength()));
		m_sm2_signs = strtemp + m_sm2_signs;
	}
	
	pubKeyX = bSendBuf;
	pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
	pUSBDlg->StringToBytes(pubKeyX, m_sm2_pubQx, SM2_CURVELEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(pubKeyY, m_sm2_pubQy, SM2_CURVELEN_INBYTE, TRUE);

	datalen = 0;
	
	id = pubKeyY + SM2_CURVELEN_INBYTE;
	templen = m_sm2_id.GetLength();
	memcpy(id, m_sm2_id, templen);
	datalen += templen;

	msg = id + datalen;
	templen = m_sm2_msg.GetLength();
	memcpy(msg, m_sm2_msg, templen);
	datalen += templen;

	signR = msg + templen;
	signS = signR + SM2_CURVELEN_INBYTE;
	pUSBDlg->StringToBytes(signR, m_sm2_signr, SM2_CURVELEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(signS, m_sm2_signs, SM2_CURVELEN_INBYTE, TRUE);

	datalen += 2*SM2_CURVELEN_INBYTE;

	crypt_req.IP_code = SM2;
	crypt_req.cmd_code = CMD_SM2_VERIFY;
	crypt_req.key_length = 2*SM2_CURVELEN_INBYTE;
	crypt_req.crypt_length = (m_sm2_id.GetLength() << 16) + m_sm2_msg.GetLength(); //high 16bit store id length, low 16bit store msg length

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}
	
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, 1, &status, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	if(SM2_SUCCESS != status)
	{
		AfxMessageBox("SM2 Verify Failed!");
	}
	else
	{
		AfxMessageBox("SM2 Verify Successfully!");
	}

	UpdateData(FALSE);
}

void CSM2Dlg::OnSM2ButtonEnc() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE bCryptBuf[2*SM2_CURVELEN_INBYTE+SM2_MAX_CRYPT_LEN];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *pubKeyX;
	BYTE *pubKeyY;
	BYTE *msg;
	BYTE *sm2C1;
	BYTE *sm2C2;
	BYTE *sm2C3;

	if(!bIsSM2KeyReady)
	{
		AfxMessageBox("SM2 Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQx.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyX key length error!");
		return;
	}
	if(SM2_CURVELEN_INBYTE != (m_sm2_pubQy.GetLength()/2))
	{
		AfxMessageBox("SM2 pubKeyY key length error!");
		return;
	}
	if(0 == m_sm2_msg.GetLength())
	{
		AfxMessageBox("SM2 msg should not be empty!");
		return;
	}

	pubKeyX = bSendBuf;
	pubKeyY = pubKeyX + SM2_CURVELEN_INBYTE;
	pUSBDlg->StringToBytes(pubKeyX, m_sm2_pubQx, SM2_CURVELEN_INBYTE, TRUE);
	pUSBDlg->StringToBytes(pubKeyY, m_sm2_pubQy, SM2_CURVELEN_INBYTE, TRUE);

	datalen = 0;

	msg = pubKeyY+SM2_CURVELEN_INBYTE;
	templen = m_sm2_msg.GetLength();
	memcpy(msg, m_sm2_msg, templen);
	datalen += templen;

	crypt_req.IP_code = SM2;
	crypt_req.cmd_code = CMD_SM2_ENCRYPT;
	crypt_req.key_length = 2*SM2_CURVELEN_INBYTE;
	crypt_req.crypt_length = m_sm2_msg.GetLength();

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	templen = crypt_req.crypt_length;
	datalen = 3*SM2_CURVELEN_INBYTE + templen;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	sm2C1 = bRecvBuf;
	sm2C2 = sm2C1 + 2*SM2_CURVELEN_INBYTE;
	sm2C3 = sm2C2 + templen;

	CString str;
	pUSBDlg->ShowReceivedData(str, sm2C1, 2*SM2_CURVELEN_INBYTE);
	m_sm2_cipher = str;
	pUSBDlg->ShowReceivedData(str, sm2C2, templen);
	m_sm2_cipher += str;
	pUSBDlg->ShowReceivedData(str, sm2C3, SM2_CURVELEN_INBYTE);
	m_sm2_cipher += str;

	UpdateData(FALSE);
}

void CSM2Dlg::OnSM2ButtonDec() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT datalen, OutputLength;
	UINT templen;
	BYTE bCryptBuf[SM2_CURVELEN_INBYTE+SM2_MAX_CRYPT_LEN];
	BYTE *bSendBuf;
	BYTE *bRecvBuf;
	BYTE *privKey;
	BYTE *encData;
	BYTE *decData;

	if(!bIsSM2KeyReady)
	{
		AfxMessageBox("SM2 Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	if(SM2_CURVELEN_INBYTE != (m_sm2_prik.GetLength()/2))
	{
		AfxMessageBox("SM2 private key length error!");
		return;
	}
	if(m_sm2_cipher.GetLength()/2 <= 3*SM2_CURVELEN_INBYTE)
	{
		AfxMessageBox("SM2 encrypt data length error!");
		return;
	}

	privKey = bSendBuf;
	pUSBDlg->StringToBytes(privKey, m_sm2_prik, SM2_CURVELEN_INBYTE, TRUE);

	datalen = 0;

	encData = privKey+SM2_CURVELEN_INBYTE;
	templen = m_sm2_cipher.GetLength()/2;
	pUSBDlg->StringToBytes(encData, m_sm2_cipher, templen, TRUE);
	datalen += templen;

	crypt_req.IP_code = SM2;
	crypt_req.cmd_code = CMD_SM2_DECRYPT;
	crypt_req.key_length = SM2_CURVELEN_INBYTE;
	crypt_req.crypt_length = m_sm2_cipher.GetLength()/2;

	datalen += crypt_req.key_length;
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	templen = crypt_req.crypt_length - 3*SM2_CURVELEN_INBYTE;
	datalen = templen;
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, datalen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	decData = bRecvBuf;
	decData[templen] = 0;
	m_sm2_plain.Format("%s", decData);

	UpdateData(FALSE);
}
