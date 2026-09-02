#ifndef __SD_H
#define __SD_H

#include "sys.h"
#include "menu.h"
extern volatile u8 Receive_SN[50][0x10F];

typedef struct
{
    u8 TypeCode[2];
    u8 FlashStartAddr[4];
    u8 SN_total[2];
    u8 FlashEndAddr[4];
} TypeInfrom;		//12byte


typedef struct
{
    TypeInfrom SN_H0E320KA;
    TypeInfrom SN_H0E321CA;
    TypeInfrom SN_H0E322YA;
    TypeInfrom SN_H0E323MA;
    TypeInfrom SN_H0F320KA;
    TypeInfrom SN_H0F320KX;
    TypeInfrom SN_H0F321CA;
    TypeInfrom SN_H0F322YA;
    TypeInfrom SN_H0F323MA;
    TypeInfrom SN_H0031CA;
    TypeInfrom SN_H0032YA;
    TypeInfrom SN_H0033MA;
    TypeInfrom SN_H0210KA;
    TypeInfrom SN_H0210KX;
    TypeInfrom SN_H0210KXL;
    TypeInfrom SN_H0211CA;
    TypeInfrom SN_H0211CXL;
    TypeInfrom SN_H0212YA;
    TypeInfrom SN_H0212YXL;
    TypeInfrom SN_H0213MA;
    TypeInfrom SN_H0213MXL;
    TypeInfrom SN_H0250KA;
    TypeInfrom SN_H0250KX;
    TypeInfrom SN_H0251CA;
    TypeInfrom SN_H0252YA;
    TypeInfrom SN_H0253MA;
    TypeInfrom SN_H0255A;
    TypeInfrom SN_H0255X;
    TypeInfrom SN_H0260KA;
    TypeInfrom SN_H0260KX;
    TypeInfrom SN_H0261CA;
    TypeInfrom SN_H0262YA;
    TypeInfrom SN_H0263MA;
    TypeInfrom SN_H0264KX;
    TypeInfrom SN_H0270KA;
    TypeInfrom SN_H0271CA;
    TypeInfrom SN_H0272YA;
    TypeInfrom SN_H0273MA;
    TypeInfrom SN_H0278A;
    TypeInfrom SN_H0279;
    TypeInfrom SN_H0280A;
    TypeInfrom SN_H0280X;
    TypeInfrom SN_H0281A;
    TypeInfrom SN_H0281X;
    TypeInfrom SN_H0285A;
    TypeInfrom SN_H0310KA;
    TypeInfrom SN_H0311CA;
    TypeInfrom SN_H0312YA;
    TypeInfrom SN_H0313MA;
    TypeInfrom SN_H0314D;
    TypeInfrom SN_H0325X;
    TypeInfrom SN_H0330KX;
    TypeInfrom SN_H0331CA;
    TypeInfrom SN_H0332YA;
    TypeInfrom SN_H0333MA;
    TypeInfrom SN_H0340KA;
    TypeInfrom SN_H0341CA;
    TypeInfrom SN_H0342YA;
    TypeInfrom SN_H0343MA;
    TypeInfrom SN_H0350KA;
    TypeInfrom SN_H0351CA;
    TypeInfrom SN_H0352YA;
    TypeInfrom SN_H0353MA;
    TypeInfrom SN_H0364A;
    TypeInfrom SN_H0364X;
    TypeInfrom SN_H0380KA;
    TypeInfrom SN_H0380KX;
    TypeInfrom SN_H0381CA;
    TypeInfrom SN_H0382YA;
    TypeInfrom SN_H0383MA;
    TypeInfrom SN_H0388;
    TypeInfrom SN_H0390A;
    TypeInfrom SN_H0390X;
    TypeInfrom SN_H0400KA;
    TypeInfrom SN_H0400KX;
    TypeInfrom SN_H0401CA;
    TypeInfrom SN_H0402YA;
    TypeInfrom SN_H0403MA;
    TypeInfrom SN_H0410KA;
    TypeInfrom SN_H0410KX;
    TypeInfrom SN_H0410KXL;
    TypeInfrom SN_H0411CA;
    TypeInfrom SN_H0411CXL;
    TypeInfrom SN_H0412YA;
    TypeInfrom SN_H0412YXL;
    TypeInfrom SN_H0413MA;
    TypeInfrom SN_H0413MXL;
    TypeInfrom SN_H0435A;
    TypeInfrom SN_H0436A;
    TypeInfrom SN_H0505A;
    TypeInfrom SN_H0505X;
    TypeInfrom SN_H0530KA;
    TypeInfrom SN_H0530KXL;
    TypeInfrom SN_H0531CA;
    TypeInfrom SN_H0531CXL;
    TypeInfrom SN_H0532YA;
    TypeInfrom SN_H0532YXL;
    TypeInfrom SN_H0533MA;
    TypeInfrom SN_H0533MXL;
    TypeInfrom SN_H0540KA;
    TypeInfrom SN_H0541CA;
    TypeInfrom SN_H0542YA;
    TypeInfrom SN_H0543MA;
    TypeInfrom SN_H0740KA;
    TypeInfrom SN_H0741CA;
    TypeInfrom SN_H0742YA;
    TypeInfrom SN_H0743MA;
    TypeInfrom SN_H0E320KA_NON;
    TypeInfrom SN_H0E321CA_NON;
    TypeInfrom SN_H0E322YA_NON;
    TypeInfrom SN_H0E323MA_NON;
    TypeInfrom SN_H0F320KA_NON;
    TypeInfrom SN_H0F320KX_NON;
    TypeInfrom SN_H0F321CA_NON;
    TypeInfrom SN_H0F322YA_NON;
    TypeInfrom SN_H0F323MA_NON;
    TypeInfrom SN_H0031CA_NON;
    TypeInfrom SN_H0032YA_NON;
    TypeInfrom SN_H0033MA_NON;
    TypeInfrom SN_H0210KA_NON;
    TypeInfrom SN_H0210KX_NON;
    TypeInfrom SN_H0210KXL_NON;
    TypeInfrom SN_H0211CA_NON;
    TypeInfrom SN_H0211CXL_NON;
    TypeInfrom SN_H0212YA_NON;
    TypeInfrom SN_H0212YXL_NON;
    TypeInfrom SN_H0213MA_NON;
    TypeInfrom SN_H0213MXL_NON;
    TypeInfrom SN_H0250KA_NON;
    TypeInfrom SN_H0250KX_NON;
    TypeInfrom SN_H0251CA_NON;
    TypeInfrom SN_H0252YA_NON;
    TypeInfrom SN_H0253MA_NON;
    TypeInfrom SN_H0255A_NON;
    TypeInfrom SN_H0255X_NON;
    TypeInfrom SN_H0260KA_NON;
    TypeInfrom SN_H0260KX_NON;
    TypeInfrom SN_H0261CA_NON;
    TypeInfrom SN_H0262YA_NON;
    TypeInfrom SN_H0263MA_NON;
    TypeInfrom SN_H0264KX_NON;
    TypeInfrom SN_H0270KA_NON;
    TypeInfrom SN_H0271CA_NON;
    TypeInfrom SN_H0272YA_NON;
    TypeInfrom SN_H0273MA_NON;
    TypeInfrom SN_H0278A_NON;
    TypeInfrom SN_H0279_NON;
    TypeInfrom SN_H0280A_NON;
    TypeInfrom SN_H0280X_NON;
    TypeInfrom SN_H0281A_NON;
    TypeInfrom SN_H0281X_NON;
    TypeInfrom SN_H0285A_NON;
    TypeInfrom SN_H0310KA_NON;
    TypeInfrom SN_H0311CA_NON;
    TypeInfrom SN_H0312YA_NON;
    TypeInfrom SN_H0313MA_NON;
    TypeInfrom SN_H0314D_NON;
    TypeInfrom SN_H0325X_NON;
    TypeInfrom SN_H0330KX_NON;
    TypeInfrom SN_H0331CA_NON;
    TypeInfrom SN_H0332YA_NON;
    TypeInfrom SN_H0333MA_NON;
    TypeInfrom SN_H0340KA_NON;
    TypeInfrom SN_H0341CA_NON;
    TypeInfrom SN_H0342YA_NON;
    TypeInfrom SN_H0343MA_NON;
    TypeInfrom SN_H0350KA_NON;
    TypeInfrom SN_H0351CA_NON;
    TypeInfrom SN_H0352YA_NON;
    TypeInfrom SN_H0353MA_NON;
    TypeInfrom SN_H0364A_NON;
    TypeInfrom SN_H0364X_NON;
    TypeInfrom SN_H0380KA_NON;
    TypeInfrom SN_H0380KX_NON;
    TypeInfrom SN_H0381CA_NON;
    TypeInfrom SN_H0382YA_NON;
    TypeInfrom SN_H0383MA_NON;
    TypeInfrom SN_H0388_NON;
    TypeInfrom SN_H0390A_NON;
    TypeInfrom SN_H0390X_NON;
    TypeInfrom SN_H0400KA_NON;
    TypeInfrom SN_H0400KX_NON;
    TypeInfrom SN_H0401CA_NON;
    TypeInfrom SN_H0402YA_NON;
    TypeInfrom SN_H0403MA_NON;
    TypeInfrom SN_H0410KA_NON;
    TypeInfrom SN_H0410KX_NON;
    TypeInfrom SN_H0410KXL_NON;
    TypeInfrom SN_H0411CA_NON;
    TypeInfrom SN_H0411CXL_NON;
    TypeInfrom SN_H0412YA_NON;
    TypeInfrom SN_H0412YXL_NON;
    TypeInfrom SN_H0413MA_NON;
    TypeInfrom SN_H0413MXL_NON;
    TypeInfrom SN_H0435A_NON;
    TypeInfrom SN_H0436A_NON;
    TypeInfrom SN_H0505A_NON;
    TypeInfrom SN_H0505X_NON;
    TypeInfrom SN_H0530KA_NON;
    TypeInfrom SN_H0530KXL_NON;
    TypeInfrom SN_H0531CA_NON;
    TypeInfrom SN_H0531CXL_NON;
    TypeInfrom SN_H0532YA_NON;
    TypeInfrom SN_H0532YXL_NON;
    TypeInfrom SN_H0533MA_NON;
    TypeInfrom SN_H0533MXL_NON;
    TypeInfrom SN_H0540KA_NON;
    TypeInfrom SN_H0541CA_NON;
    TypeInfrom SN_H0542YA_NON;
    TypeInfrom SN_H0543MA_NON;
    TypeInfrom SN_H0740KA_NON;
    TypeInfrom SN_H0741CA_NON;
    TypeInfrom SN_H0742YA_NON;
    TypeInfrom SN_H0743MA_NON;
    TypeInfrom SN_C0131U_KA;
    TypeInfrom SN_C0118U_C;
    TypeInfrom SN_C0118U_K;
    TypeInfrom SN_C0118U_M;
    TypeInfrom SN_C0118U_Y;
    TypeInfrom SN_C0119AU;
    TypeInfrom SN_C0119XU;
    TypeInfrom SN_C0120A;
    TypeInfrom SN_C0131U_C;
    TypeInfrom SN_C0131U_KX;
    TypeInfrom SN_C0131U_M;
    TypeInfrom SN_C0131U_Y;
    TypeInfrom SN_C0324A;
    TypeInfrom SN_C0324X;
    TypeInfrom SN_C0325A;
    TypeInfrom SN_C0328A;
    TypeInfrom SN_C0337;
    TypeInfrom SN_H0TRC_35N;
    TypeInfrom SN_H0TRK_35N;
    TypeInfrom SN_H0TRM_35N;
    TypeInfrom SN_H0TRY_35N;
    TypeInfrom SN_H0250U_KA;
    TypeInfrom SN_H0250U_KX;
    TypeInfrom SN_H0251U_C;
    TypeInfrom SN_H0252U_Y;
    TypeInfrom SN_H0253U_M;
    TypeInfrom SN_H0283AU;
    TypeInfrom SN_H0283XU;
    TypeInfrom SN_H0388U;
    TypeInfrom SN_H0400U_KA;
    TypeInfrom SN_H0400U_KX;
    TypeInfrom SN_H0401U_C;
    TypeInfrom SN_H0402U_Y;
    TypeInfrom SN_H0403U_M;
    TypeInfrom SN_H0436UA;
    TypeInfrom SN_H0436UX;
    TypeInfrom SN_H0505UA;
    TypeInfrom SN_H0505UX;
    TypeInfrom SN_H0540U_K;
    TypeInfrom SN_H0541U_C;
    TypeInfrom SN_H0542U_Y;
    TypeInfrom SN_H0543U_M;
    TypeInfrom SN_H0TRC_35N_NON;
    TypeInfrom SN_H0TRK_35N_NON;
    TypeInfrom SN_H0TRM_35N_NON;
    TypeInfrom SN_H0TRY_35N_NON;
    TypeInfrom SN_H0250U_KA_NON;
    TypeInfrom SN_H0250U_KX_NON;
    TypeInfrom SN_H0251U_C_NON;
    TypeInfrom SN_H0252U_Y_NON;
    TypeInfrom SN_H0253U_M_NON;
    TypeInfrom SN_H0283AU_NON;
    TypeInfrom SN_H0283XU_NON;
    TypeInfrom SN_H0388U_NON;
    TypeInfrom SN_H0400U_KA_NON;
    TypeInfrom SN_H0400U_KX_NON;
    TypeInfrom SN_H0401U_C_NON;
    TypeInfrom SN_H0402U_Y_NON;
    TypeInfrom SN_H0403U_M_NON;
    TypeInfrom SN_H0436UA_NON;
    TypeInfrom SN_H0436UX_NON;
    TypeInfrom SN_H0505UA_NON;
    TypeInfrom SN_H0505UX_NON;
    TypeInfrom SN_H0540U_K_NON;
    TypeInfrom SN_H0541U_C_NON;
    TypeInfrom SN_H0542U_Y_NON;
    TypeInfrom SN_H0543U_M_NON;


} FlashTable_Type;

