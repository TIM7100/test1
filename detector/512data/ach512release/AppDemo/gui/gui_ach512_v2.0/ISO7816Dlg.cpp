// ISO7816Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "cusb.h"
#include "ISO7816Dlg.h"
#include "UsbDevice.h"	// Added by ClassView
#include "CUSBDlg.h"	// Added by ClassView
#include "crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//cmd_code definition
//bits[7:4] means operation, bits[3:0] means card protocol type.
#define CMD_ISO7816_RESET          0x10
#define CMD_ISO7816_PPS            0x20
#define CMD_ISO7816_APDU           0x30
#define CMD_ISO7816_OPMODE_MASK    0xF0

#define CMD_ISO7816_PROTOCOL_T0    0x01
#define CMD_ISO7816_PROTOCOL_T1    0x02
#define CMD_ISO7816_PROTOCOL_MASK  0x0F 

#define ISO7816_SUCCEED            0x00
#define ISO7816_FAILED             0xFF

#define ISO7816_PROTOCOL_T0        0
#define ISO7816_PROTOCOL_T1        1

/////////////////////////////////////////////////////////////////////////////
// CISO7816Dlg dialog


CISO7816Dlg::CISO7816Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CISO7816Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CISO7816Dlg)
	m_iso7816_apdu = _T("");
	m_iso7816_apduresp = _T("");
	m_iso7816_atr = _T("");
	m_iso7816_pps = _T("");
	m_iso7816_ppsresp = _T("");
	protocol = ISO7816_PROTOCOL_T0;
	//}}AFX_DATA_INIT
}


void CISO7816Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CISO7816Dlg)
	DDX_Text(pDX, IDC_ISO7816_EDIT_APDU, m_iso7816_apdu);
	DDX_Text(pDX, IDC_ISO7816_EDIT_APDURESPONSE, m_iso7816_apduresp);
	DDX_Text(pDX, IDC_ISO7816_EDIT_ATR, m_iso7816_atr);
	DDX_Text(pDX, IDC_ISO7816_EDIT_PPS, m_iso7816_pps);
	DDX_Text(pDX, IDC_ISO7816_EDIT_PPSRESPONSE, m_iso7816_ppsresp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CISO7816Dlg, CDialog)
	//{{AFX_MSG_MAP(CISO7816Dlg)
	ON_BN_CLICKED(IDC_ISO7816_BUTTON_RESET, OnIso7816ButtonReset)
	ON_BN_CLICKED(IDC_ISO7816_BUTTON_PPS, OnIso7816ButtonPps)
	ON_BN_CLICKED(IDC_ISO7816_BUTTON_APDU, OnIso7816ButtonApdu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CISO7816Dlg message handlers

BYTE CISO7816Dlg::CalBitNum(BYTE ch)
{
	int	i, j;
    BYTE Scan;

	j = 0;
	Scan = 0x80;	
	for(i = 8; i > 0; i--)
	{
		if(ch & Scan)
		{
			j++;
		}
		Scan = Scan >> 1;		
	}
	
	return (j);	
}

BYTE CISO7816Dlg::GetProtocolType(const BYTE *atr)
{
	BYTE TD1;
	BYTE temp;
	
	//ÅÐ¶Ï¸ñÊ½×Ö·ûT0µÄ¸ß4bit
	if(atr[1] & 0x80)
	{//´æÔÚTD1
		 temp = CalBitNum(atr[1] & 0xF0);
		 
		 //ÅÐ¶ÏTD1
		 TD1 = atr[temp+1];		 
		 if((TD1 & 0x0F) == 0x00)
		 {
			return ISO7816_PROTOCOL_T0;
		 }
		 else
		 {
			return ISO7816_PROTOCOL_T1;
		 }	
	}

	return ISO7816_PROTOCOL_T0;
}

void CISO7816Dlg::OnIso7816ButtonReset() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT OutputLength;
	BYTE atr_len = 0;
	BYTE atr[256];

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	crypt_req.IP_code = ISO7816;
	crypt_req.cmd_code = CMD_ISO7816_RESET;
	crypt_req.key_length = 0;
	crypt_req.crypt_length = 0;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, 0, NULL, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// get atr_len
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, 1, &atr_len, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	if(atr_len == 0)
	{
		AfxMessageBox("Card is not present or invalid!");
		UpdateData(TRUE);
		m_iso7816_atr.Empty();
		UpdateData(FALSE);
		return;
	}
	else
	{
		// get atr
		if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, atr_len, atr, &OutputLength))
		{
			CString error;
			error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
			AfxMessageBox(error);
			return;
		}
	}

	protocol = GetProtocolType((const BYTE*)atr);
	if(protocol == ISO7816_PROTOCOL_T0)
		AfxMessageBox("Card protocol T0");
	else
		AfxMessageBox("Card protocol T1");

	UpdateData(TRUE);
	pUSBDlg->ShowReceivedData(m_iso7816_atr, atr, atr_len);
	UpdateData(FALSE);
}

