// SM4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "SM4Dlg.h"
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
#define CMD_SM4_ENC                0x10
#define CMD_SM4_DEC                0x20
#define CMD_SM4_OPMODE_MASK        0xF0

#define CMD_SM4_ECB                0x01
#define CMD_SM4_CBC                0x02
#define CMD_SM4_MODE_MASK          0x0F

#define SM4_SUCCESS                0x00
#define SM4_MAX_CRYPT_LEN          4096
#define SM4_MAX_KEY_LEN            32

/////////////////////////////////////////////////////////////////////////////
// CSM4Dlg dialog


CSM4Dlg::CSM4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSM4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSM4Dlg)
	m_sm4_key = _T("762A64E51CAE4995658B0B5347EE7778");
	m_sm4_iv = _T("00000000000000000000000000000000");
	m_sm4_plain = _T("2B3B317B40115E7D5A1D024219B940D3");
	m_sm4_cipher = _T("6414E511C7336970873A45905A4B2112");
	m_sm4_output = _T("");
	m_sm4_mode = -1;
	//}}AFX_DATA_INIT
}


void CSM4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSM4Dlg)
	DDX_Text(pDX, IDC_SM4_EDIT_KEY, m_sm4_key);
	DDX_Text(pDX, IDC_SM4_EDIT_IV, m_sm4_iv);
	DDX_Text(pDX, IDC_SM4_EDIT_PLAIN, m_sm4_plain);
	DDX_Text(pDX, IDC_SM4_EDIT_CIPHER, m_sm4_cipher);
	DDX_Text(pDX, IDC_SM4_EDIT_OUTPUT, m_sm4_output);
	DDX_Radio(pDX, IDC_SM4_RADIO_ECB, m_sm4_mode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSM4Dlg, CDialog)
	//{{AFX_MSG_MAP(CSM4Dlg)
	ON_BN_CLICKED(IDC_SM4_BUTTON_ENC, OnSM4ButtonEnc)
	ON_BN_CLICKED(IDC_SM4_BUTTON_DEC, OnSM4ButtonDec)
	ON_BN_CLICKED(IDC_SM4_BUTTON_ENCFILE, OnSM4ButtonEncFile)
	ON_BN_CLICKED(IDC_SM4_BUTTON_DECFILE, OnSM4ButtonDecFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSM4Dlg message handlers

void CSM4Dlg::OnSM4ButtonEnc() 
{
	// TODO: Add your control notification handler code here
	BYTE  bPlaintext[SM4_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dPlainLen;
	DWORD dCipherLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bPlaintext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_sm4_plain.GetLength() % 32)
	{
		AfxMessageBox("Please Input Plain in complete Blocks!");
		return;
	}
	else
	{
		dPlainLen = m_sm4_plain.GetLength() / 2;
		pUSBDlg->StringToBytes(bPlaintext, m_sm4_plain, dPlainLen, TRUE);
	}

	if(SM4_SUCCESS != sm4_crypt(pUSBDlg, CMD_SM4_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
	{
		AfxMessageBox("SM4 Encrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_sm4_output, bRecvBuf, dCipherLen);

	UpdateData(FALSE);
}

void CSM4Dlg::OnSM4ButtonDec() 
{
	// TODO: Add your control notification handler code here
	BYTE  bCiphertext[SM4_MAX_CRYPT_LEN];
	BYTE  *bRecvBuf;
	DWORD dCipherLen;
	DWORD dPlainLen;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	bRecvBuf = bCiphertext;
	
	UpdateData(TRUE);

	// Get plaintext
	if(m_sm4_cipher.GetLength() % 32)
	{
		AfxMessageBox("Please Input Cipher in complete Blocks!");
		return;
	}
	else
	{
		dCipherLen = m_sm4_cipher.GetLength() / 2;
		pUSBDlg->StringToBytes(bCiphertext, m_sm4_cipher, dCipherLen, TRUE);
	}

	if(SM4_SUCCESS != sm4_crypt(pUSBDlg, CMD_SM4_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
	{
		AfxMessageBox("SM4 Decrypt error!");
		return;
	}

	pUSBDlg->ShowReceivedData(m_sm4_output, bRecvBuf, dPlainLen);

	UpdateData(FALSE);
}

void CSM4Dlg::OnSM4ButtonEncFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bPlaintext[SM4_MAX_CRYPT_LEN];
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
		if(!(dPlainLen = m_inputfile.Read(bPlaintext, SM4_MAX_CRYPT_LEN)))
			break;		
		if(dPlainLen < SM4_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dPlainLen % 16);
			if(dPadLength != 16)
			{
				memset(bPlaintext+dPlainLen, ' ', dPadLength);
				dPlainLen += dPadLength;
			}
		}

		// Encrypt
		if(SM4_SUCCESS != sm4_crypt(pUSBDlg, CMD_SM4_ENC, bPlaintext, dPlainLen, bRecvBuf, &dCipherLen))
		{
			AfxMessageBox("SM4 Encrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dCipherLen);

		if(dCipherLen < SM4_MAX_CRYPT_LEN)
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

void CSM4Dlg::OnSM4ButtonDecFile() 
{
	// TODO: Add your control notification handler code here
	CFile m_inputfile;
	CFile m_outputfile;
	BYTE  bCiphertext[SM4_MAX_CRYPT_LEN];
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
		if(!(dCipherLen = m_inputfile.Read(bCiphertext, SM4_MAX_CRYPT_LEN)))
			break;		
		if(dCipherLen < SM4_MAX_CRYPT_LEN)
		{
			DWORD dPadLength = 16 - (dCipherLen % 16);
			if(dPadLength != 16)
			{
				memset(bCiphertext+dCipherLen, ' ', dPadLength);
				dCipherLen += dPadLength;
			}
		}

		// Decrypt
		if(SM4_SUCCESS != sm4_crypt(pUSBDlg, CMD_SM4_DEC, bCiphertext, dCipherLen, bRecvBuf, &dPlainLen))
		{
			AfxMessageBox("SM4 Decrypt error!");
			return;
		}

		m_outputfile.Write(bRecvBuf, dPlainLen);

		if(dPlainLen < SM4_MAX_CRYPT_LEN)
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

int CSM4Dlg::sm4_crypt(CCUSBDlg *pUSBDlg, BYTE operation, BYTE *bCryptIn, DWORD nBytesCryptIn, BYTE *bCryptOut, DWORD *nBytesCryptOut)
{
	CRYPT_REQUEST crypt_req;
	USHORT  key_len;
	UINT    OutputLength;
	BYTE    bSendBuf[SM4_MAX_CRYPT_LEN+SM4_MAX_KEY_LEN];

	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	// Verify key setting under rules or not
	if(m_sm4_mode == -1)
	{
		AfxMessageBox("Please Select Crypt Mode!");
		return (-1);
	}
	if (m_sm4_key.GetLength() != 32)
	{
		AfxMessageBox("Please Input 128bits KEY!");
		return (-1);
	}
	
	// Set mode, Get and fulfill key to send buffer
	key_len = 16;
	if(m_sm4_mode == 1)
	{
		crypt_req.cmd_code = CMD_SM4_CBC;
		if(m_sm4_iv.GetLength() != 32)
		{
			AfxMessageBox("Please Input 128bits IV!");
			return (-1);
		}
		else
		{
			pUSBDlg->StringToBytes(bSendBuf+key_len, m_sm4_iv, 16, TRUE);
			key_len += 16;
		}
	}
	else
	{
		crypt_req.cmd_code = CMD_SM4_ECB;
	}
	pUSBDlg->StringToBytes(bSendBuf, m_sm4_key, 16, TRUE);

    crypt_req.IP_code = SM4;
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

	return SM4_SUCCESS;
}
