// AESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "AESDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means mode.
#define CMD_AES_ENC                0x10
#define CMD_AES_DEC                0x20
#define CMD_AES_OPMODE_MASK        0xF0

#define CMD_AES_ECB_KEY128         0x01
#define CMD_AES_ECB_KEY192         0x02
#define CMD_AES_ECB_KEY256         0x03
#define CMD_AES_CBC_KEY128         0x04
#define CMD_AES_CBC_KEY192         0x05
#define CMD_AES_CBC_KEY256         0x06
#define CMD_AES_MODE_MASK          0x0F

#define AES_SUCCESS                0x00
#define AES_MAX_CRYPT_LEN          4096
#define AES_MAX_KEY_LEN            64

/////////////////////////////////////////////////////////////////////////////
// CAESDlg dialog


CAESDlg::CAESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAESDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAESDlg)
	m_aes_key = _T("E04AD1A465FE706B80CF29605DA2636A");
	m_aes_iv = _T("00000000000000000000000000000000");
	m_aes_plain = _T("C42821E13AC9B62272304AD5E5F5937D");
	m_aes_cipher = _T("1F4F49E4FB96CCA5A309032FB1A46DDE");
	m_aes_output = _T("");
	m_aes_mode = -1;
	m_aes_keymode = -1;
	//}}AFX_DATA_INIT
}


void CAESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAESDlg)
	DDX_Text(pDX, IDC_AES_EDIT_CIPHER, m_aes_cipher);
	DDX_Text(pDX, IDC_AES_EDIT_IV, m_aes_iv);
	DDX_Text(pDX, IDC_AES_EDIT_KEY, m_aes_key);
	DDX_Text(pDX, IDC_AES_EDIT_OUTPUT, m_aes_output);
	DDX_Text(pDX, IDC_AES_EDIT_PLAIN, m_aes_plain);
	DDX_Radio(pDX, IDC_AES_RADIO_ECB, m_aes_mode);
	DDX_Radio(pDX, IDC_AES_RADIO_KEY128, m_aes_keymode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAESDlg, CDialog)
	//{{AFX_MSG_MAP(CAESDlg)
	ON_BN_CLICKED(IDC_AES_BUTTON_ENC, OnAESButtonEnc)
	ON_BN_CLICKED(IDC_AES_BUTTON_DEC, OnAESButtonDec)
	ON_BN_CLICKED(IDC_AES_BUTTON_ENCFILE, OnAESButtonEncfile)
	ON_BN_CLICKED(IDC_AES_BUTTON_DECFILE, OnAESButtonDecfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAESDlg message handlers

void CAESDlg::OnAESButtonEnc() 
{
	// TODO: Add your control notification handler code here
	BYTE  bPlaintext[AES_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_aes_plain.GetLength() % 32)
	{
		AfxMessageBox("Please Input Plain in complete Blocks!");
		return;
	}
	else
	{
		dPlainLen = m_aes_plain.GetLength() / 2;
		pUSBDlg->StringToBytes(bPlaintext, m_aes_plain, dPlainLen, TRUE);
	}

	if(AES_SUCCESS != aes_crypt(pUSBDlg, CMD_AES_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
	{
		AfxMessageBox("AES Encrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_aes_output, bRecvBuf, dCipherLen);

	UpdateData(FALSE);
}

void CAESDlg::OnAESButtonDec() 
{
	// TODO: Add your control notification handler code here
	BYTE  bCiphertext[AES_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_aes_cipher.GetLength() % 32)
	{
		AfxMessageBox("Please Input Cipher in complete Blocks!");
		return;
	}
	else
	{
		dCipherLen = m_aes_cipher.GetLength() / 2;
		pUSBDlg->StringToBytes(bCiphertext, m_aes_cipher, dCipherLen, TRUE);
	}

	if(AES_SUCCESS != aes_crypt(pUSBDlg, CMD_AES_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
	{
		AfxMessageBox("AES Decrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_aes_output, bRecvBuf, dPlainLen);

	UpdateData(FALSE);
}

void CAESDlg::OnAESButtonEncfile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bPlaintext[AES_MAX_CRYPT_LEN];
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
		if(!(dPlainLen = m_inputfile.Read(bPlaintext, AES_MAX_CRYPT_LEN)))
			break;		
		if(dPlainLen < AES_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dPlainLen % 16);
			if(dPadLength != 16)
			{
				memset(bPlaintext+dPlainLen, ' ', dPadLength);
				dPlainLen += dPadLength;
			}
		}

		// Encrypt
		if(AES_SUCCESS != aes_crypt(pUSBDlg, CMD_AES_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
		{
			AfxMessageBox("AES Encrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dCipherLen);

		if(dCipherLen < AES_MAX_CRYPT_LEN)
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

void CAESDlg::OnAESButtonDecfile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bCiphertext[AES_MAX_CRYPT_LEN];
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
		if(!(dCipherLen = m_inputfile.Read(bCiphertext, AES_MAX_CRYPT_LEN)))
			break;		
		if(dCipherLen < AES_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dCipherLen % 16);
			if(dPadLength != 16)
			{
				memset(bCiphertext+dCipherLen, ' ', dPadLength);
				dCipherLen += dPadLength;
			}
		}

		// Decrypt
		if(AES_SUCCESS != aes_crypt(pUSBDlg, CMD_AES_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
		{
			AfxMessageBox("AES Decrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dPlainLen);

		if(dPlainLen < AES_MAX_CRYPT_LEN)
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

int CAESDlg::aes_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut)
{
	CRYPT_REQUEST crypt_req;
	USHORT  key_len;
	UINT    OutputLength;
	BYTE    bSendBuf[AES_MAX_CRYPT_LEN+AES_MAX_KEY_LEN];

	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	// Verify key setting under rules or not
	if(m_aes_keymode == -1)
	{
		AfxMessageBox("Please Select Key Mode!");
		return (-1);
	}
	if(m_aes_mode == -1)
	{
		AfxMessageBox("Please Select Crypt Mode!");
		return (-1);
	}
	if (m_aes_key.GetLength() != (16 * (m_aes_keymode + 2)))
	{
		AfxMessageBox("Please Input 128/192/256bits KEY, according to KEYNUM setting!");
		return (-1);
	}
	
	// Set mode, Get and fulfill key to send buffer
	key_len = 8 * (m_aes_keymode + 2);
	pUSBDlg->StringToBytes(bSendBuf, m_aes_key, key_len, TRUE);
	if(m_aes_mode == 1)
	{
		crypt_req.cmd_code = (CMD_AES_CBC_KEY128 + m_aes_keymode);
		if(m_aes_iv.GetLength() != (16 * (m_aes_keymode + 2)))
		{
			AfxMessageBox("Please Input 128/192/256bits IV, according to KEYNUM setting!");
			return (-1);
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+key_len, m_aes_iv, key_len, TRUE);
			key_len *= 2;
		}
	}
	else
	{
		crypt_req.cmd_code = (CMD_AES_ECB_KEY128 + m_aes_keymode);
	}

    crypt_req.IP_code = AES;
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

	return AES_SUCCESS;
}
