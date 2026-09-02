// SSF33Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "SSF33Dlg.h"
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
#define CMD_SSF33_ENC              0x10
#define CMD_SSF33_DEC              0x20
#define CMD_SSF33_OPMODE_MASK      0xF0

#define CMD_SSF33_ECB_INNERPARA    0x01
#define CMD_SSF33_ECB_OUTERPARA    0x02
#define CMD_SSF33_CBC_INNERPARA    0x03
#define CMD_SSF33_CBC_OUTERPARA    0x04
#define CMD_SSF33_MODE_MASK        0x0F

#define SSF33_SUCCESS              0x00
#define SSF33_MAX_CRYPT_LEN        4096
#define SSF33_MAX_KEY_LEN          48

/////////////////////////////////////////////////////////////////////////////
// CSSF33Dlg dialog


CSSF33Dlg::CSSF33Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSF33Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSF33Dlg)	
	m_ssf33_para = _T("");
	m_ssf33_key = _T("64C04B2049C8235549443C76A1C1B789");
	m_ssf33_iv = _T("00000000000000000000000000000000");
	m_ssf33_plain = _T("24800B600988631509047C36E181F7C9");
	m_ssf33_cipher = _T("2F11ECDC384A99737C08536BDBAE1F16");
	m_ssf33_output = _T("");
	m_ssf33_mode = -1;
	m_ssf33_skmode = -1;
	//}}AFX_DATA_INIT
}


