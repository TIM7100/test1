// DESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CUSB.h"
#include "DESDlg.h"
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
#define CMD_DES_ENC                0x10
#define CMD_DES_DEC                0x20
#define CMD_DES_OPMODE_MASK        0xF0

#define CMD_DES_ECB_KEYONE         0x01
#define CMD_DES_ECB_KEYTWO         0x02
#define CMD_DES_ECB_KEYTHREE       0x03
#define CMD_DES_CBC_KEYONE         0x04
#define CMD_DES_CBC_KEYTWO         0x05
#define CMD_DES_CBC_KEYTHREE       0x06
#define CMD_DES_MODE_MASK          0x0F

#define DES_SUCCESS                0x00
#define DES_MAX_CRYPT_LEN          4096
#define DES_MAX_KEY_LEN            32

/////////////////////////////////////////////////////////////////////////////
// CDESDlg dialog


CDESDlg::CDESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDESDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDESDlg)
	m_des_key1 = _T("0804020180402010");
	m_des_key2 = _T("375402f7384a6bc2");
	m_des_key3 = _T("15c8bae91073cd02");
	m_des_iv = _T("0000000000000000");
	m_des_plain = _T("000102030405060708090a0b0c0d0e0f1011121314151617");
	m_des_cipher = _T("E6BF9C4B2D5C84ED0271AC28B63AF8983FF9DAB9BC4DAE32");
	m_des_output = _T("");
	m_des_mode = -1;
	m_des_keynum = -1;
	//}}AFX_DATA_INIT
}


