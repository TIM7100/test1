
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CMDDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CMDDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CMDDLL_EXPORTS
#define CMDDLL_API __declspec(dllexport)
#else
#define CMDDLL_API __declspec(dllimport)
#endif

typedef          void         *HDISK;
// This class is exported from the CmdDll.dll
class CMDDLL_API CCmdDll {
public:
	CCmdDll(void);
	// TODO: add your methods here.
};



void __stdcall InitUSBDisks(HDISK  pDisk[], int  nInLength, int *nOutLength);
HDISK __stdcall InitUSBDiskByName(char DiskName);
void __stdcall CloseUSBDisk(HDISK hDisk);

//按扇区逻辑读
BOOL __stdcall ReadDisk( 
								 HDISK   IN   hDriver,                 //驱动器句柄
								 DWORD   IN   dwStartSectorAdd,         //起始扇区地址，从0 开始
								 DWORD   IN   dwSectorNum,              //读取的扇区个数；
								 PVOID   OUT  pOutBuff,               //接收数据的缓冲区								 										  
								 BYTE    IN   btDisk = 0
								 );

//按扇区逻辑写
BOOL __stdcall WriteDisk( 
								  HDISK   IN   hDriver,                //驱动器句柄
								  DWORD   IN   dwStartSectorAdd,         //起始扇区地址，从0 开始
								  DWORD   IN   dwSectorNum,              //写入的扇区个数；
								  PVOID   IN   pInBuff,                //写入数据的缓冲区								 										  
								  BYTE    IN   btDisk = 0
								  );

//擦除flash
BOOL __stdcall EraseFlash(
				HDISK    hDriver,  		  //驱动器句柄	
				DWORD    dwAddr,          //地址
				BOOL     bBlock,          //block或page
				BOOL     bBind = FALSE,           //绑定地址或物理地址
				BOOL     bEnableChip = FALSE,     //是否使能chip地址
				BYTE     btChipNo = 0         //chip号
				);

BOOL __stdcall EraseFlash_chk(
				HDISK    hDriver,  			
				DWORD    dwAddr,
				PBYTE     Status,
				BOOL     bBlock,
				BOOL     bBind = FALSE,
				BOOL     bEnableChip = FALSE,
				BYTE     btChipNo = 0				
				);
//擦除EFLASH的一页
BOOL __stdcall EraseEFlashPage(HDISK hDriver,DWORD PageAddr);

//
BOOL __stdcall WriteEFlashBytes(HDISK hDriver,DWORD Addr,WORD WriteLength, PBYTE pOutBuff);

//物理读
BOOL __stdcall ReadDiskPhy ( 
							  HDISK  hDriver,                  //驱动器句柄
							  DWORD   dwAddr,                  //起始地址，从0 开始
							  DWORD   dwLength,                 //读的数据的长度；
							  PVOID   pOutBuff,                //读数据的缓冲区
							  WORD    wOffsetInPage = 0,       //页内偏移量
							  BOOL    bPageAddr = FALSE,       //是否页地址
							  BOOL    bByteMode = FALSE, 	   //是否按BYTE读						          
							  BOOL    bBind = FALSE,           //绑定地址或物理地址
							  BOOL    bEnableChip = FALSE,     //是否使能chip地址
							  BYTE    btChipNo = 0 			   //chip号
							  );

//物理写
BOOL __stdcall WriteDiskPhy ( 
							   HDISK  hDriver,                  //驱动器句柄
							   DWORD   dwAddr,                  //起始地址，从0 开始
							   DWORD   dwLength,                //写的数据的长度；
							   PVOID   pInBuff,                 //写数据的缓冲区
							   WORD    wOffsetInPage = 0,       //页内偏移量
							   BOOL    bPageAddr = FALSE,       //是否页地址
							   BOOL    bByteMode = FALSE,	    //是否按BYTE读		          
							   BOOL    bBind = FALSE,           //绑定地址或物理地址
							   BOOL    bEnableChip = FALSE,     //是否使能chip地址
							   BYTE    btChipNo = 0 		    //chip号
							   );

//按块物理读
BOOL __stdcall ReadBlock( 
						 HDISK   IN   hDriver,                 //驱动器句柄
						 DWORD   IN   dwBlockAdd,         //起始block地址，从0 开始
						 DWORD   IN   BlockNum,              //读取的block个数；
						 PVOID   OUT  pOutBuff,               //接收数据的缓冲区
						 BYTE    IN   ChipNo = 0,            //物理读写时的FLASH 片数； 为0XFF则表示逻辑读写
						 BOOL    IN   bBind = FALSE             //是否绑定
						 );