void CISO7816Dlg::OnIso7816ButtonPps() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT OutputLength;
	BYTE read_len;
	BYTE pps[256];
	BYTE pps_resp[256];

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;

	UpdateData(TRUE);
	
	if((m_iso7816_pps.GetLength() <= 0) || (m_iso7816_pps.GetLength() & 0x01))
	{
		AfxMessageBox("pps length error!");
		return;
	}
	pUSBDlg->StringToBytes(pps, m_iso7816_pps, m_iso7816_pps.GetLength()/2, TRUE);
	
	crypt_req.IP_code = ISO7816;
	crypt_req.cmd_code = CMD_ISO7816_PPS;
	crypt_req.cmd_code |= ((protocol == ISO7816_PROTOCOL_T0) ? CMD_ISO7816_PROTOCOL_T0: CMD_ISO7816_PROTOCOL_T1);
	crypt_req.key_length = m_iso7816_pps.GetLength() / 2;
	crypt_req.crypt_length = 0;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, crypt_req.key_length, pps, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// get response
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, 1, &read_len, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, read_len, pps_resp, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}
	
	if(0 != memcmp(pps, pps_resp, read_len))
	{
		AfxMessageBox("PPS error!");
		return;
	}
	else
	{
		AfxMessageBox("PPS successfully!");
	}

	pUSBDlg->ShowReceivedData(m_iso7816_ppsresp, pps_resp, crypt_req.key_length);
	
	UpdateData(FALSE);
}

void CISO7816Dlg::OnIso7816ButtonApdu() 
{
	// TODO: Add your control notification handler code here
	CRYPT_REQUEST crypt_req;
	UINT OutputLength;
	BYTE read_len;
	BYTE apdu[256];
	BYTE *apdu_resp;

	CCUSBDlg *pUSBDlg = (CCUSBDlg *) (GetSafeOwner());
	CUsbDevice *usbdevice = &pUSBDlg->mydevice;
	apdu_resp = apdu;

	UpdateData(TRUE);

	if((m_iso7816_apdu.GetLength() <= 0) || (m_iso7816_apdu.GetLength() & 0x01))
	{
		AfxMessageBox("apdu length error!");
		return;
	}
	pUSBDlg->StringToBytes(apdu, m_iso7816_apdu, m_iso7816_apdu.GetLength()/2, TRUE);
	
	crypt_req.IP_code = ISO7816;
	crypt_req.cmd_code = CMD_ISO7816_APDU;
	crypt_req.cmd_code |= ((protocol == ISO7816_PROTOCOL_T0) ? CMD_ISO7816_PROTOCOL_T0: CMD_ISO7816_PROTOCOL_T1);
	crypt_req.key_length = m_iso7816_apdu.GetLength() / 2;
	crypt_req.crypt_length = 0;

	// write req
	if(ERROR_SUCCESS != usbdevice->WriteDevice(USB_OPCODE_CRYPT, &crypt_req, crypt_req.key_length, apdu, &OutputLength))
	{
		CString error;
		error.Format("Ð´´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	// get response
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, 1, &read_len, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}
	if(ERROR_SUCCESS != usbdevice->ReadDevice(USB_OPCODE_CRYPT, &crypt_req, read_len, apdu_resp, &OutputLength))
	{
		CString error;
		error.Format("¶Á´íÎó£¬´íÎóºÅ(%d)£¬Çëµ÷ÊÔ!", GetLastError());
		AfxMessageBox(error);
		return;
	}

	pUSBDlg->ShowReceivedData(m_iso7816_apduresp, apdu_resp, read_len);
	
	UpdateData(FALSE);
}
