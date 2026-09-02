// RSADlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "RSADlg.h"
#include "UsbDevice.h"	// Added by ClassView
#include "CUSBDlg.h"	// Added by ClassView
#include "crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_RSA_ENC                0x10
#define CMD_RSA_DEC                0x20
#define CMD_RSA_KEYGEN             0x30
#define CMD_RSA_OPMODE_MASK        0xF0

#define CMD_RSA_1024B              0x01
#define CMD_RSA_1024B_CRT          0x02
#define CMD_RSA_2048B              0x03
#define CMD_RSA_2048B_CRT          0x04
#define CMD_RSA_MODE_MASK          0x0F

#define RSA_SUCCESS                0x00

#define RSA_1024B_INBYTE           (1024/8)
#define RSA_2048B_INBYTE           (2048/8)

#define RSA_MAX_CRYPT_LEN          RSA_2048B_INBYTE
#define RSA_MAX_KEYLEN_INBYTE      RSA_2048B_INBYTE

static BOOL bIsRSAKeyReady = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CRSADlg dialog


CRSADlg::CRSADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRSADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRSADlg)
	m_rsa_n = _T("");
	m_rsa_e = 65537;
	m_rsa_d = _T("");
	m_rsa_p = _T("");
	m_rsa_q = _T("");
	m_rsa_dp = _T("");
	m_rsa_dq = _T("");
	m_rsa_qinv = _T("");
	m_rsa_plain = _T("");
	m_rsa_cipher = _T("");
	m_rsa_output = _T("");	
	m_rsa_nlength = -1;
	m_rsa_crt = FALSE;
	//}}AFX_DATA_INIT
}


void CRSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRSADlg)
	DDX_Text(pDX, IDC_RSA_EDIT_N, m_rsa_n);
	DDX_Text(pDX, IDC_RSA_EDIT_E, m_rsa_e);
	DDV_MinMaxDWord(pDX, m_rsa_e, 0, 4294967295);
	DDX_Text(pDX, IDC_RSA_EDIT_D, m_rsa_d);
	DDX_Text(pDX, IDC_RSA_EDIT_P, m_rsa_p);
	DDX_Text(pDX, IDC_RSA_EDIT_Q, m_rsa_q);
	DDX_Text(pDX, IDC_RSA_EDIT_DP, m_rsa_dp);
	DDX_Text(pDX, IDC_RSA_EDIT_DQ, m_rsa_dq);
	DDX_Text(pDX, IDC_RSA_EDIT_QINV, m_rsa_qinv);
	DDX_Text(pDX, IDC_RSA_EDIT_PLAIN, m_rsa_plain);
	DDX_Text(pDX, IDC_RSA_EDIT_CIPHER, m_rsa_cipher);
	DDX_Text(pDX, IDC_RSA_EDIT_OUTPUT, m_rsa_output);	
	DDX_Radio(pDX, IDC_RSA_RADIO_1024BIT, m_rsa_nlength);
	DDX_Check(pDX, IDC_RSA_CHECK_CRT, m_rsa_crt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRSADlg, CDialog)
	//{{AFX_MSG_MAP(CRSADlg)
	ON_BN_CLICKED(IDC_RSA_BUTTON_KEYGEN, OnRSAButtonKeyGen)
	ON_BN_CLICKED(IDC_RSA_BUTTON_ENC, OnRSAButtonEnc)
	ON_BN_CLICKED(IDC_RSA_BUTTON_DEC, OnRSAButtonDec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSADlg message handlers

void CRSADlg::OnRSAButtonKeyGen() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	DWORD rsaLen;
	DWORD keyLen;
	DWORD tempLen;
	UINT  OutputLength;
	BYTE  mode;
	BYTE  bCryptBuf[6*RSA_MAX_KEYLEN_INBYTE];
	BYTE  *bSendBuf;
	BYTE  *bRecvBuf;
	BYTE  *rsaN;
	BYTE  *rsaD;
	BYTE  *rsaP;
	BYTE  *rsaQ;
	BYTE  *rsaDp;
	BYTE  *rsaDq;
	BYTE  *rsaQInv;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	// set mode
	if(m_rsa_nlength == -1)
	{
		AfxMessageBox("Please Select Key Length!");
		return;
	}
	else if(m_rsa_nlength == 0)
	{
		rsaLen = RSA_1024B_INBYTE;
		if(m_rsa_crt)
		{
			mode = CMD_RSA_1024B_CRT;
			keyLen = (RSA_1024B_INBYTE/2)*9;
		}
		else
		{
			mode = CMD_RSA_1024B;
			keyLen = RSA_1024B_INBYTE*2;
		}
	}
	else
	{
		rsaLen = RSA_2048B_INBYTE;
		if(m_rsa_crt)
		{
			mode = CMD_RSA_2048B_CRT;
			keyLen = (RSA_2048B_INBYTE/2)*9;
		}
		else
		{
			mode = CMD_RSA_2048B;
			keyLen = RSA_2048B_INBYTE*2;
		}
	}

	if(m_rsa_e == 0)
	{
		AfxMessageBox("RSA_E invalid!");
		return;
	}

	crypt_req.IP_code = RSA;
	crypt_req.cmd_code = CMD_RSA_KEYGEN | mode;
	crypt_req.key_length = 4;  //key_length must be 4bytes-aligned, if key_length != 0, after generated, keys should be return back, else nothing will be returned.
	crypt_req.crypt_length = 4;

	tempLen = crypt_req.key_length;

	(bSendBuf+tempLen)[0] = (BYTE)((m_rsa_e >> 24) & 0xFF);
	(bSendBuf+tempLen)[1] = (BYTE)((m_rsa_e >> 16) & 0xFF);
	(bSendBuf+tempLen)[2] = (BYTE)((m_rsa_e >> 8) & 0xFF);
	(bSendBuf+tempLen)[3] = (BYTE)((m_rsa_e >> 0) & 0xFF);
	tempLen += crypt_req.crypt_length;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, tempLen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);

		bIsRSAKeyReady = FALSE;
		return;
	}

	// read key
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, keyLen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);

		bIsRSAKeyReady = FALSE;
		return;
	}
	AfxMessageBox("RSA KeyGen Successfully!");
	bIsRSAKeyReady = TRUE;

	rsaN = bRecvBuf;
	rsaD = rsaN + rsaLen;
	pUSBDlg->ShowReceivedData(m_rsa_n, rsaN, rsaLen);
	pUSBDlg->ShowReceivedData(m_rsa_d, rsaD, rsaLen);
	switch(mode)
	{
	case CMD_RSA_2048B_CRT:
	case CMD_RSA_1024B_CRT:
		rsaP = rsaD + rsaLen;
		rsaLen /= 2;
		rsaQ = rsaP + rsaLen;
		rsaDp = rsaQ + rsaLen;
		rsaDq = rsaDp + rsaLen;
		rsaQInv = rsaDq + rsaLen;
		pUSBDlg->ShowReceivedData(m_rsa_p, rsaP, rsaLen);
		pUSBDlg->ShowReceivedData(m_rsa_q, rsaQ, rsaLen);
		pUSBDlg->ShowReceivedData(m_rsa_dp, rsaDp, rsaLen);
		pUSBDlg->ShowReceivedData(m_rsa_dq, rsaDq, rsaLen);
		pUSBDlg->ShowReceivedData(m_rsa_qinv, rsaQInv, rsaLen);
		break;
	case CMD_RSA_2048B:
	case CMD_RSA_1024B:
		m_rsa_p.Empty();
		m_rsa_q.Empty();
		m_rsa_dp.Empty();
		m_rsa_dq.Empty();
		m_rsa_qinv.Empty();
		break;
	default:
		break;
	}

	UpdateData(FALSE);
}