typedef enum {
    /**
    * @brief  SD 响应及错误标志
    */
    SD_RESPONSE_NO_ERROR      = (0x00),
    SD_IN_IDLE_STATE          = (0x01),
    SD_ERASE_RESET            = (0x02),
    SD_ILLEGAL_COMMAND        = (0x04),
    SD_COM_CRC_ERROR          = (0x08),
    SD_ERASE_SEQUENCE_ERROR   = (0x10),
    SD_ADDRESS_ERROR          = (0x20),
    SD_PARAMETER_ERROR        = (0x40),
    SD_RESPONSE_FAILURE       = (0xFF),

    /**
    * @brief  数据响应类型
    */
    SD_DATA_OK                = (0x05),
    SD_DATA_CRC_ERROR         = (0x0B),
    SD_DATA_WRITE_ERROR       = (0x0D),
    SD_DATA_OTHER_ERROR       = (0xFF)
} SD_Error;

enum TypeEnmu
{
    H0E320KA = 0x0000,
    H0E321CA = 0x0001,
    H0E322YA = 0x0002,
    H0E323MA = 0x0003,
    H0F320KA = 0x0004,
    H0F320KX = 0x0005,
    H0F321CA = 0x0006,
    H0F322YA = 0x0007,
    H0F323MA = 0x0008,
    H0031CA = 0x0009,
    H0032YA = 0x000A,
    H0033MA = 0x000B,
    H0210KA = 0x000C,
    H0210KX = 0x000D,
    H0210KXL = 0x000E,
    H0211CA = 0x000F,
    H0211CXL = 0x0010,
    H0212YA = 0x0011,
    H0212YXL = 0x0012,
    H0213MA = 0x0013,
    H0213MXL = 0x0014,
    H0250KA = 0x0015,
    H0250KX = 0x0016,
    H0251CA = 0x0017,
    H0252YA = 0x0018,
    H0253MA = 0x0019,
    H0255A = 0x001A,
    H0255X = 0x001B,
    H0260KA = 0x001C,
    H0260KX = 0x001D,
    H0261CA = 0x001E,
    H0262YA = 0x001F,
    H0263MA = 0x0020,
    H0264KX = 0x0021,
    H0270KA = 0x0022,
    H0271CA = 0x0023,
    H0272YA = 0x0024,
    H0273MA = 0x0025,
    H0278A = 0x0026,
    H0279 = 0x0027,
    H0280A = 0x0028,
    H0280X = 0x0029,
    H0281A = 0x002A,
    H0281X = 0x002B,
    H0285A = 0x002C,
    H0310KA = 0x002D,
    H0311CA = 0x002E,
    H0312YA = 0x002F,
    H0313MA = 0x0030,
    H0314D = 0x0031,
    H0325X = 0x0032,
    H0330KX = 0x0033,
    H0331CA = 0x0034,
    H0332YA = 0x0035,
    H0333MA = 0x0036,
    H0340KA = 0x0037,
    H0341CA = 0x0038,
    H0342YA = 0x0039,
    H0343MA = 0x003A,
    H0350KA = 0x003B,
    H0351CA = 0x003C,
    H0352YA = 0x003D,
    H0353MA = 0x003E,
    H0364A = 0x003F,
    H0364X = 0x0040,
    H0380KA = 0x0041,
    H0380KX = 0x0042,
    H0381CA = 0x0043,
    H0382YA = 0x0044,
    H0383MA = 0x0045,
    H0388 = 0x0046,
    H0390A = 0x0047,
    H0390X = 0x0048,
    H0400KA = 0x0049,
    H0400KX = 0x004A,
    H0401CA = 0x004B,
    H0402YA = 0x004C,
    H0403MA = 0x004D,
    H0410KA = 0x004E,
    H0410KX = 0x004F,
    H0410KXL = 0x0050,
    H0411CA = 0x0051,
    H0411CXL = 0x0052,
    H0412YA = 0x0053,
    H0412YXL = 0x0054,
    H0413MA = 0x0055,
    H0413MXL = 0x0056,
    H0435A = 0x0057,
    H0436A = 0x0058,
    H0505A = 0x0059,
    H0505X = 0x005A,
    H0530KA = 0x005B,
    H0530KXL = 0x005C,
    H0531CA = 0x005D,
    H0531CXL = 0x005E,
    H0532YA = 0x005F,
    H0532YXL = 0x0060,
    H0533MA = 0x0061,
    H0533MXL = 0x0062,
    H0540KA = 0x0063,
    H0541CA = 0x0064,
    H0542YA = 0x0065,
    H0543MA = 0x0066,
    H0740KA = 0x0067,
    H0741CA = 0x0068,
    H0742YA = 0x0069,
    H0743MA = 0x006A,
    H0E320KA_NON = 0x006B,
    H0E321CA_NON = 0x006C,
    H0E322YA_NON = 0x006D,
    H0E323MA_NON = 0x006E,
    H0F320KA_NON = 0x006F,
    H0F320KX_NON = 0x0070,
    H0F321CA_NON = 0x0071,
    H0F322YA_NON = 0x0072,
    H0F323MA_NON = 0x0073,
    H0031CA_NON = 0x0074,
    H0032YA_NON = 0x0075,
    H0033MA_NON = 0x0076,
    H0210KA_NON = 0x0077,
    H0210KX_NON = 0x0078,
    H0210KXL_NON = 0x0079,
    H0211CA_NON = 0x007A,
    H0211CXL_NON = 0x007B,
    H0212YA_NON = 0x007C,
    H0212YXL_NON = 0x007D,
    H0213MA_NON = 0x007E,
    H0213MXL_NON = 0x007F,
    H0250KA_NON = 0x0080,
    H0250KX_NON = 0x0081,
    H0251CA_NON = 0x0082,
    H0252YA_NON = 0x0083,
    H0253MA_NON = 0x0084,
    H0255A_NON = 0x0085,
    H0255X_NON = 0x0086,
    H0260KA_NON = 0x0087,
    H0260KX_NON = 0x0088,
    H0261CA_NON = 0x0089,
    H0262YA_NON = 0x008A,
    H0263MA_NON = 0x008B,
    H0264KX_NON = 0x008C,
    H0270KA_NON = 0x008D,
    H0271CA_NON = 0x008E,
    H0272YA_NON = 0x008F,
    H0273MA_NON = 0x0090,
    H0278A_NON = 0x0091,
    H0279_NON = 0x0092,
    H0280A_NON = 0x0093,
    H0280X_NON = 0x0094,
    H0281A_NON = 0x0095,
    H0281X_NON = 0x0096,
    H0285A_NON = 0x0097,
    H0310KA_NON = 0x0098,
    H0311CA_NON = 0x0099,
    H0312YA_NON = 0x009A,
    H0313MA_NON = 0x009B,
    H0314D_NON = 0x009C,
    H0325X_NON = 0x009D,
    H0330KX_NON = 0x009E,
    H0331CA_NON = 0x009F,
    H0332YA_NON = 0x00A0,
    H0333MA_NON = 0x00A1,
    H0340KA_NON = 0x00A2,
    H0341CA_NON = 0x00A3,
    H0342YA_NON = 0x00A4,
    H0343MA_NON = 0x00A5,
    H0350KA_NON = 0x00A6,
    H0351CA_NON = 0x00A7,
    H0352YA_NON = 0x00A8,
    H0353MA_NON = 0x00A9,
    H0364A_NON = 0x00AA,
    H0364X_NON = 0x00AB,
    H0380KA_NON = 0x00AC,
    H0380KX_NON = 0x00AD,
    H0381CA_NON = 0x00AE,
    H0382YA_NON = 0x00AF,
    H0383MA_NON = 0x00B0,
    H0388_NON = 0x00B1,
    H0390A_NON = 0x00B2,
    H0390X_NON = 0x00B3,
    H0400KA_NON = 0x00B4,
    H0400KX_NON = 0x00B5,
    H0401CA_NON = 0x00B6,
    H0402YA_NON = 0x00B7,
    H0403MA_NON = 0x00B8,
    H0410KA_NON = 0x00B9,
    H0410KX_NON = 0x00BA,
    H0410KXL_NON = 0x00BB,
    H0411CA_NON = 0x00BC,
    H0411CXL_NON = 0x00BD,
    H0412YA_NON = 0x00BE,
    H0412YXL_NON = 0x00BF,
    H0413MA_NON = 0x00C0,
    H0413MXL_NON = 0x00C1,
    H0435A_NON = 0x00C2,
    H0436A_NON = 0x00C3,
    H0505A_NON = 0x00C4,
    H0505X_NON = 0x00C5,
    H0530KA_NON = 0x00C6,
    H0530KXL_NON = 0x00C7,
    H0531CA_NON = 0x00C8,
    H0531CXL_NON = 0x00C9,
    H0532YA_NON = 0x00CA,
    H0532YXL_NON = 0x00CB,
    H0533MA_NON = 0x00CC,
    H0533MXL_NON = 0x00CD,
    H0540KA_NON = 0x00CE,
    H0541CA_NON = 0x00CF,
    H0542YA_NON = 0x00D0,
    H0543MA_NON = 0x00D1,
    H0740KA_NON = 0x00D2,
    H0741CA_NON = 0x00D3,
    H0742YA_NON = 0x00D4,
    H0743MA_NON = 0x00D5,
    C0131U_KA = 0x00D6,
    C0118U_C = 0x00D7,
    C0118U_K = 0x00D8,
    C0118U_M = 0x00D9,
    C0118U_Y = 0x00DA,
    C0119AU = 0x00DB,
    C0119XU = 0x00DC,
    C0120A = 0x00DD,
    C0131U_C = 0x00DE,
    C0131U_KX = 0x00DF,
    C0131U_M = 0x00E0,
    C0131U_Y = 0x00E1,
    C0324A = 0x00E2,
    C0324X = 0x00E3,
    C0325A = 0x00E4,
    C0328A = 0x00E5,
    C0337 = 0x00E6,
    H0TRC_35N = 0x00E7,
    H0TRK_35N = 0x00E8,
    H0TRM_35N = 0x00E9,
    H0TRY_35N = 0x00EA,
    H0250U_KA = 0x00EB,
    H0250U_KX = 0x00EC,
    H0251U_C = 0x00ED,
    H0252U_Y = 0x00EE,
    H0253U_M = 0x00EF,
    H0283AU = 0x00F0,
    H0283XU = 0x00F1,
    H0388U = 0x00F2,
    H0400U_KA = 0x00F3,
    H0400U_KX = 0x00F4,
    H0401U_C = 0x00F5,
    H0402U_Y = 0x00F6,
    H0403U_M = 0x00F7,
    H0436UA = 0x00F8,
    H0436UX = 0x00F9,
    H0505UA = 0x00FA,
    H0505UX = 0x00FB,
    H0540U_K = 0x00FC,
    H0541U_C = 0x00FD,
    H0542U_Y = 0x00FE,
    H0543U_M = 0x00FF,
    H0TRC_35N_NON = 0x0100,
    H0TRK_35N_NON = 0x0101,
    H0TRM_35N_NON = 0x0102,
    H0TRY_35N_NON = 0x0103,
    H0250U_KA_NON = 0x0104,
    H0250U_KX_NON = 0x0105,
    H0251U_C_NON = 0x0106,
    H0252U_Y_NON = 0x0107,
    H0253U_M_NON = 0x0108,
    H0283AU_NON = 0x0109,
    H0283XU_NON = 0x010A,
    H0388U_NON = 0x010B,
    H0400U_KA_NON = 0x010C,
    H0400U_KX_NON = 0x010D,
    H0401U_C_NON = 0x010E,
    H0402U_Y_NON = 0x010F,
    H0403U_M_NON = 0x0110,
    H0436UA_NON = 0x0111,
    H0436UX_NON = 0x0112,
    H0505UA_NON = 0x0113,
    H0505UX_NON = 0x0114,
    H0540U_K_NON = 0x0115,
    H0541U_C_NON = 0x0116,
    H0542U_Y_NON = 0x0117,
    H0543U_M_NON = 0x0118,

};