void CSSF33Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSF33Dlg)	
	DDX_Text(pDX, IDC_SSF33_EDIT_PARA, m_ssf33_para);
	DDX_Text(pDX, IDC_SSF33_EDIT_KEY, m_ssf33_key);
	DDX_Text(pDX, IDC_SSF33_EDIT_IV, m_ssf33_iv);
	DDX_Text(pDX, IDC_SSF33_EDIT_PLAIN, m_ssf33_plain);
	DDX_Text(pDX, IDC_SSF33_EDIT_CIPHER, m_ssf33_cipher);
	DDX_Text(pDX, IDC_SSF33_EDIT_OUTPUT, m_ssf33_output);
	DDX_Radio(pDX, IDC_SSF33_RADIO_ECB, m_ssf33_mode);
	DDX_Radio(pDX, IDC_SSF33_RADIO_INNER, m_ssf33_skmode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSSF33Dlg, CDialog)
	//{{AFX_MSG_MAP(CSSF33Dlg)
	ON_BN_CLICKED(IDC_SSF33_BUTTON_ENC, OnSSF33ButtonEnc)
	ON_BN_CLICKED(IDC_SSF33_BUTTON_DEC, OnSSF33ButtonDec)
	ON_BN_CLICKED(IDC_SSF33_BUTTON_ENCFILE, OnSSF33ButtonEncFile)
	ON_BN_CLICKED(IDC_SSF33_BUTTON_DECFILE, OnSSF33ButtonDecFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSF33Dlg message handlers

void CSSF33Dlg::OnSSF33ButtonEnc() 
{
	// TODO: Add your control notification handler code here
	BYTE  bPlaintext[SSF33_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_ssf33_plain.GetLength() % 32)
	{
		AfxMessageBox("Please Input Plain in complete Blocks!");
		return;
	}
	else
	{
		dPlainLen = m_ssf33_plain.GetLength() / 2;
		pUSBDlg->StringToBytes(bPlaintext, m_ssf33_plain, dPlainLen, TRUE);
	}

	if(SSF33_SUCCESS != ssf33_crypt(pUSBDlg, CMD_SSF33_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
	{
		AfxMessageBox("SSF33 Encrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_ssf33_output, bRecvBuf, dCipherLen);

	UpdateData(FALSE);
}

void CSSF33Dlg::OnSSF33ButtonDec() 
{
	// TODO: Add your control notification handler code here
	BYTE  bCiphertext[SSF33_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_ssf33_cipher.GetLength() % 32)
	{
		AfxMessageBox("Please Input Cipher in complete Blocks!");
		return;
	}
	else
	{
		dCipherLen = m_ssf33_cipher.GetLength() / 2;
		pUSBDlg->StringToBytes(bCiphertext, m_ssf33_cipher, dCipherLen, TRUE);
	}

	if(SSF33_SUCCESS != ssf33_crypt(pUSBDlg, CMD_SSF33_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
	{
		AfxMessageBox("SSF33 Decrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_ssf33_output, bRecvBuf, dPlainLen);

	UpdateData(FALSE);
}

void CSSF33Dlg::OnSSF33ButtonEncFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bPlaintext[SSF33_MAX_CRYPT_LEN];
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
		if(!(dPlainLen = m_inputfile.Read(bPlaintext, SSF33_MAX_CRYPT_LEN)))
			break;		
		if(dPlainLen < SSF33_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dPlainLen % 16);
			if(dPadLength != 16)
			{
				memset(bPlaintext+dPlainLen, ' ', dPadLength);
				dPlainLen += dPadLength;
			}
		}

		// Encrypt
		if(SSF33_SUCCESS != ssf33_crypt(pUSBDlg, CMD_SSF33_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
		{
			AfxMessageBox("SSF33 Encrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dCipherLen);

		if(dCipherLen < SSF33_MAX_CRYPT_LEN)
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

void CSSF33Dlg::OnSSF33ButtonDecFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bCiphertext[SSF33_MAX_CRYPT_LEN];
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
		if(!(dCipherLen = m_inputfile.Read(bCiphertext, SSF33_MAX_CRYPT_LEN)))
			break;		
		if(dCipherLen < SSF33_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dCipherLen % 16);
			if(dPadLength != 16)
			{
				memset(bCiphertext+dCipherLen, ' ', dPadLength);
				dCipherLen += dPadLength;
			}
		}

		// Decrypt
		if(SSF33_SUCCESS != ssf33_crypt(pUSBDlg, CMD_SSF33_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
		{
			AfxMessageBox("SSF33 Decrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dPlainLen);

		if(dPlainLen < SSF33_MAX_CRYPT_LEN)
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

int CSSF33Dlg::ssf33_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut)
{
	CRYPT_REQUEST crypt_req;
	USHORT  key_len;
	UINT    OutputLength;
	BYTE    bSendBuf[SSF33_MAX_CRYPT_LEN+SSF33_MAX_KEY_LEN];

	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	// Verify key setting under rules or not
	switch(m_ssf33_skmode)
	{
	case -1:
		{
			AfxMessageBox("Please Select Parameter Mode!");
			return (-1);
		}
	case 1:
		if(m_ssf33_para.GetLength() != 32)
		{
			AfxMessageBox("Please Input 128bits Parameter!");
			return (-1);
		}
	case 0:
		if(m_ssf33_key.GetLength() != 32)
		{
			AfxMessageBox("Please Input 128bits KEY!");
			return (-1);
		}
		break;
	default:
		{
			AfxMessageBox("Key mode error!");
			return (-1);
		}
	}
	
	// Set mode
	key_len = 16*((USHORT)m_ssf33_skmode + 1);
	switch(m_ssf33_mode)
	{
	case -1:
		{
			AfxMessageBox("Please Select Crypt Mode!");
			return (-1);
		}
		break;
	case 1:
		if(m_ssf33_iv.GetLength() != 32)
		{
			AfxMessageBox("Please Input 128bits IV!");
			return (-1);
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+key_len, m_ssf33_iv, 16, TRUE);
			key_len += 16;
		}

		switch(m_ssf33_skmode)
		{
		case 1:
			crypt_req.cmd_code = CMD_SSF33_CBC_OUTERPARA;
			break;
		case 0:
			crypt_req.cmd_code = CMD_SSF33_CBC_INNERPARA;
			break;
		default:
			{
				AfxMessageBox("Mode not support!");
				return (-1);
			}
			break;
		}

		break;
	case 0:
		switch(m_ssf33_skmode)
		{
		case 1:
			crypt_req.cmd_code = CMD_SSF33_ECB_OUTERPARA;
			break;
		case 0:
			crypt_req.cmd_code = CMD_SSF33_ECB_INNERPARA;
			break;
		default:
			{
				AfxMessageBox("Mode not support!");
				return (-1);
			}
			break;
		}

		break;
	default:
		{
			AfxMessageBox("Mode not support!");
			return (-1);
		}
		break;
	}
	
	// Get and fulfill key to send buffer
	switch(crypt_req.cmd_code)
	{
	case CMD_SSF33_CBC_OUTERPARA:
	case CMD_SSF33_ECB_OUTERPARA:
		pUSBDlg->StringToBytes(bSendBuf+16, m_ssf33_para, 16, TRUE);	
	case CMD_SSF33_CBC_INNERPARA:
	case CMD_SSF33_ECB_INNERPARA:
		pUSBDlg->StringToBytes(bSendBuf, m_ssf33_key, 16, TRUE);
		break;
	default:
		{
			AfxMessageBox("Mode not support!");
			return (-1);
		}
		break;
	}

    crypt_req.IP_code = SSF33;
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

	return SSF33_SUCCESS;
}
