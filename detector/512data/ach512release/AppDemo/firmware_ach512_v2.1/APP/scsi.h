/***********************************************************************
 * Copyright (c)  2008 - 2016, Shanghai AisinoChip Co.,Ltd .
 * All rights reserved.
 * Filename    : scsi.h
 * Description : scsi application header file
 * Author(s)   : Eric  
 * version     : V1.0
 * Modify date : 2016-03-24
 ***********************************************************************/
#ifndef __SCSI_H__
#define __SCSI_H__

#include  "common.h"

#define NO_SENSE                       0
#define INVALID_FIELD_IN_COMMAND       1
#define NEW_MEDIUM_ARRIVEL             2
#define WRITE_PROTECTED                3 
#define PASSWORD_ERROR                 4 
#define NOT_CONFIG                     5 
#define USER_LOCKED                    6 
#define MEDIUM_NOT_PRESENT             7 
#define LOGDRIVER_NOT_READY            8 
#define ADDRESS_OUT_RANGE              9 
#define WRITEFAULT                     10
#define CBW_PHASE_ERROR                11
#define UNRECOVERED_READ_ERROR         12
#define MEDIAREMOVALPREVENTED          13
#define INVALID_COMMAND_OPERATION_CODE 14
#define HARDWARE_ERROR                 15
#define APPCOS_CRYPT_ERROR             16
#define SenseCodeCnt                   17

#define	CBW_DIR_IN				0x80

#define	MSC_GOOD_STATUS			0
#define	MSC_CMD_FAILED			1
#define	MSC_PHASE_ERROR			2

typedef struct _MSCCMD {
	UINT32		Signature;
	UINT32		CbwTag;
	UINT32		CbwTrxLength;
	UINT8		CbwFlag;
	UINT8		CbwLun;
	UINT8		CbwCbLen;
	
	UINT8		OpCode;
	UINT8		SubOpCode;
	UINT32		Address;
	UINT8		Chip;
	UINT16		Length;
	UINT8		A8Length;
	UINT32		Residue;
	UINT8		Reserved[2];
} MSCCMD;

typedef	struct	_MSCCSW {
	UINT32		Signature;
	UINT32		CswTag;
	UINT32		Residue;
	UINT8		Status;
} MSCCSW;

typedef  struct _SENSE_CODE {
	UINT8	SenseKey;
	UINT8	ASC;
	UINT8	ASCQ;
} SENSE_CODE;

UINT8 scsi_proc(UINT8 ep_index);

#endif