extern u16 SD;                 

#define SDCard_CS       PBout(12)       

#define SD_START_DATA_SINGLE_BLOCK_READ    0xFE

/*!< Data token start byte, 多块读起始Token */
#define SD_START_DATA_MULTIPLE_BLOCK_READ  0xFE

/*!< Data token start byte, 单块写起始Token */
#define SD_START_DATA_SINGLE_BLOCK_WRITE   0xFE

/*!< Data token start byte, 多块写起始Token */
#define SD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFC

/*!< Data toke stop byte, 多块写停止Token */
#define SD_STOP_DATA_MULTIPLE_BLOCK_WRITE  0xFD


//SD卡的类型
#define SD_TYPE_NOT_SD   0	   //非SD卡
#define SD_TYPE_V1       1	  
#define SD_TYPE_V2       2    
#define SD_TYPE_V2HC     4	 
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

#define SD_CMD_GO_IDLE_STATE          0   	/*!< CMD0 = 0x40 */
#define SD_CMD_SEND_OP_COND           1   	/*!< CMD1 = 0x41 */
#define SD_CMD_SEND_IF_COND			  8	  	/*!< CMD8 = 0x48 */
#define SD_CMD_SEND_CSD               9   	/*!< CMD9 = 0x49 */
#define SD_CMD_SEND_CID               10  	/*!< CMD10 = 0x4A */
#define SD_CMD_STOP_TRANSMISSION      12  	/*!< CMD12 = 0x4C */
#define SD_CMD_SEND_STATUS            13  	/*!< CMD13 = 0x4D */
#define SD_CMD_SET_BLOCKLEN           16  	/*!< CMD16 = 0x50 */
#define SD_CMD_READ_SINGLE_BLOCK      17  	/*!< CMD17 = 0x51 */
#define SD_CMD_READ_MULT_BLOCK        18  	/*!< CMD18 = 0x52 */
#define SD_CMD_SET_BLOCK_COUNT        23  	/*!< CMD23 = 0x57 */
#define SD_CMD_WRITE_SINGLE_BLOCK     24  	/*!< CMD24 = 0x58 */
#define SD_CMD_WRITE_MULT_BLOCK       25  	/*!< CMD25 = 0x59 */
#define SD_CMD_PROG_CSD               27  	/*!< CMD27 = 0x5B */
#define SD_CMD_SET_WRITE_PROT         28  	/*!< CMD28 = 0x5C */
#define SD_CMD_CLR_WRITE_PROT         29  	/*!< CMD29 = 0x5D */
#define SD_CMD_SEND_WRITE_PROT        30  	/*!< CMD30 = 0x5E */
#define SD_CMD_SD_ERASE_GRP_START     32  	/*!< CMD32 = 0x60 */
#define SD_CMD_SD_ERASE_GRP_END       33  	/*!< CMD33 = 0x61 */
#define SD_CMD_UNTAG_SECTOR           34  	/*!< CMD34 = 0x62 */
#define SD_CMD_ERASE_GRP_START        35  	/*!< CMD35 = 0x63 */
#define SD_CMD_ERASE_GRP_END          36  	/*!< CMD36 = 0x64 */
#define SD_CMD_UNTAG_ERASE_GROUP      37  	/*!< CMD37 = 0x65 */
#define SD_CMD_ERASE                  38  	/*!< CMD38 = 0x66 */