void CRSADlg::OnRSAButtonEnc() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	DWORD rsaLen;
	DWORD tempLen;
	UINT  OutputLength;
	BYTE  mode;
	BYTE  bCryptBuf[6*RSA_MAX_KEYLEN_INBYTE+RSA_MAX_CRYPT_LEN];
	BYTE  *bSendBuf;
	BYTE  *bRecvBuf;
	BYTE  *rsaOut;

	if(!bIsRSAKeyReady)
	{
		AfxMessageBox("RSA Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	// set mode
	if(m_rsa_nlength == -1)
	{
		AfxMessageBox("Please Select Key Length!");
		return;
	}
	else if(m_rsa_nlength == 0)
	{
		rsaLen = RSA_1024B_INBYTE;
		mode = CMD_RSA_1024B;
	}
	else
	{
		rsaLen = RSA_2048B_INBYTE;
		mode = CMD_RSA_2048B;
	}

	// verify and set key
	tempLen = 0;
	if(m_rsa_n.GetLength() != (int)(2*rsaLen))
	{
		AfxMessageBox("RSA_N length invalid!");
		return;
	}
	else
	{
		pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_n, rsaLen, TRUE);
		tempLen += rsaLen;
	}

	if(m_rsa_e == 0)
	{
		AfxMessageBox("RSA_E invalid!");
		return;
	}
	else
	{
		(bSendBuf+tempLen)[0] = (BYTE)((m_rsa_e >> 24) & 0xFF);
		(bSendBuf+tempLen)[1] = (BYTE)((m_rsa_e >> 16) & 0xFF);
		(bSendBuf+tempLen)[2] = (BYTE)((m_rsa_e >> 8) & 0xFF);
		(bSendBuf+tempLen)[3] = (BYTE)((m_rsa_e >> 0) & 0xFF);
		tempLen += 4;
	}

	if((m_rsa_plain.GetLength() == 0) || ((DWORD)m_rsa_plain.GetLength() > (2*rsaLen)))
	{
		CString error;
		error.Format("Plain should be between 1 and %d bytes!", rsaLen);
		AfxMessageBox(error);
		return;
	}
	else if(m_rsa_plain.GetLength() % 8)
	{
		CString strtemp('0', (8 - (m_rsa_plain.GetLength() % 8)));
		m_rsa_plain = strtemp + m_rsa_plain;
	}

	crypt_req.IP_code = RSA;
	crypt_req.cmd_code = CMD_RSA_ENC | mode;
	crypt_req.key_length = (USHORT)(tempLen);
	crypt_req.crypt_length = ((DWORD)m_rsa_plain.GetLength()+1) / 2;

	pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_plain, crypt_req.crypt_length, TRUE);
	tempLen += crypt_req.crypt_length;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, tempLen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// read ciphertext
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, rsaLen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	rsaOut = bRecvBuf;
	pUSBDlg->ShowReceivedData(m_rsa_output, rsaOut, OutputLength);

	UpdateData(FALSE);
}