//按块物理写
BOOL __stdcall WriteBlock( 
						  HDISK   IN   hDriver,                 //驱动器句柄
						  DWORD   IN   StartBlockAdd,         //起始block地址，从0 开始
						  DWORD   IN   BlockNum,              //写入的block个数；
						  PVOID   IN   pInBuff,               //写入数据的缓冲区
						  BYTE    IN   ChipNo = 0,            //物理读写时的FLASH 片数； 为0XFF则表示逻辑读写
						  BOOL    IN   bBind = FALSE);        //是否绑定

BYTE __stdcall  SetBit(BYTE Data,    //源数据
					   int  nBit,    //指定的位数，从0开始
					   BOOL   bFalg   //参数为TRUE，设置指定位的值位1，反之则设置指定位的值为0
							);


//返回指定BIT的值，如果指定位的值位1则返回TRUE;
BOOL  __stdcall GetBit(BYTE Data, int nBit);

DWORD  __stdcall  ReverseDWORD(DWORD InData);
WORD  __stdcall ReverseWORD(WORD InData);
void  __stdcall ByteToChar(PBYTE pInBuff, DWORD InLength, PCHAR   pStr);
void __stdcall CharToByte(PCHAR  pInBuff, PBYTE pOutBuff, PDWORD pRetLen);
char __stdcall  UpChar(char  ch);
BOOL __stdcall TestReady(HDISK  hDriver );

BOOL  __stdcall ComputeBit(
							LPVOID   pBuff1,     ///数据1
							LPVOID  pBuff2,     ////数据2
							const int BuffLen,   ////数据长度，字节数
							const int AllowBit,   ///
							PDWORD pdwErrorByte=NULL   //出现错误的BYTE
							);
//获取容量
BOOL __stdcall GetDiskTotalSize(HDISK hDisk, DWORD *pdwSize);

//校验
BYTE __stdcall  CheckSum(PBYTE pBuffer, int shLength);

//获取flash ID
BOOL __stdcall GetFlashID(HDISK hDisk, BYTE btChannel, PBYTE pBuff);

//获取固件版本
BOOL __stdcall GetFirmwareVer(
							  HDISK  IN   hDriver,                 //驱动器句柄
							  PVOID  OUT  pOutBuff, 
							  DWORD   IN   ReadLength
							  );

//获取盘顺序
BOOL __stdcall GetDiskLun(HDISK hDisk,PBYTE pResult);

//建对照表
BOOL  __stdcall BuildLPTabl(HDISK hDisk);

//设置flash 信息
BOOL __stdcall SetFlashInfo(HDISK IN hDriver,PBYTE pInBuff);

//获取盘容量
BOOL __stdcall GetLogicCapacity (
					   HDISK  hDriver,                 //驱动器句柄
					   PDWORD   pSectors,             //总的扇区数；					   
					   BYTE btDisk = 0);

//读CI表
BOOL __stdcall ReadCI(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  OUT   pOutBuff,                //传入数据的指针
						WORD   IN  ReadLength              //传入的长度, 以sector为单位						          
						);
//写CI表
BOOL __stdcall WriteCI(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pInBuff,                //传入数据的指针
						WORD   IN   WriteLength              //传入的长度, 以sector为单位
						
						);

//写好坏块信息表
BOOL __stdcall WriteGBListInfo(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pInBuff,                //传入数据的指针
						WORD   IN   WriteLength,              //传入的长度, 以sector为单位
						DWORD  IN   Offset               //CI表的偏移地址, 以sector为单位
						);

//读好坏块信息表
BOOL __stdcall ReadGBListInfo(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pOutBuff,                //传入数据的指针
						WORD   IN   ReadLength,              //传入的长度, 以sector为单位
						DWORD  IN   Offset               //CI表的偏移地址, 以sector为单位
						);

BOOL __stdcall FwScanFlashRead(HDISK  IN   hDriver,                 //驱动器句柄
				 DWORD  IN   dwAddress,                //传入数据的指针
				 WORD   IN   wLength,
				 BYTE   IN   btScanLevel,
				 BYTE    *pResult
				 );


//获取盘类型
BOOL  __stdcall GetDiskType(HDISK hDisk, PBYTE pType);

//获取盘信息表
BOOL  __stdcall GetDiskInfoTable(HDISK hDisk, PBYTE pBuff);

//写取盘信息表
BOOL  __stdcall WriteDiskInfoTable(HDISK hDisk, PBYTE pBuff);