void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDESDlg)
	DDX_Text(pDX, IDC_DES_EDIT_KEY1, m_des_key1);
	DDX_Text(pDX, IDC_DES_EDIT_KEY2, m_des_key2);
	DDX_Text(pDX, IDC_DES_EDIT_KEY3, m_des_key3);
	DDX_Text(pDX, IDC_DES_EDIT_IV, m_des_iv);
	DDX_Text(pDX, IDC_DES_EDIT_PLAIN, m_des_plain);
	DDX_Text(pDX, IDC_DES_EDIT_CIPHER, m_des_cipher);
	DDX_Text(pDX, IDC_DES_EDIT_OUTPUT, m_des_output);
	DDX_Radio(pDX, IDC_DES_RADIO_ECB, m_des_mode);
	DDX_Radio(pDX, IDC_DES_RADIO_KEY1, m_des_keynum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDESDlg, CDialog)
	//{{AFX_MSG_MAP(CDESDlg)
	ON_BN_CLICKED(IDC_DES_BUTTON_ENC, OnDESButtonEnc)
	ON_BN_CLICKED(IDC_DES_BUTTON_DEC, OnDESButtonDec)
	ON_BN_CLICKED(IDC_DES_BUTTON_ENCFILE, OnDESButtonEncFile)
	ON_BN_CLICKED(IDC_DES_BUTTON_DECFILE, OnDESButtonDecFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDESDlg message handlers

void CDESDlg::OnDESButtonEnc() 
{
	// TODO: Add your control notification handler code here
	BYTE  bPlaintext[DES_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_des_plain.GetLength() % 16)
	{
		AfxMessageBox("Please Input Plain in complete Blocks!");
		return;
	}
	else
	{
		dPlainLen = m_des_plain.GetLength() / 2;
		pUSBDlg->StringToBytes(bPlaintext, m_des_plain, dPlainLen, TRUE);
	}

	if(DES_SUCCESS != des_crypt(pUSBDlg, CMD_DES_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
	{
		AfxMessageBox("DES Encrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_des_output, bRecvBuf, dCipherLen);

	UpdateData(FALSE);
}


void CDESDlg::OnDESButtonDec() 
{
	// TODO: Add your control notification handler code here
	BYTE  bCiphertext[DES_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_des_cipher.GetLength() % 16)
	{
		AfxMessageBox("Please Input Cipher in complete Blocks!");
		return;
	}
	else
	{
		dCipherLen = m_des_cipher.GetLength() / 2;
		pUSBDlg->StringToBytes(bCiphertext, m_des_cipher, dCipherLen, TRUE);
	}

	if(DES_SUCCESS != des_crypt(pUSBDlg, CMD_DES_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
	{
		AfxMessageBox("DES Decrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_des_output, bRecvBuf, dPlainLen);

	UpdateData(FALSE);
}

void CDESDlg::OnDESButtonEncFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bPlaintext[DES_MAX_CRYPT_LEN];
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
		if(!(dPlainLen = m_inputfile.Read(bPlaintext, DES_MAX_CRYPT_LEN)))
			break;		
		if(dPlainLen < DES_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dPlainLen % 16);
			if(dPadLength != 16)
			{
				memset(bPlaintext+dPlainLen, ' ', dPadLength);
				dPlainLen += dPadLength;
			}
		}

		// Encrypt
		if(DES_SUCCESS != des_crypt(pUSBDlg, CMD_DES_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
		{
			AfxMessageBox("DES Encrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dCipherLen);

		if(dCipherLen < DES_MAX_CRYPT_LEN)
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

void CDESDlg::OnDESButtonDecFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bCiphertext[DES_MAX_CRYPT_LEN];
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
		if(!(dCipherLen = m_inputfile.Read(bCiphertext, DES_MAX_CRYPT_LEN)))
			break;		
		if(dCipherLen < DES_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dCipherLen % 16);
			if(dPadLength != 16)
			{
				memset(bCiphertext+dCipherLen, ' ', dPadLength);
				dCipherLen += dPadLength;
			}
		}

		// Decrypt
		if(DES_SUCCESS != des_crypt(pUSBDlg, CMD_DES_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
		{
			AfxMessageBox("DES Decrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dPlainLen);

		if(dPlainLen < DES_MAX_CRYPT_LEN)
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

int CDESDlg::des_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut)
{
	CRYPT_REQUEST crypt_req;
	USHORT  key_len;
	UINT    OutputLength;
	BYTE    bSendBuf[DES_MAX_CRYPT_LEN+DES_MAX_KEY_LEN];

	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	// Verify key setting under rules or not
	switch(m_des_keynum)
	{
	case -1:
		{
			AfxMessageBox("Please Select Key Number!");
			return (-1);
		}
	case 2:
		if(m_des_key3.GetLength() != 16)
		{
			AfxMessageBox("Please Input 64bits KEY-3!");
			return (-1);
		}
	case 1:
		if(m_des_key2.GetLength() != 16)
		{
			AfxMessageBox("Please Input 64bits KEY-2!");
			return (-1);
		}
	case 0:
		if(m_des_key1.GetLength() != 16)
		{
			AfxMessageBox("Please Input 64bits KEY-1!");
			return (-1);
		}
		break;
	default:
		if((m_des_keynum > 2) || (m_des_keynum < -1))
		{
			AfxMessageBox("Key number error!");
			return (-1);
		}
		break;
	}
	
	// Set mode
	key_len = 8*((USHORT)m_des_keynum + 1);
	switch(m_des_mode)
	{
	case -1:
		{
			AfxMessageBox("Please Select Crypt Mode!");
			return (-1);
		}
		break;
	case 1:
		if(m_des_iv.GetLength() != 16)
		{
			AfxMessageBox("Please Input 64bits IV!");
			return (-1);
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+key_len, m_des_iv, 8, TRUE);
			key_len += 8;
		}

		switch(m_des_keynum)
		{
		case 2:
			crypt_req.cmd_code = CMD_DES_CBC_KEYTHREE;
			break;
		case 1:
			crypt_req.cmd_code = CMD_DES_CBC_KEYTWO;
			break;
		case 0:
			crypt_req.cmd_code = CMD_DES_CBC_KEYONE;
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
		switch(m_des_keynum)
		{
		case 2:
			crypt_req.cmd_code = CMD_DES_ECB_KEYTHREE;
			break;
		case 1:
			crypt_req.cmd_code = CMD_DES_ECB_KEYTWO;
			break;
		case 0:
			crypt_req.cmd_code = CMD_DES_ECB_KEYONE;
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
	case CMD_DES_CBC_KEYTHREE:
	case CMD_DES_ECB_KEYTHREE:
		pUSBDlg->StringToBytes(bSendBuf+16, m_des_key3, 8, TRUE);	
	case CMD_DES_CBC_KEYTWO:
	case CMD_DES_ECB_KEYTWO:
		pUSBDlg->StringToBytes(bSendBuf+8, m_des_key2, 8, TRUE);
	case CMD_DES_CBC_KEYONE:
	case CMD_DES_ECB_KEYONE:
		pUSBDlg->StringToBytes(bSendBuf, m_des_key1, 8, TRUE);
		break;
	default:
		{
			AfxMessageBox("Mode not support!");
			return (-1);
		}
		break;
	}

    crypt_req.IP_code = DES;
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

	return DES_SUCCESS;
}