void CRSADlg::OnRSAButtonDec() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	DWORD rsaLen;
	DWORD tempLen;
	UINT  OutputLength;
	BYTE  mode;
	BYTE  bCryptBuf[6*RSA_MAX_KEYLEN_INBYTE+RSA_MAX_CRYPT_LEN];
	BYTE  *bSendBuf;
	BYTE  *bRecvBuf;
	BYTE  *rsaOut;

	if(!bIsRSAKeyReady)
	{
		AfxMessageBox("RSA Key is Not Generated!");
		return;
	}

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;	
	bSendBuf = bCryptBuf;
	bRecvBuf = bCryptBuf;

	UpdateData(TRUE);

	// set mode
	if(m_rsa_nlength == -1)
	{
		AfxMessageBox("Please Select Key Length!");
		return;
	}
	else if(m_rsa_nlength == 0)
	{
		rsaLen = RSA_1024B_INBYTE;
		if(m_rsa_crt)
		{
			mode = CMD_RSA_1024B_CRT;
		}
		else
		{
			mode = CMD_RSA_1024B;
		}
	}
	else
	{
		rsaLen = RSA_2048B_INBYTE;
		if(m_rsa_crt)
		{
			mode = CMD_RSA_2048B_CRT;
		}
		else
		{
			mode = CMD_RSA_2048B;
		}
	}

	// verify and set key
	tempLen = 0;
	if(m_rsa_n.GetLength() != (int)(2*rsaLen))
	{
		AfxMessageBox("RSA_N length invalid!");
		return;
	}
	else
	{
		pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_n, rsaLen, TRUE);
		tempLen += rsaLen;
	}

	if(m_rsa_d.GetLength() != (int)(2*rsaLen))
	{
		AfxMessageBox("RSA_D length invalid!");
		return;
	}
	else
	{
		pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_d, rsaLen, TRUE);
		tempLen += rsaLen;
	}

	if(m_rsa_crt)
	{
		if(m_rsa_e == 0)
		{
			AfxMessageBox("RSA_E invalid!");
			return;
		}
		else
		{
			(bSendBuf+tempLen)[0] = (BYTE)((m_rsa_e >> 24) & 0xFF);
			(bSendBuf+tempLen)[1] = (BYTE)((m_rsa_e >> 16) & 0xFF);
			(bSendBuf+tempLen)[2] = (BYTE)((m_rsa_e >> 8) & 0xFF);
			(bSendBuf+tempLen)[3] = (BYTE)((m_rsa_e >> 0) & 0xFF);
			tempLen += 4;
		}

		if(m_rsa_p.GetLength() != (int)(rsaLen))
		{
			AfxMessageBox("RSA_P length invalid!");
			return;
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_p, rsaLen/2, TRUE);
			tempLen += rsaLen / 2;
		}

		if(m_rsa_q.GetLength() != (int)(rsaLen))
		{
			AfxMessageBox("RSA_Q length invalid!");
			return;
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_q, rsaLen/2, TRUE);
			tempLen += rsaLen / 2;
		}

		if(m_rsa_dp.GetLength() != (int)(rsaLen))
		{
			AfxMessageBox("RSA_Dp length invalid!");
			return;
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_dp, rsaLen/2, TRUE);
			tempLen += rsaLen / 2;
		}

		if(m_rsa_dq.GetLength() != (int)(rsaLen))
		{
			AfxMessageBox("RSA_Dq length invalid!");
			return;
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_dq, rsaLen/2, TRUE);
			tempLen += rsaLen / 2;
		}

		if(m_rsa_qinv.GetLength() != (int)(rsaLen))
		{
			AfxMessageBox("RSA_QInv length invalid!");
			return;
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_qinv, rsaLen/2, TRUE);
			tempLen += rsaLen / 2;
		}
	}

	if((m_rsa_cipher.GetLength() == 0) || ((DWORD)m_rsa_cipher.GetLength() > (2*rsaLen)))
	{
		CString error;
		error.Format("Cipher should be between 1 and %d bytes!", rsaLen);
		AfxMessageBox(error);
		return;
	}
	else if(m_rsa_cipher.GetLength() % 8)
	{
		CString strtemp('0', (8 - (m_rsa_plain.GetLength() % 8)));
		m_rsa_cipher = strtemp + m_rsa_cipher;
	}

	crypt_req.IP_code = RSA;
	crypt_req.cmd_code = CMD_RSA_DEC | mode;
	crypt_req.key_length = (USHORT)(tempLen);
	crypt_req.crypt_length = ((DWORD)m_rsa_cipher.GetLength()+1) / 2;
	
	pUSBDlg->StringToBytes(bSendBuf+tempLen, m_rsa_cipher, crypt_req.crypt_length, TRUE);
	tempLen += crypt_req.crypt_length;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, tempLen, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// read plaintext
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, rsaLen, bRecvBuf, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	rsaOut = bRecvBuf;
	pUSBDlg->ShowReceivedData(m_rsa_output, rsaOut, OutputLength);

	UpdateData(FALSE);
}