#define SD_CMD_READ_OCR				  58    /*!< CMD58 */
#define SD_CMD_APP_CMD				  55    /*!< CMD55 返回0x01*/
#define SD_ACMD_SD_SEND_OP_COND		  41    /*!< ACMD41  返回0x00*/

 
#define SPI_CS_Select()				GPIO_ResetBits(GPIOB, GPIO_Pin_12) 
#define SPI_CS_Release()		    GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define SPI_CLK_HIGH()		        GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define SPI_CLK_LOW()				GPIO_ResetBits(GPIOB, GPIO_Pin_13) 

#define SPI_MOSI_HIGH()				GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define SPI_MOSI_LOW()		        GPIO_ResetBits(GPIOB, GPIO_Pin_15) 

#define SPI_MISO()                  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

#define DELAY_US    1

#define SD_DUMMY_BYTE   0xFF		//空 字节数据


u8 SD_Initialize(void);
void SPI_GPIO_Config(void);
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u16 CRC_check(u8 *point);
void Get_TableHead(void);
u8 Get_SN_number(TypeInfrom TypeSel, u32 SN_Count, u8 ReadNum);
u8 SD_SPI_ReadWriteByte(u8 data);
void SD_SendCmd(u8 cmd, u32 arg, u8 crc);
void SD_DisSelect(void);
u8 SD_WaitReady(void);
u8 SD_Select(void);
u8 SD_ReadDisk(u8* buf, u32 sector, u8 cnt);
static u8 SD_RecvData(u8 Token, u8 *RxBuf, u16 Size);
u8 SD_GetResponse(u8 Response);
u8 SD_WriteDisk(u8*buf, u32 sector, u8 cnt);
static u8 SD_WriteBlock(u8 *TxBuf, u8 Token);
u8 SD_GoIdleState(void);
static void SPI_Delay(__IO uint32_t count);
static u8 SD_ReadByte(void);
SD_Error SD_GetCardType(void);
static u8 SD_WriteByte(u8 TxData);
uint8_t SD_GetDataResponse(void);
static u8 SD_WriteReadByte(u8 TxData);
u8 SPI_ReadWriteByte(u8 Data);
#endif
















