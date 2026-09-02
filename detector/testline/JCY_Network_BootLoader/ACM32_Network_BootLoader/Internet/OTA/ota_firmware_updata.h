#ifndef  _OTA_FIRM_UPDATA_H_
#define  _OTA_FIRM_UPDATA_H_

#include "fxx_std.h"

//#define DEVICE_NAME                  "ACMTest"
#define DEVICE_NAME_SIZE             12
#define OTA_SERVER_PORT	             443			             //腾讯云连接HTTPS服务器所使用的端口号：HTTP:80  HTTPS:443
#define LOCAL_VERSION_SECTOR         (0x0000 / 512)              //本地固件包信息在SD卡的存放起始扇区
#define LOCAL_INFO_COUNT             (1024 / 512)			     //本机记录的信息占SD卡的扇区数量
#define	FIRMWARE_VERSION_SIZE        13                          //固件包的版本名大小
#define SERIES_NAME_SIZE		     8                           //系列名的大小
#define SERIES_VERSION_SIZE          4                           //系列版本的大小
#define NEW_SERIES_VERSION           "01.0"						 //新添加的系列的默认版本为01.0	
#define WRITE_SD_SIZE                2048U			             //单次写入SD卡的大小, 改变写入大小前，需要前往user_cong.c改变W5500的接收缓存区的大小，不然无法接收全部的数据
#define DOWNLOAD_SIZE_ONCE           (WRITE_SD_SIZE + 2U)	     //单次下载的字节数量 WRITE_SD_SIZE + 2位CRC
#define WRITE_SD_START_ADDR          (SD_APP_END_ADDR * 2)       //固件包的写入位置, 缓存区
#define CRC_ERROR_COUNT              5							 //CRC出错后的判断次数
#define FIRST_BUFF_SERIES_COUNT      39							 //第一片存储区域内能存放的系列数
/***********************************************************************************************
 *                                   OTA信息的命令参数                                          *
************************************************************************************************/
#define SERIES_UPDATE        "UPDATE_SERIES"				//表示该OTA信息用于更新系列
#define SERIES_ADD		     "ADD_SERIES"					//表示该OTA信息用于添加设备支持的系列
#define DELETE_UPDATE		 "DELETE_UPDATE"				//表示该OTA信息用于删除某些系列，并且对删除后支持的系列进行更新
/***********************************************************************************************
 *                                  数据上报云平台的模板                                         *
************************************************************************************************/
#define THING_UPLOAD_TEMPLATE             "{\"method\":\"report\",\"clientToken\":\"123\",\"timestamp\":1628646783,\"params\":{\"FV\":\"%s\",\"DV\":\"%s\",\"DS\":\"%s\"}}"
#define OTA_UPLOAD_VERSION_TEMPLATE	      "{\"type\": \"report_version\",\"report\":{\"version\": \"%s\"}}"			//上报版本时，报文数据部分的格式
#define OTA_UPLOAD_RESULT_TEMPLATE        "{\"type\": \"report_progress\",\"report\":{\"progress\":{\"state\":\"%s\",\"result_code\":\"%s\",\"result_msg\":\"\"},\"version\": \"%s\"}}"

//HTTP的请求报文. GET：后面跟随URL标识的资源; HTTP/1.1:为HTTP的版本; HOST:请求的目的主机域名或IP; Range:请求下载的起始地址和结束地址.
#define HTTP_UPLOAD_TEMPLATE              "GET %s HTTP/1.1\r\nHOST: %s\r\nRange: bytes=%d-%d\r\n\r\n"	
#define HTTP_ACK_PACKET	                  "HTTP/1.1 206 Partial Content\r\n"					       //206表示HTTP请求成功

/***********************************************************************************************
 *                                   固件更新状态上报参数                                        *
************************************************************************************************/
#define STATE_DOWNLOAD_START			  "burning"	
#define STATE_UPDATA_SUCCESS			  "done"	
#define STATE_ERROR			  		      "fail"		
#define RESULT_CODE_SUCCESS				   "0"
#define RESULT_CODE_ERROR				   "1"


/***********************************************************************************************
 *                                OTA下发的固件包信息的结构体                                    *
************************************************************************************************/
#pragma pack(1)
typedef struct OAT_INFO {
    u32 FileSize;								  //文件大小
    char Md5Sum[64];						      //服务器下发的md5
    char Host[64];								  //OTA下载固件包的URL的服务器域名
	char URLABuff[512];                           //固件包的URL的资源地址
	char Command[16];                             //自定义命令
	char CommandBuff[1024];						  //
	u16 CmdBuffLen;
    char Version[FIRMWARE_VERSION_SIZE];		  //OTA版本信息
} OTA_INFO_t;
#pragma pack()


/***********************************************************************************************
 *                                  HTTP下载时的记录结构体                                      *
************************************************************************************************/
typedef struct HTTP_PACKET{
//	char DataBuff[DOWNLOAD_SIZE_ONCE + 512]; //数据缓存区
	u32 DownloadCount;			           //总共需要下载的次数
	u32 DownloadNum;			           //已经下载的次数
}HTTP_PACKET_t;



/***********************************************************************************************
 *                                    函数返回值的枚举集合                                       *
************************************************************************************************/
enum {
	OTA_SUCCESS = 0, 
	OTA_CONNECT_SERVER_ERR = 1, 				//连接服务器失败标志
	OTA_SSL_ERR = 2,			                //SLL握手失败
	OTA_CONNECT_MQTT_ERR = 3,                   //连接MQTT服务器失败
	OTA_COMMAND_ERR = 4,
	OTA_LOSE_INFO = 5,                              // 
	OTA_HTTP_ERROR,
	OTA_CRC_ERROR,	
	OTA_WRITE_ERROR,
	OTA_FIRM_CRC_ERR,
	OTA_LINK_ERR,
	OTA_NEW_VERSION,
	OTA_DONT_UPDATA,
	OTA_COMMAND_ADD,
	OTA_COMMAND_UPDATA,
	OTA_COMMAND_ANALYSIS,
	OTA_ADD_SERIES_ZERO,
	OTA_HTTP_SUCCESS,
	OTA_CONNECT_TIMEOUT,
	OTA_COMMAND_DELETE_UPDATE
};

u8 FactoryReset(void);
u8 ConnectTencentCloud(void);
u8 AnalyseOTADownloadInfo(void);
u8 HandleFirmwareProcess(void);
u8 HandleDownloadFirmwareCommand(u8 Command, u8 *AddSeriesCount);
u8 HandleDownloadFirmwareState(u8 State);
void DisconnectTencentCloud(void);


#endif
