// SM1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "SM1Dlg.h"
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
#define CMD_SM1_ENC                0x10
#define CMD_SM1_DEC                0x20
#define CMD_SM1_OPMODE_MASK        0xF0

#define CMD_SM1_ECB_INNERPARA      0x01
#define CMD_SM1_ECB_OUTERPARA      0x02
#define CMD_SM1_CBC_INNERPARA      0x03
#define CMD_SM1_CBC_OUTERPARA      0x04
#define CMD_SM1_MODE_MASK          0x0F

#define SM1_SUCCESS                0x00
#define SM1_MAX_CRYPT_LEN          4096
#define SM1_MAX_KEY_LEN            64

/////////////////////////////////////////////////////////////////////////////
// CSM1Dlg dialog


CSM1Dlg::CSM1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSM1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSM1Dlg)
	m_sm1_ek = _T("40BB12DD6A8273867F3529D354B4A026");
	m_sm1_ak = _T("0C90E15A4F9236B20000000000000000");
	m_sm1_sk = _T("A291B3EBA4ED225F1CEAA607628FB236");
	m_sm1_iv = _T("00000000000000000000000000000000");
	m_sm1_plain = _T("12153524C0895E818484D609B1F05663");
	m_sm1_cipher = _T("8F8F4B94E6FABF8011AF390454FC9EF7");
	m_sm1_output = _T("");
	m_sm1_mode = -1;
	m_sm1_skmode = -1;	
	//}}AFX_DATA_INIT
}