//获取物理句柄
void __stdcall GetPhysicalHandel(HDISK  phDisk[], int InLength, int &OutLength);

//关闭物理句柄
void __stdcall ClosePhyDeviceHandle(HDISK  hDriver);

//写用户信息
BOOL __stdcall WriteUserInfo(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pInBuff,                //传入数据的指针
						WORD   IN   WriteLength              //传入的长度, 以sector为单位						
						);

//读用户信息
BOOL  __stdcall GetUserInfo(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  OUT   pOutBuff,                //传入数据的指针
						WORD   IN   ReadLength              //传入的长度, 以sector为单位						
						);
//验证密码
BOOL  __stdcall VerifyPassword(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pBuff,                //传入数据的指针
						WORD   IN   wLength              //传入的长度, 以sector为单位						
						);
//验证密码并进入加密区
BOOL  __stdcall CheckInSD(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pBuff,                //传入数据的指针
						WORD   IN   wLength,              //传入的长度, 以sector为单位
						BOOL   IN   bMP = FALSE
						);

BOOL __stdcall LogoutSD(HDISK  hDriver);

//使能隐藏区
BOOL __stdcall EnableHiddenDisk(HDISK  hDriver);


BOOL  __stdcall GetPublicKey(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  OUT   pOutBuff,                //传入数据的指针
						WORD   IN   ReadLength              //传入的长度, 以sector为单位						
						);

//设置密码
BOOL  __stdcall SetPassword(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  IN   pBuff,                //传入数据的指针
						WORD   IN   wLength              //传入的长度, 以sector为单位						
						);

//验证管理员密码
BOOL  __stdcall VerifyAdminPassword(
									HDISK  IN   hDriver,                 //驱动器句柄
									PVOID  IN   pBuff,                //传入数据的指针
									WORD   IN   wLength              //传入的长度, 以sector为单位						
							   );

//设置管理员密码
BOOL  __stdcall SetAdminPassword(
								 HDISK  IN   hDriver,                 //驱动器句柄
								 PVOID  IN   pBuff,                //传入数据的指针
								 WORD   IN   wLength              //传入的长度, 以sector为单位						
							);

//解除锁定
BOOL __stdcall UnlockDisk(HDISK  hDriver);

//读隐藏区
BOOL __stdcall ReadHiddenDisk( 
							  HDISK  hDriver,                 //驱动器句柄
							  DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
							  DWORD   dwSectorNum,              //读取的扇区个数；
							  PVOID   pOutBuff               //接收数据的缓冲区
						);
//写隐藏区
BOOL __stdcall WriteHiddenDisk( 
							   HDISK  hDriver,                 //驱动器句柄
							   DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
							   DWORD   dwSectorNum,              //写入的扇区个数；
							   PVOID   pInBuff               //写入数据的缓冲区								   		   
						 );

//读安全区
BOOL __stdcall ReadSecurityDisk( 
						HDISK  hDriver,                 //驱动器句柄
						DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
						DWORD   dwSectorNum,              //读取的扇区个数；
						PVOID   pOutBuff               //接收数据的缓冲区
						);
//写安全区
BOOL __stdcall WriteSecurityDisk( 
						 HDISK  hDriver,                 //驱动器句柄
						 DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
						 DWORD   dwSectorNum,              //写入的扇区个数；
						 PVOID   pInBuff               //写入数据的缓冲区						 		   
						 );
//格式化时写盘
BOOL __stdcall FormatWriteDisk( 
			   HDISK  hDriver,                 //驱动器句柄
			   DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
			   DWORD   dwSectorNum,              //写入的扇区个数；
			   PVOID   pInBuff,               //写入数据的缓冲区			   
			   BYTE    btDisk			   
			   );
//格式化时读盘
BOOL __stdcall FormatReadDisk( 
			  HDISK  hDriver,                 //驱动器句柄
			  DWORD   dwStartSectorAdd,         //起始扇区地址，从0 开始
			  DWORD   dwSectorNum,              //读取的扇区个数；
			  PVOID   pOutBuff,               //接收数据的缓冲区			  
			  BYTE    btDisk			  
			  );

//获取数据key
BOOL  __stdcall GetDataKey(
						HDISK  IN   hDriver,                 //驱动器句柄
						PVOID  OUT   pOutBuff,                //传入数据的指针
						WORD   IN   ReadLength              //传入的长度, 以sector为单位						
						);

BOOL __stdcall EraseEFlashPhy(HDISK hDriver);

BOOL  __stdcall GetSDState(HDISK hDisk,PBYTE pResult);

long  GetErrorType(HDISK   hDriver);