void CSM1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSM1Dlg)
	DDX_Text(pDX, IDC_SM1_EDIT_EK, m_sm1_ek);
	DDX_Text(pDX, IDC_SM1_EDIT_AK, m_sm1_ak);
	DDX_Text(pDX, IDC_SM1_EDIT_SK, m_sm1_sk);
	DDX_Text(pDX, IDC_SM1_EDIT_IV, m_sm1_iv);
	DDX_Text(pDX, IDC_SM1_EDIT_PLAIN, m_sm1_plain);
	DDX_Text(pDX, IDC_SM1_EDIT_CIPHER, m_sm1_cipher);
	DDX_Text(pDX, IDC_SM1_EDIT_OUTPUT, m_sm1_output);
	DDX_Radio(pDX, IDC_SM1_RADIO_ECB, m_sm1_mode);
	DDX_Radio(pDX, IDC_SM1_RADIO_INNER, m_sm1_skmode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSM1Dlg, CDialog)
	//{{AFX_MSG_MAP(CSM1Dlg)
	ON_BN_CLICKED(IDC_SM1_BUTTON_ENC, OnSM1ButtonEnc)
	ON_BN_CLICKED(IDC_SM1_BUTTON_DEC, OnSM1ButtonDec)
	ON_BN_CLICKED(IDC_SM1_BUTTON_ENCFILE, OnSM1ButtonEncFile)
	ON_BN_CLICKED(IDC_SM1_BUTTON_DECFILE, OnSM1ButtonDecFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSM1Dlg message handlers

void CSM1Dlg::OnSM1ButtonEnc() 
{
	// TODO: Add your control notification handler code here
	BYTE  bPlaintext[SM1_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_sm1_plain.GetLength() % 32)
	{
		AfxMessageBox("Please Input Plain in complete Blocks!");
		return;
	}
	else
	{
		dPlainLen = m_sm1_plain.GetLength() / 2;
		pUSBDlg->StringToBytes(bPlaintext, m_sm1_plain, dPlainLen, TRUE);
	}

	if(SM1_SUCCESS != sm1_crypt(pUSBDlg, CMD_SM1_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
	{
		AfxMessageBox("SM1 Encrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_sm1_output, bRecvBuf, dCipherLen);

	UpdateData(FALSE);
}

void CSM1Dlg::OnSM1ButtonDec() 
{
	// TODO: Add your control notification handler code here
	BYTE  bCiphertext[SM1_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_sm1_cipher.GetLength() % 32)
	{
		AfxMessageBox("Please Input Cipher in complete Blocks!");
		return;
	}
	else
	{
		dCipherLen = m_sm1_cipher.GetLength() / 2;
		pUSBDlg->StringToBytes(bCiphertext, m_sm1_cipher, dCipherLen, TRUE);
	}

	if(SM1_SUCCESS != sm1_crypt(pUSBDlg, CMD_SM1_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
	{
		AfxMessageBox("SM1 Decrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_sm1_output, bRecvBuf, dPlainLen);

	UpdateData(FALSE);
}

void CSM1Dlg::OnSM1ButtonEncFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bPlaintext[SM1_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);
	(CWnd*)pUSBDlg->UpdateData(TRUE);

	m_inputfile.Abort();
	m_outputfile.Abort();
	m_inputfile.Open(pUSBDlg->m_inputfilepath, CFile::modeRead | CFile::shareDenyWrite);
	m_outputfile.Open(pUSBDlg->m_outputfilepath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
	
	while(1)
	{
		// Read file
		if(!(dPlainLen = m_inputfile.Read(bPlaintext, SM1_MAX_CRYPT_LEN)))
			break;		
		if(dPlainLen < SM1_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dPlainLen % 16);
			if(dPadLength != 16)
			{
				memset(bPlaintext+dPlainLen, ' ', dPadLength);
				dPlainLen += dPadLength;
			}
		}

		// Encrypt
		if(SM1_SUCCESS != sm1_crypt(pUSBDlg, CMD_SM1_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
		{
			AfxMessageBox("SM1 Encrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dCipherLen);

		if(dCipherLen < SM1_MAX_CRYPT_LEN)
			break;
	}

	m_inputfile.Close();
	m_outputfile.Close();

	CString result;
	result.Format("文件加密完成!");
	AfxMessageBox(result);
	
	UpdateData(FALSE);
	(CWnd*)pUSBDlg->UpdateData(FALSE);
}

void CSM1Dlg::OnSM1ButtonDecFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bCiphertext[SM1_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);
	(CWnd*)pUSBDlg->UpdateData(TRUE);

	m_inputfile.Abort();
	m_outputfile.Abort();
	m_inputfile.Open(pUSBDlg->m_inputfilepath, CFile::modeRead | CFile::shareDenyWrite);
	m_outputfile.Open(pUSBDlg->m_outputfilepath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
	
	while(1)
	{
		// Read file
		if(!(dCipherLen = m_inputfile.Read(bCiphertext, SM1_MAX_CRYPT_LEN)))
			break;		
		if(dCipherLen < SM1_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dCipherLen % 16);
			if(dPadLength != 16)
			{
				memset(bCiphertext+dCipherLen, ' ', dPadLength);
				dCipherLen += dPadLength;
			}
		}

		// Decrypt
		if(SM1_SUCCESS != sm1_crypt(pUSBDlg, CMD_SM1_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
		{
			AfxMessageBox("SM1 Decrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dPlainLen);

		if(dPlainLen < SM1_MAX_CRYPT_LEN)
			break;
	}

	m_inputfile.Close();
	m_outputfile.Close();

	CString result;
	result.Format("文件解密完成!");
	AfxMessageBox(result);
	
	UpdateData(FALSE);
	(CWnd*)pUSBDlg->UpdateData(FALSE);
}

int CSM1Dlg::sm1_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut)
{
	CRYPT_REQUEST crypt_req;
	USHORT  key_len;
	UINT    OutputLength;
	BYTE    bSendBuf[SM1_MAX_CRYPT_LEN+SM1_MAX_KEY_LEN];

	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	// Verify key setting under rules or not
	if(m_sm1_skmode == -1)
	{
		AfxMessageBox("Please Select Key Mode!");
		return (-1);
	}
	if(m_sm1_mode == -1)
	{
		AfxMessageBox("Please Select Crypt Mode!");
		return (-1);
	}
	if (m_sm1_ek.GetLength() != 32)
	{
		AfxMessageBox("Please Input 128bits EK!");
		return (-1);
	}
	if (m_sm1_ak.GetLength() != 32)
	{
		AfxMessageBox("Please Input 128bits AK!");
		return (-1);
	}
	if (m_sm1_sk.GetLength() != 32)
	{
		AfxMessageBox("Please Input 128bits SK!");
		return (-1);
	}
	
	// Set mode
	switch(m_sm1_skmode + m_sm1_mode)
	{
	case 2:
		crypt_req.cmd_code = CMD_SM1_CBC_OUTERPARA;
		break;
	case 1:
		if(m_sm1_mode == 1)
		{
			crypt_req.cmd_code = CMD_SM1_CBC_INNERPARA;
		}
		else
		{
			crypt_req.cmd_code = CMD_SM1_ECB_OUTERPARA;
		}

		break;
	case 0:
		crypt_req.cmd_code = CMD_SM1_ECB_INNERPARA;
		break;
	default:
		{
			AfxMessageBox("Mode not support!");
			return (-1);
		}
		break;
	}
	
	// Get and fulfill key to send buffer
	key_len = 16*3;
	switch(crypt_req.cmd_code)
	{
	case CMD_SM1_CBC_OUTERPARA:
	case CMD_SM1_CBC_INNERPARA:
		if(m_sm1_iv.GetLength() != 32)
		{
			AfxMessageBox("Please Input 128bits IV!");
			return (-1);
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+key_len, m_sm1_iv, 16, TRUE);
			key_len += 16;
		}	
	case CMD_SM1_ECB_OUTERPARA:
	case CMD_SM1_ECB_INNERPARA:
		pUSBDlg->StringToBytes(bSendBuf, m_sm1_ek, 16, TRUE);
		pUSBDlg->StringToBytes(bSendBuf+16, m_sm1_ak, 16, TRUE);
		pUSBDlg->StringToBytes(bSendBuf+32, m_sm1_sk, 16, TRUE);
		break;
	default:
		{
			AfxMessageBox("Mode not support!");
			return (-1);
		}
		break;
	}

    crypt_req.IP_code = SM1;
	crypt_req.cmd_code |= operation;
	crypt_req.key_length = key_len;
	crypt_req.crypt_length = nBytesCryptIn;

	// Fulfill crypt data to buffer
	memcpy(bSendBuf+key_len, bCryptIn, nBytesCryptIn);

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, key_len+nBytesCryptIn, bSendBuf, &OutputLength))
	{
		CString error;
		error.Format("写错误，错误号(%d)，请调试!", GetLastError());
		AfxMessageBox(error);
		return (-1);
	}

	// read output
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, nBytesCryptIn, bCryptOut, &OutputLength))
	{
		CString error;
		error.Format("读错误，错误号(%d)，请调试!", GetLastError());
		AfxMessageBox(error);
		return (-1);
	}

	*nBytesCryptOut = OutputLength;

	return SM1_SUCCESS;
}
