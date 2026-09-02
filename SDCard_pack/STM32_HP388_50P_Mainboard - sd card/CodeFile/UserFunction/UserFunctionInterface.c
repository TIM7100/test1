#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "commandlist_HP388.h"
#include "menu.h"
#include "user_OS.h"
#include "SD.h"
#include "user_Peripheral.h"
#include "BurnFunction.h"
#include "CheckFunction.h"
#include "UserFunctionInterface.h"

FlashTable_Type FlashTable;
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     basicUserInterface                                                                        */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*              _NONe                                                                                      */
/* Returns:                                                                                                */
/*              _NONe                                                                                      */
/* Description:                                                                                            */
/*               基础用户界面                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
void UserFunctionInterface(void)
{
    u16 KeyPressed;
    u8  EEDataTotal;

    Confirm_Selection(MenuPoint[UserChoose].DisplayString);

    while (1)   /*  基础用户界面循环,只接受Enter键(执行命令)和Esc键(返回父目录) */
    {
        KeyPressed = 0x0000;
        KeyPressed = AutoKeyScan();


        if (KeyPressed == Key_Enter)             /* 执行对应命令 */
        {
            EEDataTotal = MenuPoint[UserChoose].EETotal;
            switch (MenuPoint[UserChoose].Command)
            {
            case    SHOWVERSIONSELECT:                              /* 显示版本功能 */
                GetVersionScreen();
                ShowVersionInterface();
                break;
            }

            switch (MenuPoint[UserChoose].Command)
            {
            case  BURPROGRAM_H0400U_KA :
            case  BURPROGRAM_H0400U_KX :
            case  BURPROGRAM_H0401U_C  :
            case  BURPROGRAM_H0402U_Y  :
            case  BURPROGRAM_H0403U_M  :
            case  BURPROGRAM_H0436UA   :
            case  BURPROGRAM_H0436UX   :
            case  BURPROGRAM_H0400U_KA_NON  :
            case  BURPROGRAM_H0400U_KX_NON  :
            case  BURPROGRAM_H0401U_C_NON   :
            case  BURPROGRAM_H0402U_Y_NON   :
            case  BURPROGRAM_H0403U_M_NON   :
            case  BURPROGRAM_H0436UA_NON    :
            case  BURPROGRAM_H0436UX_NON    :
                EEDataTotal = 7;
                break;
            case  BURPROGRAM_H0388U    :
            case  BURPROGRAM_H0388U_NON     :
                EEDataTotal = 6;
                break;
            case  BURPROGRAM_H0250U_KA :
            case  BURPROGRAM_H0250U_KX :
            case  BURPROGRAM_H0251U_C  :
            case  BURPROGRAM_H0252U_Y  :
            case  BURPROGRAM_H0253U_M  :
            case  BURPROGRAM_H0250U_KA_NON  :
            case  BURPROGRAM_H0250U_KX_NON  :
            case  BURPROGRAM_H0251U_C_NON   :
            case  BURPROGRAM_H0252U_Y_NON   :
            case  BURPROGRAM_H0253U_M_NON   :
                EEDataTotal = 5;
                break;
            case  BURPROGRAM_H0283AU   :
            case  BURPROGRAM_H0283XU   :
            case  BURPROGRAM_H0283AU_NON    :
            case  BURPROGRAM_H0283XU_NON    :
            case BURPROGRAM_H0388:
            case BURPROGRAM_H0388_NON:
                EEDataTotal = 4;
                break;
            case  BURPROGRAM_H0TRC_35N :
            case  BURPROGRAM_H0TRK_35N :
            case  BURPROGRAM_H0TRM_35N :
            case  BURPROGRAM_H0TRY_35N :
            case  BURPROGRAM_H0TRC_35N_NON  :
            case  BURPROGRAM_H0TRK_35N_NON  :
            case  BURPROGRAM_H0TRM_35N_NON  :
            case  BURPROGRAM_H0TRY_35N_NON  :
                EEDataTotal = 3;
                break;
            case  BURPROGRAM_H0505UA   :
            case  BURPROGRAM_H0505UX   :
            case  BURPROGRAM_H0540U_K  :
            case  BURPROGRAM_H0541U_C  :
            case  BURPROGRAM_H0542U_Y  :
            case  BURPROGRAM_H0543U_M  :
            case  BURPROGRAM_H0505UA_NON    :
            case  BURPROGRAM_H0505UX_NON    :
            case  BURPROGRAM_H0540U_K_NON   :
            case  BURPROGRAM_H0541U_C_NON   :
            case  BURPROGRAM_H0542U_Y_NON   :
            case  BURPROGRAM_H0543U_M_NON   :
            case BURPROGRAM_C0119AU:
            case BURPROGRAM_C0119XU:
                EEDataTotal = 2;
                break;
            default:
                EEDataTotal = 1;
                break;
            }
            //开始烧录
            switch (MenuPoint[UserChoose].Command)
            {

            case    BURPROGRAM_H0E320KA:
                ModifBurnInterface(FlashTable.SN_H0E320KA, H0E320KA, BURN_H0E320KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0E321CA:
                ModifBurnInterface(FlashTable.SN_H0E321CA, H0E321CA, BURN_H0E321CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0E322YA:
                ModifBurnInterface(FlashTable.SN_H0E322YA, H0E322YA, BURN_H0E322YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0E323MA:
                ModifBurnInterface(FlashTable.SN_H0E323MA, H0E323MA, BURN_H0E323MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0F320KA:
                ModifBurnInterface(FlashTable.SN_H0F320KA, H0F320KA, BURN_H0F320KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0F320KX:
                ModifBurnInterface(FlashTable.SN_H0F320KX, H0F320KX, BURN_H0F320KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0F321CA:
                ModifBurnInterface(FlashTable.SN_H0F321CA, H0F321CA, BURN_H0F321CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0F322YA:
                ModifBurnInterface(FlashTable.SN_H0F322YA, H0F322YA, BURN_H0F322YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0F323MA:
                ModifBurnInterface(FlashTable.SN_H0F323MA, H0F323MA, BURN_H0F323MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0031CA:
                ModifBurnInterface(FlashTable.SN_H0031CA, H0031CA, BURN_H0031CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0032YA:
                ModifBurnInterface(FlashTable.SN_H0032YA, H0032YA, BURN_H0032YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0033MA:
                ModifBurnInterface(FlashTable.SN_H0033MA, H0033MA, BURN_H0033MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KA:
                ModifBurnInterface(FlashTable.SN_H0210KA, H0210KA, BURN_H0210KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KX:
                ModifBurnInterface(FlashTable.SN_H0210KX, H0210KX, BURN_H0210KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KXL:
                ModifBurnInterface(FlashTable.SN_H0210KXL, H0210KXL, BURN_H0210KXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0211CA:
                ModifBurnInterface(FlashTable.SN_H0211CA, H0211CA, BURN_H0211CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0211CXL:
                ModifBurnInterface(FlashTable.SN_H0211CXL, H0211CXL, BURN_H0211CXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0212YA:
                ModifBurnInterface(FlashTable.SN_H0212YA, H0212YA, BURN_H0212YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0212YXL:
                ModifBurnInterface(FlashTable.SN_H0212YXL, H0212YXL, BURN_H0212YXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0213MA:
                ModifBurnInterface(FlashTable.SN_H0213MA, H0213MA, BURN_H0213MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0213MXL:
                ModifBurnInterface(FlashTable.SN_H0213MXL, H0213MXL, BURN_H0213MXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0250KA:
                ModifBurnInterface(FlashTable.SN_H0250KA, H0250KA, BURN_H0250KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0250KX:
                ModifBurnInterface(FlashTable.SN_H0250KX, H0250KX, BURN_H0250KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0251CA:
                ModifBurnInterface(FlashTable.SN_H0251CA, H0251CA, BURN_H0251CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0252YA:
                ModifBurnInterface(FlashTable.SN_H0252YA, H0252YA, BURN_H0252YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0253MA:
                ModifBurnInterface(FlashTable.SN_H0253MA, H0253MA, BURN_H0253MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0255A:
                ModifBurnInterface(FlashTable.SN_H0255A, H0255A, BURN_H0255A, EEDataTotal);
                break;
            case    BURPROGRAM_H0255X:
                ModifBurnInterface(FlashTable.SN_H0255X, H0255X, BURN_H0255X, EEDataTotal);
                break;
            case    BURPROGRAM_H0260KA:
                ModifBurnInterface(FlashTable.SN_H0260KA, H0260KA, BURN_H0260KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0260KX:
                ModifBurnInterface(FlashTable.SN_H0260KX, H0260KX, BURN_H0260KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0261CA:
                ModifBurnInterface(FlashTable.SN_H0261CA, H0261CA, BURN_H0261CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0262YA:
                ModifBurnInterface(FlashTable.SN_H0262YA, H0262YA, BURN_H0262YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0263MA:
                ModifBurnInterface(FlashTable.SN_H0263MA, H0263MA, BURN_H0263MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0264KX:
                ModifBurnInterface(FlashTable.SN_H0264KX, H0264KX, BURN_H0264KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0270KA:
                ModifBurnInterface(FlashTable.SN_H0270KA, H0270KA, BURN_H0270KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0271CA:
                ModifBurnInterface(FlashTable.SN_H0271CA, H0271CA, BURN_H0271CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0272YA:
                ModifBurnInterface(FlashTable.SN_H0272YA, H0272YA, BURN_H0272YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0273MA:
                ModifBurnInterface(FlashTable.SN_H0273MA, H0273MA, BURN_H0273MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0278A:
                ModifBurnInterface(FlashTable.SN_H0278A, H0278A, BURN_H0278A, EEDataTotal);
                break;
            case    BURPROGRAM_H0279:
                ModifBurnInterface(FlashTable.SN_H0279, H0279, BURN_H0279, EEDataTotal);
                break;
            case    BURPROGRAM_H0280A:
                ModifBurnInterface(FlashTable.SN_H0280A, H0280A, BURN_H0280A, EEDataTotal);
                break;
            case    BURPROGRAM_H0280X:
                ModifBurnInterface(FlashTable.SN_H0280X, H0280X, BURN_H0280X, EEDataTotal);
                break;
            case    BURPROGRAM_H0281A:
                ModifBurnInterface(FlashTable.SN_H0281A, H0281A, BURN_H0281A, EEDataTotal);
                break;
            case    BURPROGRAM_H0281X:
                ModifBurnInterface(FlashTable.SN_H0281X, H0281X, BURN_H0281X, EEDataTotal);
                break;
            case    BURPROGRAM_H0285A:
                ModifBurnInterface(FlashTable.SN_H0285A, H0285A, BURN_H0285A, EEDataTotal);
                break;
            case    BURPROGRAM_H0310KA:
                ModifBurnInterface(FlashTable.SN_H0310KA, H0310KA, BURN_H0310KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0311CA:
                ModifBurnInterface(FlashTable.SN_H0311CA, H0311CA, BURN_H0311CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0312YA:
                ModifBurnInterface(FlashTable.SN_H0312YA, H0312YA, BURN_H0312YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0313MA:
                ModifBurnInterface(FlashTable.SN_H0313MA, H0313MA, BURN_H0313MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0314D:
                ModifBurnInterface(FlashTable.SN_H0314D, H0314D, BURN_H0314D, EEDataTotal);
                break;
            case    BURPROGRAM_H0325X:
                ModifBurnInterface(FlashTable.SN_H0325X, H0325X, BURN_H0325X, EEDataTotal);
                break;
            case    BURPROGRAM_H0330KX:
                ModifBurnInterface(FlashTable.SN_H0330KX, H0330KX, BURN_H0330KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0331CA:
                ModifBurnInterface(FlashTable.SN_H0331CA, H0331CA, BURN_H0331CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0332YA:
                ModifBurnInterface(FlashTable.SN_H0332YA, H0332YA, BURN_H0332YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0333MA:
                ModifBurnInterface(FlashTable.SN_H0333MA, H0333MA, BURN_H0333MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0340KA:
                ModifBurnInterface(FlashTable.SN_H0340KA, H0340KA, BURN_H0340KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0341CA:
                ModifBurnInterface(FlashTable.SN_H0341CA, H0341CA, BURN_H0341CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0342YA:
                ModifBurnInterface(FlashTable.SN_H0342YA, H0342YA, BURN_H0342YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0343MA:
                ModifBurnInterface(FlashTable.SN_H0343MA, H0343MA, BURN_H0343MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0350KA:
                ModifBurnInterface(FlashTable.SN_H0350KA, H0350KA, BURN_H0350KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0351CA:
                ModifBurnInterface(FlashTable.SN_H0351CA, H0351CA, BURN_H0351CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0352YA:
                ModifBurnInterface(FlashTable.SN_H0352YA, H0352YA, BURN_H0352YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0353MA:
                ModifBurnInterface(FlashTable.SN_H0353MA, H0353MA, BURN_H0353MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0364A:
                ModifBurnInterface(FlashTable.SN_H0364A, H0364A, BURN_H0364A, EEDataTotal);
                break;
            case    BURPROGRAM_H0364X:
                ModifBurnInterface(FlashTable.SN_H0364X, H0364X, BURN_H0364X, EEDataTotal);
                break;
            case    BURPROGRAM_H0380KA:
                ModifBurnInterface(FlashTable.SN_H0380KA, H0380KA, BURN_H0380KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0380KX:
                ModifBurnInterface(FlashTable.SN_H0380KX, H0380KX, BURN_H0380KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0381CA:
                ModifBurnInterface(FlashTable.SN_H0381CA, H0381CA, BURN_H0381CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0382YA:
                ModifBurnInterface(FlashTable.SN_H0382YA, H0382YA, BURN_H0382YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0383MA:
                ModifBurnInterface(FlashTable.SN_H0383MA, H0383MA, BURN_H0383MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0388:
                ModifBurnInterface(FlashTable.SN_H0388, H0388, BURN_H0388, EEDataTotal);
                break;
            case    BURPROGRAM_H0390A:
                ModifBurnInterface(FlashTable.SN_H0390A, H0390A, BURN_H0390A, EEDataTotal);
                break;
            case    BURPROGRAM_H0390X:
                ModifBurnInterface(FlashTable.SN_H0390X, H0390X, BURN_H0390X, EEDataTotal);
                break;
            case    BURPROGRAM_H0400KA:
                ModifBurnInterface(FlashTable.SN_H0400KA, H0400KA, BURN_H0400KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0400KX:
                ModifBurnInterface(FlashTable.SN_H0400KX, H0400KX, BURN_H0400KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0401CA:
                ModifBurnInterface(FlashTable.SN_H0401CA, H0401CA, BURN_H0401CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0402YA:
                ModifBurnInterface(FlashTable.SN_H0402YA, H0402YA, BURN_H0402YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0403MA:
                ModifBurnInterface(FlashTable.SN_H0403MA, H0403MA, BURN_H0403MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KA:
                ModifBurnInterface(FlashTable.SN_H0410KA, H0410KA, BURN_H0410KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KX:
                ModifBurnInterface(FlashTable.SN_H0410KX, H0410KX, BURN_H0410KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KXL:
                ModifBurnInterface(FlashTable.SN_H0410KXL, H0410KXL, BURN_H0410KXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0411CA:
                ModifBurnInterface(FlashTable.SN_H0411CA, H0411CA, BURN_H0411CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0411CXL:
                ModifBurnInterface(FlashTable.SN_H0411CXL, H0411CXL, BURN_H0411CXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0412YA:
                ModifBurnInterface(FlashTable.SN_H0412YA, H0412YA, BURN_H0412YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0412YXL:
                ModifBurnInterface(FlashTable.SN_H0412YXL, H0412YXL, BURN_H0412YXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0413MA:
                ModifBurnInterface(FlashTable.SN_H0413MA, H0413MA, BURN_H0413MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0413MXL:
                ModifBurnInterface(FlashTable.SN_H0413MXL, H0413MXL, BURN_H0413MXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0435A:
                ModifBurnInterface(FlashTable.SN_H0435A, H0435A, BURN_H0435A, EEDataTotal);
                break;
            case    BURPROGRAM_H0436A:
                ModifBurnInterface(FlashTable.SN_H0436A, H0436A, BURN_H0436A, EEDataTotal);
                break;
            case    BURPROGRAM_H0505A:
                ModifBurnInterface(FlashTable.SN_H0505A, H0505A, BURN_H0505A, EEDataTotal);
                break;
            case    BURPROGRAM_H0505X:
                ModifBurnInterface(FlashTable.SN_H0505X, H0505X, BURN_H0505X, EEDataTotal);
                break;
            case    BURPROGRAM_H0530KA:
                ModifBurnInterface(FlashTable.SN_H0530KA, H0530KA, BURN_H0530KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0530KXL:
                ModifBurnInterface(FlashTable.SN_H0530KXL, H0530KXL, BURN_H0530KXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0531CA:
                ModifBurnInterface(FlashTable.SN_H0531CA, H0531CA, BURN_H0531CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0531CXL:
                ModifBurnInterface(FlashTable.SN_H0531CXL, H0531CXL, BURN_H0531CXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0532YA:
                ModifBurnInterface(FlashTable.SN_H0532YA, H0532YA, BURN_H0532YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0532YXL:
                ModifBurnInterface(FlashTable.SN_H0532YXL, H0532YXL, BURN_H0532YXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0533MA:
                ModifBurnInterface(FlashTable.SN_H0533MA, H0533MA, BURN_H0533MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0533MXL:
                ModifBurnInterface(FlashTable.SN_H0533MXL, H0533MXL, BURN_H0533MXL, EEDataTotal);
                break;
            case    BURPROGRAM_H0540KA:
                ModifBurnInterface(FlashTable.SN_H0540KA, H0540KA, BURN_H0540KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0541CA:
                ModifBurnInterface(FlashTable.SN_H0541CA, H0541CA, BURN_H0541CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0542YA:
                ModifBurnInterface(FlashTable.SN_H0542YA, H0542YA, BURN_H0542YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0543MA:
                ModifBurnInterface(FlashTable.SN_H0543MA, H0543MA, BURN_H0543MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0740KA:
                ModifBurnInterface(FlashTable.SN_H0740KA, H0740KA, BURN_H0740KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0741CA:
                ModifBurnInterface(FlashTable.SN_H0741CA, H0741CA, BURN_H0741CA, EEDataTotal);
                break;
            case    BURPROGRAM_H0742YA:
                ModifBurnInterface(FlashTable.SN_H0742YA, H0742YA, BURN_H0742YA, EEDataTotal);
                break;
            case    BURPROGRAM_H0743MA:
                ModifBurnInterface(FlashTable.SN_H0743MA, H0743MA, BURN_H0743MA, EEDataTotal);
                break;
            case    BURPROGRAM_H0E320KA_NON:
                ModifBurnInterface(FlashTable.SN_H0E320KA_NON, H0E320KA_NON, BURN_H0E320KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0E321CA_NON:
                ModifBurnInterface(FlashTable.SN_H0E321CA_NON, H0E321CA_NON, BURN_H0E321CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0E322YA_NON:
                ModifBurnInterface(FlashTable.SN_H0E322YA_NON, H0E322YA_NON, BURN_H0E322YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0E323MA_NON:
                ModifBurnInterface(FlashTable.SN_H0E323MA_NON, H0E323MA_NON, BURN_H0E323MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0F320KA_NON:
                ModifBurnInterface(FlashTable.SN_H0F320KA_NON, H0F320KA_NON, BURN_H0F320KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0F320KX_NON:
                ModifBurnInterface(FlashTable.SN_H0F320KX_NON, H0F320KX_NON, BURN_H0F320KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0F321CA_NON:
                ModifBurnInterface(FlashTable.SN_H0F321CA_NON, H0F321CA_NON, BURN_H0F321CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0F322YA_NON:
                ModifBurnInterface(FlashTable.SN_H0F322YA_NON, H0F322YA_NON, BURN_H0F322YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0F323MA_NON:
                ModifBurnInterface(FlashTable.SN_H0F323MA_NON, H0F323MA_NON, BURN_H0F323MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0031CA_NON:
                ModifBurnInterface(FlashTable.SN_H0031CA_NON, H0031CA_NON, BURN_H0031CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0032YA_NON:
                ModifBurnInterface(FlashTable.SN_H0032YA_NON, H0032YA_NON, BURN_H0032YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0033MA_NON:
                ModifBurnInterface(FlashTable.SN_H0033MA_NON, H0033MA_NON, BURN_H0033MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KA_NON:
                ModifBurnInterface(FlashTable.SN_H0210KA_NON, H0210KA_NON, BURN_H0210KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KX_NON:
                ModifBurnInterface(FlashTable.SN_H0210KX_NON, H0210KX_NON, BURN_H0210KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0210KXL_NON:
                ModifBurnInterface(FlashTable.SN_H0210KXL_NON, H0210KXL_NON, BURN_H0210KXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0211CA_NON:
                ModifBurnInterface(FlashTable.SN_H0211CA_NON, H0211CA_NON, BURN_H0211CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0211CXL_NON:
                ModifBurnInterface(FlashTable.SN_H0211CXL_NON, H0211CXL_NON, BURN_H0211CXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0212YA_NON:
                ModifBurnInterface(FlashTable.SN_H0212YA_NON, H0212YA_NON, BURN_H0212YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0212YXL_NON:
                ModifBurnInterface(FlashTable.SN_H0212YXL_NON, H0212YXL_NON, BURN_H0212YXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0213MA_NON:
                ModifBurnInterface(FlashTable.SN_H0213MA_NON, H0213MA_NON, BURN_H0213MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0213MXL_NON:
                ModifBurnInterface(FlashTable.SN_H0213MXL_NON, H0213MXL_NON, BURN_H0213MXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0250KA_NON:
                ModifBurnInterface(FlashTable.SN_H0250KA_NON, H0250KA_NON, BURN_H0250KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0250KX_NON:
                ModifBurnInterface(FlashTable.SN_H0250KX_NON, H0250KX_NON, BURN_H0250KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0251CA_NON:
                ModifBurnInterface(FlashTable.SN_H0251CA_NON, H0251CA_NON, BURN_H0251CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0252YA_NON:
                ModifBurnInterface(FlashTable.SN_H0252YA_NON, H0252YA_NON, BURN_H0252YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0253MA_NON:
                ModifBurnInterface(FlashTable.SN_H0253MA_NON, H0253MA_NON, BURN_H0253MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0255A_NON:
                ModifBurnInterface(FlashTable.SN_H0255A_NON, H0255A_NON, BURN_H0255A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0255X_NON:
                ModifBurnInterface(FlashTable.SN_H0255X_NON, H0255X_NON, BURN_H0255X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0260KA_NON:
                ModifBurnInterface(FlashTable.SN_H0260KA_NON, H0260KA_NON, BURN_H0260KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0260KX_NON:
                ModifBurnInterface(FlashTable.SN_H0260KX_NON, H0260KX_NON, BURN_H0260KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0261CA_NON:
                ModifBurnInterface(FlashTable.SN_H0261CA_NON, H0261CA_NON, BURN_H0261CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0262YA_NON:
                ModifBurnInterface(FlashTable.SN_H0262YA_NON, H0262YA_NON, BURN_H0262YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0263MA_NON:
                ModifBurnInterface(FlashTable.SN_H0263MA_NON, H0263MA_NON, BURN_H0263MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0264KX_NON:
                ModifBurnInterface(FlashTable.SN_H0264KX_NON, H0264KX_NON, BURN_H0264KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0270KA_NON:
                ModifBurnInterface(FlashTable.SN_H0270KA_NON, H0270KA_NON, BURN_H0270KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0271CA_NON:
                ModifBurnInterface(FlashTable.SN_H0271CA_NON, H0271CA_NON, BURN_H0271CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0272YA_NON:
                ModifBurnInterface(FlashTable.SN_H0272YA_NON, H0272YA_NON, BURN_H0272YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0273MA_NON:
                ModifBurnInterface(FlashTable.SN_H0273MA_NON, H0273MA_NON, BURN_H0273MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0278A_NON:
                ModifBurnInterface(FlashTable.SN_H0278A_NON, H0278A_NON, BURN_H0278A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0279_NON:
                ModifBurnInterface(FlashTable.SN_H0279_NON, H0279_NON, BURN_H0279_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0280A_NON:
                ModifBurnInterface(FlashTable.SN_H0280A_NON, H0280A_NON, BURN_H0280A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0280X_NON:
                ModifBurnInterface(FlashTable.SN_H0280X_NON, H0280X_NON, BURN_H0280X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0281A_NON:
                ModifBurnInterface(FlashTable.SN_H0281A_NON, H0281A_NON, BURN_H0281A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0281X_NON:
                ModifBurnInterface(FlashTable.SN_H0281X_NON, H0281X_NON, BURN_H0281X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0285A_NON:
                ModifBurnInterface(FlashTable.SN_H0285A_NON, H0285A_NON, BURN_H0285A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0310KA_NON:
                ModifBurnInterface(FlashTable.SN_H0310KA_NON, H0310KA_NON, BURN_H0310KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0311CA_NON:
                ModifBurnInterface(FlashTable.SN_H0311CA_NON, H0311CA_NON, BURN_H0311CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0312YA_NON:
                ModifBurnInterface(FlashTable.SN_H0312YA_NON, H0312YA_NON, BURN_H0312YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0313MA_NON:
                ModifBurnInterface(FlashTable.SN_H0313MA_NON, H0313MA_NON, BURN_H0313MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0314D_NON:
                ModifBurnInterface(FlashTable.SN_H0314D_NON, H0314D_NON, BURN_H0314D_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0325X_NON:
                ModifBurnInterface(FlashTable.SN_H0325X_NON, H0325X_NON, BURN_H0325X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0330KX_NON:
                ModifBurnInterface(FlashTable.SN_H0330KX_NON, H0330KX_NON, BURN_H0330KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0331CA_NON:
                ModifBurnInterface(FlashTable.SN_H0331CA_NON, H0331CA_NON, BURN_H0331CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0332YA_NON:
                ModifBurnInterface(FlashTable.SN_H0332YA_NON, H0332YA_NON, BURN_H0332YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0333MA_NON:
                ModifBurnInterface(FlashTable.SN_H0333MA_NON, H0333MA_NON, BURN_H0333MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0340KA_NON:
                ModifBurnInterface(FlashTable.SN_H0340KA_NON, H0340KA_NON, BURN_H0340KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0341CA_NON:
                ModifBurnInterface(FlashTable.SN_H0341CA_NON, H0341CA_NON, BURN_H0341CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0342YA_NON:
                ModifBurnInterface(FlashTable.SN_H0342YA_NON, H0342YA_NON, BURN_H0342YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0343MA_NON:
                ModifBurnInterface(FlashTable.SN_H0343MA_NON, H0343MA_NON, BURN_H0343MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0350KA_NON:
                ModifBurnInterface(FlashTable.SN_H0350KA_NON, H0350KA_NON, BURN_H0350KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0351CA_NON:
                ModifBurnInterface(FlashTable.SN_H0351CA_NON, H0351CA_NON, BURN_H0351CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0352YA_NON:
                ModifBurnInterface(FlashTable.SN_H0352YA_NON, H0352YA_NON, BURN_H0352YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0353MA_NON:
                ModifBurnInterface(FlashTable.SN_H0353MA_NON, H0353MA_NON, BURN_H0353MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0364A_NON:
                ModifBurnInterface(FlashTable.SN_H0364A_NON, H0364A_NON, BURN_H0364A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0364X_NON:
                ModifBurnInterface(FlashTable.SN_H0364X_NON, H0364X_NON, BURN_H0364X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0380KA_NON:
                ModifBurnInterface(FlashTable.SN_H0380KA_NON, H0380KA_NON, BURN_H0380KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0380KX_NON:
                ModifBurnInterface(FlashTable.SN_H0380KX_NON, H0380KX_NON, BURN_H0380KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0381CA_NON:
                ModifBurnInterface(FlashTable.SN_H0381CA_NON, H0381CA_NON, BURN_H0381CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0382YA_NON:
                ModifBurnInterface(FlashTable.SN_H0382YA_NON, H0382YA_NON, BURN_H0382YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0383MA_NON:
                ModifBurnInterface(FlashTable.SN_H0383MA_NON, H0383MA_NON, BURN_H0383MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0388_NON:
                ModifBurnInterface(FlashTable.SN_H0388_NON, H0388_NON, BURN_H0388_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0390A_NON:
                ModifBurnInterface(FlashTable.SN_H0390A_NON, H0390A_NON, BURN_H0390A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0390X_NON:
                ModifBurnInterface(FlashTable.SN_H0390X_NON, H0390X_NON, BURN_H0390X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0400KA_NON:
                ModifBurnInterface(FlashTable.SN_H0400KA_NON, H0400KA_NON, BURN_H0400KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0400KX_NON:
                ModifBurnInterface(FlashTable.SN_H0400KX_NON, H0400KX_NON, BURN_H0400KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0401CA_NON:
                ModifBurnInterface(FlashTable.SN_H0401CA_NON, H0401CA_NON, BURN_H0401CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0402YA_NON:
                ModifBurnInterface(FlashTable.SN_H0402YA_NON, H0402YA_NON, BURN_H0402YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0403MA_NON:
                ModifBurnInterface(FlashTable.SN_H0403MA_NON, H0403MA_NON, BURN_H0403MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KA_NON:
                ModifBurnInterface(FlashTable.SN_H0410KA_NON, H0410KA_NON, BURN_H0410KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KX_NON:
                ModifBurnInterface(FlashTable.SN_H0410KX_NON, H0410KX_NON, BURN_H0410KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0410KXL_NON:
                ModifBurnInterface(FlashTable.SN_H0410KXL_NON, H0410KXL_NON, BURN_H0410KXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0411CA_NON:
                ModifBurnInterface(FlashTable.SN_H0411CA_NON, H0411CA_NON, BURN_H0411CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0411CXL_NON:
                ModifBurnInterface(FlashTable.SN_H0411CXL_NON, H0411CXL_NON, BURN_H0411CXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0412YA_NON:
                ModifBurnInterface(FlashTable.SN_H0412YA_NON, H0412YA_NON, BURN_H0412YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0412YXL_NON:
                ModifBurnInterface(FlashTable.SN_H0412YXL_NON, H0412YXL_NON, BURN_H0412YXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0413MA_NON:
                ModifBurnInterface(FlashTable.SN_H0413MA_NON, H0413MA_NON, BURN_H0413MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0413MXL_NON:
                ModifBurnInterface(FlashTable.SN_H0413MXL_NON, H0413MXL_NON, BURN_H0413MXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0435A_NON:
                ModifBurnInterface(FlashTable.SN_H0435A_NON, H0435A_NON, BURN_H0435A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0436A_NON:
                ModifBurnInterface(FlashTable.SN_H0436A_NON, H0436A_NON, BURN_H0436A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0505A_NON:
                ModifBurnInterface(FlashTable.SN_H0505A_NON, H0505A_NON, BURN_H0505A_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0505X_NON:
                ModifBurnInterface(FlashTable.SN_H0505X_NON, H0505X_NON, BURN_H0505X_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0530KA_NON:
                ModifBurnInterface(FlashTable.SN_H0530KA_NON, H0530KA_NON, BURN_H0530KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0530KXL_NON:
                ModifBurnInterface(FlashTable.SN_H0530KXL_NON, H0530KXL_NON, BURN_H0530KXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0531CA_NON:
                ModifBurnInterface(FlashTable.SN_H0531CA_NON, H0531CA_NON, BURN_H0531CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0531CXL_NON:
                ModifBurnInterface(FlashTable.SN_H0531CXL_NON, H0531CXL_NON, BURN_H0531CXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0532YA_NON:
                ModifBurnInterface(FlashTable.SN_H0532YA_NON, H0532YA_NON, BURN_H0532YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0532YXL_NON:
                ModifBurnInterface(FlashTable.SN_H0532YXL_NON, H0532YXL_NON, BURN_H0532YXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0533MA_NON:
                ModifBurnInterface(FlashTable.SN_H0533MA_NON, H0533MA_NON, BURN_H0533MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0533MXL_NON:
                ModifBurnInterface(FlashTable.SN_H0533MXL_NON, H0533MXL_NON, BURN_H0533MXL_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0540KA_NON:
                ModifBurnInterface(FlashTable.SN_H0540KA_NON, H0540KA_NON, BURN_H0540KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0541CA_NON:
                ModifBurnInterface(FlashTable.SN_H0541CA_NON, H0541CA_NON, BURN_H0541CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0542YA_NON:
                ModifBurnInterface(FlashTable.SN_H0542YA_NON, H0542YA_NON, BURN_H0542YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0543MA_NON:
                ModifBurnInterface(FlashTable.SN_H0543MA_NON, H0543MA_NON, BURN_H0543MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0740KA_NON:
                ModifBurnInterface(FlashTable.SN_H0740KA_NON, H0740KA_NON, BURN_H0740KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0741CA_NON:
                ModifBurnInterface(FlashTable.SN_H0741CA_NON, H0741CA_NON, BURN_H0741CA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0742YA_NON:
                ModifBurnInterface(FlashTable.SN_H0742YA_NON, H0742YA_NON, BURN_H0742YA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0743MA_NON:
                ModifBurnInterface(FlashTable.SN_H0743MA_NON, H0743MA_NON, BURN_H0743MA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_C0131U_KA:
                ModifBurnInterface(FlashTable.SN_C0131U_KA, C0131U_KA, BURN_C0131U_KA, EEDataTotal);
                break;
            case    BURPROGRAM_C0118U_C:
                ModifBurnInterface(FlashTable.SN_C0118U_C, C0118U_C, BURN_C0118U_C, EEDataTotal);
                break;
            case    BURPROGRAM_C0118U_K:
                ModifBurnInterface(FlashTable.SN_C0118U_K, C0118U_K, BURN_C0118U_K, EEDataTotal);
                break;
            case    BURPROGRAM_C0118U_M:
                ModifBurnInterface(FlashTable.SN_C0118U_M, C0118U_M, BURN_C0118U_M, EEDataTotal);
                break;
            case    BURPROGRAM_C0118U_Y:
                ModifBurnInterface(FlashTable.SN_C0118U_Y, C0118U_Y, BURN_C0118U_Y, EEDataTotal);
                break;
            case    BURPROGRAM_C0119AU:
                ModifBurnInterface(FlashTable.SN_C0119AU, C0119AU, BURN_C0119AU, EEDataTotal);
                break;
            case    BURPROGRAM_C0119XU:
                ModifBurnInterface(FlashTable.SN_C0119XU, C0119XU, BURN_C0119XU, EEDataTotal);
                break;
            case    BURPROGRAM_C0120A:
                ModifBurnInterface(FlashTable.SN_C0120A, C0120A, BURN_C0120A, EEDataTotal);
                break;
            case    BURPROGRAM_C0131U_C:
                ModifBurnInterface(FlashTable.SN_C0131U_C, C0131U_C, BURN_C0131U_C, EEDataTotal);
                break;
            case    BURPROGRAM_C0131U_KX:
                ModifBurnInterface(FlashTable.SN_C0131U_KX, C0131U_KX, BURN_C0131U_KX, EEDataTotal);
                break;
            case    BURPROGRAM_C0131U_M:
                ModifBurnInterface(FlashTable.SN_C0131U_M, C0131U_M, BURN_C0131U_M, EEDataTotal);
                break;
            case    BURPROGRAM_C0131U_Y:
                ModifBurnInterface(FlashTable.SN_C0131U_Y, C0131U_Y, BURN_C0131U_Y, EEDataTotal);
                break;
            case    BURPROGRAM_C0324A:
                ModifBurnInterface(FlashTable.SN_C0324A, C0324A, BURN_C0324A, EEDataTotal);
                break;
            case    BURPROGRAM_C0324X:
                ModifBurnInterface(FlashTable.SN_C0324X, C0324X, BURN_C0324X, EEDataTotal);
                break;
            case    BURPROGRAM_C0325A:
                ModifBurnInterface(FlashTable.SN_C0325A, C0325A, BURN_C0325A, EEDataTotal);
                break;
            case    BURPROGRAM_C0328A:
                ModifBurnInterface(FlashTable.SN_C0328A, C0328A, BURN_C0328A, EEDataTotal);
                break;
            case    BURPROGRAM_C0337:
                ModifBurnInterface(FlashTable.SN_C0337, C0337, BURN_C0337, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRC_35N:
                ModifBurnInterface(FlashTable.SN_H0TRC_35N, H0TRC_35N, BURN_H0TRC_35N, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRK_35N:
                ModifBurnInterface(FlashTable.SN_H0TRK_35N, H0TRK_35N, BURN_H0TRK_35N, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRM_35N:
                ModifBurnInterface(FlashTable.SN_H0TRM_35N, H0TRM_35N, BURN_H0TRM_35N, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRY_35N:
                ModifBurnInterface(FlashTable.SN_H0TRY_35N, H0TRY_35N, BURN_H0TRY_35N, EEDataTotal);
                break;
            case    BURPROGRAM_H0250U_KA:
                ModifBurnInterface(FlashTable.SN_H0250U_KA, H0250U_KA, BURN_H0250U_KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0250U_KX:
                ModifBurnInterface(FlashTable.SN_H0250U_KX, H0250U_KX, BURN_H0250U_KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0251U_C:
                ModifBurnInterface(FlashTable.SN_H0251U_C, H0251U_C, BURN_H0251U_C, EEDataTotal);
                break;
            case    BURPROGRAM_H0252U_Y:
                ModifBurnInterface(FlashTable.SN_H0252U_Y, H0252U_Y, BURN_H0252U_Y, EEDataTotal);
                break;
            case    BURPROGRAM_H0253U_M:
                ModifBurnInterface(FlashTable.SN_H0253U_M, H0253U_M, BURN_H0253U_M, EEDataTotal);
                break;
            case    BURPROGRAM_H0283AU:
                ModifBurnInterface(FlashTable.SN_H0283AU, H0283AU, BURN_H0283AU, EEDataTotal);
                break;
            case    BURPROGRAM_H0283XU:
                ModifBurnInterface(FlashTable.SN_H0283XU, H0283XU, BURN_H0283XU, EEDataTotal);
                break;
            case    BURPROGRAM_H0388U:
                ModifBurnInterface(FlashTable.SN_H0388U, H0388U, BURN_H0388U, EEDataTotal);
                break;
            case    BURPROGRAM_H0400U_KA:
                ModifBurnInterface(FlashTable.SN_H0400U_KA, H0400U_KA, BURN_H0400U_KA, EEDataTotal);
                break;
            case    BURPROGRAM_H0400U_KX:
                ModifBurnInterface(FlashTable.SN_H0400U_KX, H0400U_KX, BURN_H0400U_KX, EEDataTotal);
                break;
            case    BURPROGRAM_H0401U_C:
                ModifBurnInterface(FlashTable.SN_H0401U_C, H0401U_C, BURN_H0401U_C, EEDataTotal);
                break;
            case    BURPROGRAM_H0402U_Y:
                ModifBurnInterface(FlashTable.SN_H0402U_Y, H0402U_Y, BURN_H0402U_Y, EEDataTotal);
                break;
            case    BURPROGRAM_H0403U_M:
                ModifBurnInterface(FlashTable.SN_H0403U_M, H0403U_M, BURN_H0403U_M, EEDataTotal);
                break;
            case    BURPROGRAM_H0436UA:
                ModifBurnInterface(FlashTable.SN_H0436UA, H0436UA, BURN_H0436UA, EEDataTotal);
                break;
            case    BURPROGRAM_H0436UX:
                ModifBurnInterface(FlashTable.SN_H0436UX, H0436UX, BURN_H0436UX, EEDataTotal);
                break;
            case    BURPROGRAM_H0505UA:
                ModifBurnInterface(FlashTable.SN_H0505UA, H0505UA, BURN_H0505UA, EEDataTotal);
                break;
            case    BURPROGRAM_H0505UX:
                ModifBurnInterface(FlashTable.SN_H0505UX, H0505UX, BURN_H0505UX, EEDataTotal);
                break;
            case    BURPROGRAM_H0540U_K:
                ModifBurnInterface(FlashTable.SN_H0540U_K, H0540U_K, BURN_H0540U_K, EEDataTotal);
                break;
            case    BURPROGRAM_H0541U_C:
                ModifBurnInterface(FlashTable.SN_H0541U_C, H0541U_C, BURN_H0541U_C, EEDataTotal);
                break;
            case    BURPROGRAM_H0542U_Y:
                ModifBurnInterface(FlashTable.SN_H0542U_Y, H0542U_Y, BURN_H0542U_Y, EEDataTotal);
                break;
            case    BURPROGRAM_H0543U_M:
                ModifBurnInterface(FlashTable.SN_H0543U_M, H0543U_M, BURN_H0543U_M, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRC_35N_NON:
                ModifBurnInterface(FlashTable.SN_H0TRC_35N_NON, H0TRC_35N_NON, BURN_H0TRC_35N_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRK_35N_NON:
                ModifBurnInterface(FlashTable.SN_H0TRK_35N_NON, H0TRK_35N_NON, BURN_H0TRK_35N_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRM_35N_NON:
                ModifBurnInterface(FlashTable.SN_H0TRM_35N_NON, H0TRM_35N_NON, BURN_H0TRM_35N_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0TRY_35N_NON:
                ModifBurnInterface(FlashTable.SN_H0TRY_35N_NON, H0TRY_35N_NON, BURN_H0TRY_35N_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0250U_KA_NON:
                ModifBurnInterface(FlashTable.SN_H0250U_KA_NON, H0250U_KA_NON, BURN_H0250U_KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0250U_KX_NON:
                ModifBurnInterface(FlashTable.SN_H0250U_KX_NON, H0250U_KX_NON, BURN_H0250U_KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0251U_C_NON:
                ModifBurnInterface(FlashTable.SN_H0251U_C_NON, H0251U_C_NON, BURN_H0251U_C_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0252U_Y_NON:
                ModifBurnInterface(FlashTable.SN_H0252U_Y_NON, H0252U_Y_NON, BURN_H0252U_Y_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0253U_M_NON:
                ModifBurnInterface(FlashTable.SN_H0253U_M_NON, H0253U_M_NON, BURN_H0253U_M_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0283AU_NON:
                ModifBurnInterface(FlashTable.SN_H0283AU_NON, H0283AU_NON, BURN_H0283AU_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0283XU_NON:
                ModifBurnInterface(FlashTable.SN_H0283XU_NON, H0283XU_NON, BURN_H0283XU_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0388U_NON:
                ModifBurnInterface(FlashTable.SN_H0388U_NON, H0388U_NON, BURN_H0388U_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0400U_KA_NON:
                ModifBurnInterface(FlashTable.SN_H0400U_KA_NON, H0400U_KA_NON, BURN_H0400U_KA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0400U_KX_NON:
                ModifBurnInterface(FlashTable.SN_H0400U_KX_NON, H0400U_KX_NON, BURN_H0400U_KX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0401U_C_NON:
                ModifBurnInterface(FlashTable.SN_H0401U_C_NON, H0401U_C_NON, BURN_H0401U_C_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0402U_Y_NON:
                ModifBurnInterface(FlashTable.SN_H0402U_Y_NON, H0402U_Y_NON, BURN_H0402U_Y_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0403U_M_NON:
                ModifBurnInterface(FlashTable.SN_H0403U_M_NON, H0403U_M_NON, BURN_H0403U_M_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0436UA_NON:
                ModifBurnInterface(FlashTable.SN_H0436UA_NON, H0436UA_NON, BURN_H0436UA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0436UX_NON:
                ModifBurnInterface(FlashTable.SN_H0436UX_NON, H0436UX_NON, BURN_H0436UX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0505UA_NON:
                ModifBurnInterface(FlashTable.SN_H0505UA_NON, H0505UA_NON, BURN_H0505UA_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0505UX_NON:
                ModifBurnInterface(FlashTable.SN_H0505UX_NON, H0505UX_NON, BURN_H0505UX_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0540U_K_NON:
                ModifBurnInterface(FlashTable.SN_H0540U_K_NON, H0540U_K_NON, BURN_H0540U_K_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0541U_C_NON:
                ModifBurnInterface(FlashTable.SN_H0541U_C_NON, H0541U_C_NON, BURN_H0541U_C_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0542U_Y_NON:
                ModifBurnInterface(FlashTable.SN_H0542U_Y_NON, H0542U_Y_NON, BURN_H0542U_Y_NON, EEDataTotal);
                break;
            case    BURPROGRAM_H0543U_M_NON:
                ModifBurnInterface(FlashTable.SN_H0543U_M_NON, H0543U_M_NON, BURN_H0543U_M_NON, EEDataTotal);
                break;




// //             //-------------------------------  检测  ---------------------------------------------
            case    TESTCHIP_H0E320KA:
                CheckChipInterface(FlashTable.SN_H0E320KA, TEST_H0E320KA, EEDataTotal);
                break;
            case    TESTCHIP_H0E321CA:
                CheckChipInterface(FlashTable.SN_H0E321CA, TEST_H0E321CA, EEDataTotal);
                break;
            case    TESTCHIP_H0E322YA:
                CheckChipInterface(FlashTable.SN_H0E322YA, TEST_H0E322YA, EEDataTotal);
                break;
            case    TESTCHIP_H0E323MA:
                CheckChipInterface(FlashTable.SN_H0E323MA, TEST_H0E323MA, EEDataTotal);
                break;
            case    TESTCHIP_H0F320KA:
                CheckChipInterface(FlashTable.SN_H0F320KA, TEST_H0F320KA, EEDataTotal);
                break;
            case    TESTCHIP_H0F320KX:
                CheckChipInterface(FlashTable.SN_H0F320KX, TEST_H0F320KX, EEDataTotal);
                break;
            case    TESTCHIP_H0F321CA:
                CheckChipInterface(FlashTable.SN_H0F321CA, TEST_H0F321CA, EEDataTotal);
                break;
            case    TESTCHIP_H0F322YA:
                CheckChipInterface(FlashTable.SN_H0F322YA, TEST_H0F322YA, EEDataTotal);
                break;
            case    TESTCHIP_H0F323MA:
                CheckChipInterface(FlashTable.SN_H0F323MA, TEST_H0F323MA, EEDataTotal);
                break;
            case    TESTCHIP_H0031CA:
                CheckChipInterface(FlashTable.SN_H0031CA, TEST_H0031CA, EEDataTotal);
                break;
            case    TESTCHIP_H0032YA:
                CheckChipInterface(FlashTable.SN_H0032YA, TEST_H0032YA, EEDataTotal);
                break;
            case    TESTCHIP_H0033MA:
                CheckChipInterface(FlashTable.SN_H0033MA, TEST_H0033MA, EEDataTotal);
                break;
            case    TESTCHIP_H0210KA:
                CheckChipInterface(FlashTable.SN_H0210KA, TEST_H0210KA, EEDataTotal);
                break;
            case    TESTCHIP_H0210KX:
                CheckChipInterface(FlashTable.SN_H0210KX, TEST_H0210KX, EEDataTotal);
                break;
            case    TESTCHIP_H0210KXL:
                CheckChipInterface(FlashTable.SN_H0210KXL, TEST_H0210KXL, EEDataTotal);
                break;
            case    TESTCHIP_H0211CA:
                CheckChipInterface(FlashTable.SN_H0211CA, TEST_H0211CA, EEDataTotal);
                break;
            case    TESTCHIP_H0211CXL:
                CheckChipInterface(FlashTable.SN_H0211CXL, TEST_H0211CXL, EEDataTotal);
                break;
            case    TESTCHIP_H0212YA:
                CheckChipInterface(FlashTable.SN_H0212YA, TEST_H0212YA, EEDataTotal);
                break;
            case    TESTCHIP_H0212YXL:
                CheckChipInterface(FlashTable.SN_H0212YXL, TEST_H0212YXL, EEDataTotal);
                break;
            case    TESTCHIP_H0213MA:
                CheckChipInterface(FlashTable.SN_H0213MA, TEST_H0213MA, EEDataTotal);
                break;
            case    TESTCHIP_H0213MXL:
                CheckChipInterface(FlashTable.SN_H0213MXL, TEST_H0213MXL, EEDataTotal);
                break;
            case    TESTCHIP_H0250KA:
                CheckChipInterface(FlashTable.SN_H0250KA, TEST_H0250KA, EEDataTotal);
                break;
            case    TESTCHIP_H0250KX:
                CheckChipInterface(FlashTable.SN_H0250KX, TEST_H0250KX, EEDataTotal);
                break;
            case    TESTCHIP_H0251CA:
                CheckChipInterface(FlashTable.SN_H0251CA, TEST_H0251CA, EEDataTotal);
                break;
            case    TESTCHIP_H0252YA:
                CheckChipInterface(FlashTable.SN_H0252YA, TEST_H0252YA, EEDataTotal);
                break;
            case    TESTCHIP_H0253MA:
                CheckChipInterface(FlashTable.SN_H0253MA, TEST_H0253MA, EEDataTotal);
                break;
            case    TESTCHIP_H0255A:
                CheckChipInterface(FlashTable.SN_H0255A, TEST_H0255A, EEDataTotal);
                break;
            case    TESTCHIP_H0255X:
                CheckChipInterface(FlashTable.SN_H0255X, TEST_H0255X, EEDataTotal);
                break;
            case    TESTCHIP_H0260KA:
                CheckChipInterface(FlashTable.SN_H0260KA, TEST_H0260KA, EEDataTotal);
                break;
            case    TESTCHIP_H0260KX:
                CheckChipInterface(FlashTable.SN_H0260KX, TEST_H0260KX, EEDataTotal);
                break;
            case    TESTCHIP_H0261CA:
                CheckChipInterface(FlashTable.SN_H0261CA, TEST_H0261CA, EEDataTotal);
                break;
            case    TESTCHIP_H0262YA:
                CheckChipInterface(FlashTable.SN_H0262YA, TEST_H0262YA, EEDataTotal);
                break;
            case    TESTCHIP_H0263MA:
                CheckChipInterface(FlashTable.SN_H0263MA, TEST_H0263MA, EEDataTotal);
                break;
            case    TESTCHIP_H0264KX:
                CheckChipInterface(FlashTable.SN_H0264KX, TEST_H0264KX, EEDataTotal);
                break;
            case    TESTCHIP_H0270KA:
                CheckChipInterface(FlashTable.SN_H0270KA, TEST_H0270KA, EEDataTotal);
                break;
            case    TESTCHIP_H0271CA:
                CheckChipInterface(FlashTable.SN_H0271CA, TEST_H0271CA, EEDataTotal);
                break;
            case    TESTCHIP_H0272YA:
                CheckChipInterface(FlashTable.SN_H0272YA, TEST_H0272YA, EEDataTotal);
                break;
            case    TESTCHIP_H0273MA:
                CheckChipInterface(FlashTable.SN_H0273MA, TEST_H0273MA, EEDataTotal);
                break;
            case    TESTCHIP_H0278A:
                CheckChipInterface(FlashTable.SN_H0278A, TEST_H0278A, EEDataTotal);
                break;
            case    TESTCHIP_H0279:
                CheckChipInterface(FlashTable.SN_H0279, TEST_H0279, EEDataTotal);
                break;
            case    TESTCHIP_H0280A:
                CheckChipInterface(FlashTable.SN_H0280A, TEST_H0280A, EEDataTotal);
                break;
            case    TESTCHIP_H0280X:
                CheckChipInterface(FlashTable.SN_H0280X, TEST_H0280X, EEDataTotal);
                break;
            case    TESTCHIP_H0281A:
                CheckChipInterface(FlashTable.SN_H0281A, TEST_H0281A, EEDataTotal);
                break;
            case    TESTCHIP_H0281X:
                CheckChipInterface(FlashTable.SN_H0281X, TEST_H0281X, EEDataTotal);
                break;
            case    TESTCHIP_H0285A:
                CheckChipInterface(FlashTable.SN_H0285A, TEST_H0285A, EEDataTotal);
                break;
            case    TESTCHIP_H0310KA:
                CheckChipInterface(FlashTable.SN_H0310KA, TEST_H0310KA, EEDataTotal);
                break;
            case    TESTCHIP_H0311CA:
                CheckChipInterface(FlashTable.SN_H0311CA, TEST_H0311CA, EEDataTotal);
                break;
            case    TESTCHIP_H0312YA:
                CheckChipInterface(FlashTable.SN_H0312YA, TEST_H0312YA, EEDataTotal);
                break;
            case    TESTCHIP_H0313MA:
                CheckChipInterface(FlashTable.SN_H0313MA, TEST_H0313MA, EEDataTotal);
                break;
            case    TESTCHIP_H0314D:
                CheckChipInterface(FlashTable.SN_H0314D, TEST_H0314D, EEDataTotal);
                break;
            case    TESTCHIP_H0325X:
                CheckChipInterface(FlashTable.SN_H0325X, TEST_H0325X, EEDataTotal);
                break;
            case    TESTCHIP_H0330KX:
                CheckChipInterface(FlashTable.SN_H0330KX, TEST_H0330KX, EEDataTotal);
                break;
            case    TESTCHIP_H0331CA:
                CheckChipInterface(FlashTable.SN_H0331CA, TEST_H0331CA, EEDataTotal);
                break;
            case    TESTCHIP_H0332YA:
                CheckChipInterface(FlashTable.SN_H0332YA, TEST_H0332YA, EEDataTotal);
                break;
            case    TESTCHIP_H0333MA:
                CheckChipInterface(FlashTable.SN_H0333MA, TEST_H0333MA, EEDataTotal);
                break;
            case    TESTCHIP_H0340KA:
                CheckChipInterface(FlashTable.SN_H0340KA, TEST_H0340KA, EEDataTotal);
                break;
            case    TESTCHIP_H0341CA:
                CheckChipInterface(FlashTable.SN_H0341CA, TEST_H0341CA, EEDataTotal);
                break;
            case    TESTCHIP_H0342YA:
                CheckChipInterface(FlashTable.SN_H0342YA, TEST_H0342YA, EEDataTotal);
                break;
            case    TESTCHIP_H0343MA:
                CheckChipInterface(FlashTable.SN_H0343MA, TEST_H0343MA, EEDataTotal);
                break;
            case    TESTCHIP_H0350KA:
                CheckChipInterface(FlashTable.SN_H0350KA, TEST_H0350KA, EEDataTotal);
                break;
            case    TESTCHIP_H0351CA:
                CheckChipInterface(FlashTable.SN_H0351CA, TEST_H0351CA, EEDataTotal);
                break;
            case    TESTCHIP_H0352YA:
                CheckChipInterface(FlashTable.SN_H0352YA, TEST_H0352YA, EEDataTotal);
                break;
            case    TESTCHIP_H0353MA:
                CheckChipInterface(FlashTable.SN_H0353MA, TEST_H0353MA, EEDataTotal);
                break;
            case    TESTCHIP_H0364A:
                CheckChipInterface(FlashTable.SN_H0364A, TEST_H0364A, EEDataTotal);
                break;
            case    TESTCHIP_H0364X:
                CheckChipInterface(FlashTable.SN_H0364X, TEST_H0364X, EEDataTotal);
                break;
            case    TESTCHIP_H0380KA:
                CheckChipInterface(FlashTable.SN_H0380KA, TEST_H0380KA, EEDataTotal);
                break;
            case    TESTCHIP_H0380KX:
                CheckChipInterface(FlashTable.SN_H0380KX, TEST_H0380KX, EEDataTotal);
                break;
            case    TESTCHIP_H0381CA:
                CheckChipInterface(FlashTable.SN_H0381CA, TEST_H0381CA, EEDataTotal);
                break;
            case    TESTCHIP_H0382YA:
                CheckChipInterface(FlashTable.SN_H0382YA, TEST_H0382YA, EEDataTotal);
                break;
            case    TESTCHIP_H0383MA:
                CheckChipInterface(FlashTable.SN_H0383MA, TEST_H0383MA, EEDataTotal);
                break;
            case    TESTCHIP_H0388:
                CheckChipInterface(FlashTable.SN_H0388, TEST_H0388, EEDataTotal);
                break;
            case    TESTCHIP_H0390A:
                CheckChipInterface(FlashTable.SN_H0390A, TEST_H0390A, EEDataTotal);
                break;
            case    TESTCHIP_H0390X:
                CheckChipInterface(FlashTable.SN_H0390X, TEST_H0390X, EEDataTotal);
                break;
            case    TESTCHIP_H0400KA:
                CheckChipInterface(FlashTable.SN_H0400KA, TEST_H0400KA, EEDataTotal);
                break;
            case    TESTCHIP_H0400KX:
                CheckChipInterface(FlashTable.SN_H0400KX, TEST_H0400KX, EEDataTotal);
                break;
            case    TESTCHIP_H0401CA:
                CheckChipInterface(FlashTable.SN_H0401CA, TEST_H0401CA, EEDataTotal);
                break;
            case    TESTCHIP_H0402YA:
                CheckChipInterface(FlashTable.SN_H0402YA, TEST_H0402YA, EEDataTotal);
                break;
            case    TESTCHIP_H0403MA:
                CheckChipInterface(FlashTable.SN_H0403MA, TEST_H0403MA, EEDataTotal);
                break;
            case    TESTCHIP_H0410KA:
                CheckChipInterface(FlashTable.SN_H0410KA, TEST_H0410KA, EEDataTotal);
                break;
            case    TESTCHIP_H0410KX:
                CheckChipInterface(FlashTable.SN_H0410KX, TEST_H0410KX, EEDataTotal);
                break;
            case    TESTCHIP_H0410KXL:
                CheckChipInterface(FlashTable.SN_H0410KXL, TEST_H0410KXL, EEDataTotal);
                break;
            case    TESTCHIP_H0411CA:
                CheckChipInterface(FlashTable.SN_H0411CA, TEST_H0411CA, EEDataTotal);
                break;
            case    TESTCHIP_H0411CXL:
                CheckChipInterface(FlashTable.SN_H0411CXL, TEST_H0411CXL, EEDataTotal);
                break;
            case    TESTCHIP_H0412YA:
                CheckChipInterface(FlashTable.SN_H0412YA, TEST_H0412YA, EEDataTotal);
                break;
            case    TESTCHIP_H0412YXL:
                CheckChipInterface(FlashTable.SN_H0412YXL, TEST_H0412YXL, EEDataTotal);
                break;
            case    TESTCHIP_H0413MA:
                CheckChipInterface(FlashTable.SN_H0413MA, TEST_H0413MA, EEDataTotal);
                break;
            case    TESTCHIP_H0413MXL:
                CheckChipInterface(FlashTable.SN_H0413MXL, TEST_H0413MXL, EEDataTotal);
                break;
            case    TESTCHIP_H0435A:
                CheckChipInterface(FlashTable.SN_H0435A, TEST_H0435A, EEDataTotal);
                break;
            case    TESTCHIP_H0436A:
                CheckChipInterface(FlashTable.SN_H0436A, TEST_H0436A, EEDataTotal);
                break;
            case    TESTCHIP_H0505A:
                CheckChipInterface(FlashTable.SN_H0505A, TEST_H0505A, EEDataTotal);
                break;
            case    TESTCHIP_H0505X:
                CheckChipInterface(FlashTable.SN_H0505X, TEST_H0505X, EEDataTotal);
                break;
            case    TESTCHIP_H0530KA:
                CheckChipInterface(FlashTable.SN_H0530KA, TEST_H0530KA, EEDataTotal);
                break;
            case    TESTCHIP_H0530KXL:
                CheckChipInterface(FlashTable.SN_H0530KXL, TEST_H0530KXL, EEDataTotal);
                break;
            case    TESTCHIP_H0531CA:
                CheckChipInterface(FlashTable.SN_H0531CA, TEST_H0531CA, EEDataTotal);
                break;
            case    TESTCHIP_H0531CXL:
                CheckChipInterface(FlashTable.SN_H0531CXL, TEST_H0531CXL, EEDataTotal);
                break;
            case    TESTCHIP_H0532YA:
                CheckChipInterface(FlashTable.SN_H0532YA, TEST_H0532YA, EEDataTotal);
                break;
            case    TESTCHIP_H0532YXL:
                CheckChipInterface(FlashTable.SN_H0532YXL, TEST_H0532YXL, EEDataTotal);
                break;
            case    TESTCHIP_H0533MA:
                CheckChipInterface(FlashTable.SN_H0533MA, TEST_H0533MA, EEDataTotal);
                break;
            case    TESTCHIP_H0533MXL:
                CheckChipInterface(FlashTable.SN_H0533MXL, TEST_H0533MXL, EEDataTotal);
                break;
            case    TESTCHIP_H0540KA:
                CheckChipInterface(FlashTable.SN_H0540KA, TEST_H0540KA, EEDataTotal);
                break;
            case    TESTCHIP_H0541CA:
                CheckChipInterface(FlashTable.SN_H0541CA, TEST_H0541CA, EEDataTotal);
                break;
            case    TESTCHIP_H0542YA:
                CheckChipInterface(FlashTable.SN_H0542YA, TEST_H0542YA, EEDataTotal);
                break;
            case    TESTCHIP_H0543MA:
                CheckChipInterface(FlashTable.SN_H0543MA, TEST_H0543MA, EEDataTotal);
                break;
            case    TESTCHIP_H0740KA:
                CheckChipInterface(FlashTable.SN_H0740KA, TEST_H0740KA, EEDataTotal);
                break;
            case    TESTCHIP_H0741CA:
                CheckChipInterface(FlashTable.SN_H0741CA, TEST_H0741CA, EEDataTotal);
                break;
            case    TESTCHIP_H0742YA:
                CheckChipInterface(FlashTable.SN_H0742YA, TEST_H0742YA, EEDataTotal);
                break;
            case    TESTCHIP_H0743MA:
                CheckChipInterface(FlashTable.SN_H0743MA, TEST_H0743MA, EEDataTotal);
                break;
            case    TESTCHIP_H0E320KA_NON:
                CheckChipInterface(FlashTable.SN_H0E320KA_NON, TEST_H0E320KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0E321CA_NON:
                CheckChipInterface(FlashTable.SN_H0E321CA_NON, TEST_H0E321CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0E322YA_NON:
                CheckChipInterface(FlashTable.SN_H0E322YA_NON, TEST_H0E322YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0E323MA_NON:
                CheckChipInterface(FlashTable.SN_H0E323MA_NON, TEST_H0E323MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0F320KA_NON:
                CheckChipInterface(FlashTable.SN_H0F320KA_NON, TEST_H0F320KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0F320KX_NON:
                CheckChipInterface(FlashTable.SN_H0F320KX_NON, TEST_H0F320KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0F321CA_NON:
                CheckChipInterface(FlashTable.SN_H0F321CA_NON, TEST_H0F321CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0F322YA_NON:
                CheckChipInterface(FlashTable.SN_H0F322YA_NON, TEST_H0F322YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0F323MA_NON:
                CheckChipInterface(FlashTable.SN_H0F323MA_NON, TEST_H0F323MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0031CA_NON:
                CheckChipInterface(FlashTable.SN_H0031CA_NON, TEST_H0031CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0032YA_NON:
                CheckChipInterface(FlashTable.SN_H0032YA_NON, TEST_H0032YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0033MA_NON:
                CheckChipInterface(FlashTable.SN_H0033MA_NON, TEST_H0033MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0210KA_NON:
                CheckChipInterface(FlashTable.SN_H0210KA_NON, TEST_H0210KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0210KX_NON:
                CheckChipInterface(FlashTable.SN_H0210KX_NON, TEST_H0210KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0210KXL_NON:
                CheckChipInterface(FlashTable.SN_H0210KXL_NON, TEST_H0210KXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0211CA_NON:
                CheckChipInterface(FlashTable.SN_H0211CA_NON, TEST_H0211CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0211CXL_NON:
                CheckChipInterface(FlashTable.SN_H0211CXL_NON, TEST_H0211CXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0212YA_NON:
                CheckChipInterface(FlashTable.SN_H0212YA_NON, TEST_H0212YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0212YXL_NON:
                CheckChipInterface(FlashTable.SN_H0212YXL_NON, TEST_H0212YXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0213MA_NON:
                CheckChipInterface(FlashTable.SN_H0213MA_NON, TEST_H0213MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0213MXL_NON:
                CheckChipInterface(FlashTable.SN_H0213MXL_NON, TEST_H0213MXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0250KA_NON:
                CheckChipInterface(FlashTable.SN_H0250KA_NON, TEST_H0250KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0250KX_NON:
                CheckChipInterface(FlashTable.SN_H0250KX_NON, TEST_H0250KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0251CA_NON:
                CheckChipInterface(FlashTable.SN_H0251CA_NON, TEST_H0251CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0252YA_NON:
                CheckChipInterface(FlashTable.SN_H0252YA_NON, TEST_H0252YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0253MA_NON:
                CheckChipInterface(FlashTable.SN_H0253MA_NON, TEST_H0253MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0255A_NON:
                CheckChipInterface(FlashTable.SN_H0255A_NON, TEST_H0255A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0255X_NON:
                CheckChipInterface(FlashTable.SN_H0255X_NON, TEST_H0255X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0260KA_NON:
                CheckChipInterface(FlashTable.SN_H0260KA_NON, TEST_H0260KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0260KX_NON:
                CheckChipInterface(FlashTable.SN_H0260KX_NON, TEST_H0260KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0261CA_NON:
                CheckChipInterface(FlashTable.SN_H0261CA_NON, TEST_H0261CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0262YA_NON:
                CheckChipInterface(FlashTable.SN_H0262YA_NON, TEST_H0262YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0263MA_NON:
                CheckChipInterface(FlashTable.SN_H0263MA_NON, TEST_H0263MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0264KX_NON:
                CheckChipInterface(FlashTable.SN_H0264KX_NON, TEST_H0264KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0270KA_NON:
                CheckChipInterface(FlashTable.SN_H0270KA_NON, TEST_H0270KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0271CA_NON:
                CheckChipInterface(FlashTable.SN_H0271CA_NON, TEST_H0271CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0272YA_NON:
                CheckChipInterface(FlashTable.SN_H0272YA_NON, TEST_H0272YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0273MA_NON:
                CheckChipInterface(FlashTable.SN_H0273MA_NON, TEST_H0273MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0278A_NON:
                CheckChipInterface(FlashTable.SN_H0278A_NON, TEST_H0278A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0279_NON:
                CheckChipInterface(FlashTable.SN_H0279_NON, TEST_H0279_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0280A_NON:
                CheckChipInterface(FlashTable.SN_H0280A_NON, TEST_H0280A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0280X_NON:
                CheckChipInterface(FlashTable.SN_H0280X_NON, TEST_H0280X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0281A_NON:
                CheckChipInterface(FlashTable.SN_H0281A_NON, TEST_H0281A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0281X_NON:
                CheckChipInterface(FlashTable.SN_H0281X_NON, TEST_H0281X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0285A_NON:
                CheckChipInterface(FlashTable.SN_H0285A_NON, TEST_H0285A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0310KA_NON:
                CheckChipInterface(FlashTable.SN_H0310KA_NON, TEST_H0310KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0311CA_NON:
                CheckChipInterface(FlashTable.SN_H0311CA_NON, TEST_H0311CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0312YA_NON:
                CheckChipInterface(FlashTable.SN_H0312YA_NON, TEST_H0312YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0313MA_NON:
                CheckChipInterface(FlashTable.SN_H0313MA_NON, TEST_H0313MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0314D_NON:
                CheckChipInterface(FlashTable.SN_H0314D_NON, TEST_H0314D_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0325X_NON:
                CheckChipInterface(FlashTable.SN_H0325X_NON, TEST_H0325X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0330KX_NON:
                CheckChipInterface(FlashTable.SN_H0330KX_NON, TEST_H0330KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0331CA_NON:
                CheckChipInterface(FlashTable.SN_H0331CA_NON, TEST_H0331CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0332YA_NON:
                CheckChipInterface(FlashTable.SN_H0332YA_NON, TEST_H0332YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0333MA_NON:
                CheckChipInterface(FlashTable.SN_H0333MA_NON, TEST_H0333MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0340KA_NON:
                CheckChipInterface(FlashTable.SN_H0340KA_NON, TEST_H0340KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0341CA_NON:
                CheckChipInterface(FlashTable.SN_H0341CA_NON, TEST_H0341CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0342YA_NON:
                CheckChipInterface(FlashTable.SN_H0342YA_NON, TEST_H0342YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0343MA_NON:
                CheckChipInterface(FlashTable.SN_H0343MA_NON, TEST_H0343MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0350KA_NON:
                CheckChipInterface(FlashTable.SN_H0350KA_NON, TEST_H0350KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0351CA_NON:
                CheckChipInterface(FlashTable.SN_H0351CA_NON, TEST_H0351CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0352YA_NON:
                CheckChipInterface(FlashTable.SN_H0352YA_NON, TEST_H0352YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0353MA_NON:
                CheckChipInterface(FlashTable.SN_H0353MA_NON, TEST_H0353MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0364A_NON:
                CheckChipInterface(FlashTable.SN_H0364A_NON, TEST_H0364A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0364X_NON:
                CheckChipInterface(FlashTable.SN_H0364X_NON, TEST_H0364X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0380KA_NON:
                CheckChipInterface(FlashTable.SN_H0380KA_NON, TEST_H0380KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0380KX_NON:
                CheckChipInterface(FlashTable.SN_H0380KX_NON, TEST_H0380KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0381CA_NON:
                CheckChipInterface(FlashTable.SN_H0381CA_NON, TEST_H0381CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0382YA_NON:
                CheckChipInterface(FlashTable.SN_H0382YA_NON, TEST_H0382YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0383MA_NON:
                CheckChipInterface(FlashTable.SN_H0383MA_NON, TEST_H0383MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0388_NON:
                CheckChipInterface(FlashTable.SN_H0388_NON, TEST_H0388_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0390A_NON:
                CheckChipInterface(FlashTable.SN_H0390A_NON, TEST_H0390A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0390X_NON:
                CheckChipInterface(FlashTable.SN_H0390X_NON, TEST_H0390X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0400KA_NON:
                CheckChipInterface(FlashTable.SN_H0400KA_NON, TEST_H0400KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0400KX_NON:
                CheckChipInterface(FlashTable.SN_H0400KX_NON, TEST_H0400KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0401CA_NON:
                CheckChipInterface(FlashTable.SN_H0401CA_NON, TEST_H0401CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0402YA_NON:
                CheckChipInterface(FlashTable.SN_H0402YA_NON, TEST_H0402YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0403MA_NON:
                CheckChipInterface(FlashTable.SN_H0403MA_NON, TEST_H0403MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0410KA_NON:
                CheckChipInterface(FlashTable.SN_H0410KA_NON, TEST_H0410KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0410KX_NON:
                CheckChipInterface(FlashTable.SN_H0410KX_NON, TEST_H0410KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0410KXL_NON:
                CheckChipInterface(FlashTable.SN_H0410KXL_NON, TEST_H0410KXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0411CA_NON:
                CheckChipInterface(FlashTable.SN_H0411CA_NON, TEST_H0411CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0411CXL_NON:
                CheckChipInterface(FlashTable.SN_H0411CXL_NON, TEST_H0411CXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0412YA_NON:
                CheckChipInterface(FlashTable.SN_H0412YA_NON, TEST_H0412YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0412YXL_NON:
                CheckChipInterface(FlashTable.SN_H0412YXL_NON, TEST_H0412YXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0413MA_NON:
                CheckChipInterface(FlashTable.SN_H0413MA_NON, TEST_H0413MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0413MXL_NON:
                CheckChipInterface(FlashTable.SN_H0413MXL_NON, TEST_H0413MXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0435A_NON:
                CheckChipInterface(FlashTable.SN_H0435A_NON, TEST_H0435A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0436A_NON:
                CheckChipInterface(FlashTable.SN_H0436A_NON, TEST_H0436A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0505A_NON:
                CheckChipInterface(FlashTable.SN_H0505A_NON, TEST_H0505A_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0505X_NON:
                CheckChipInterface(FlashTable.SN_H0505X_NON, TEST_H0505X_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0530KA_NON:
                CheckChipInterface(FlashTable.SN_H0530KA_NON, TEST_H0530KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0530KXL_NON:
                CheckChipInterface(FlashTable.SN_H0530KXL_NON, TEST_H0530KXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0531CA_NON:
                CheckChipInterface(FlashTable.SN_H0531CA_NON, TEST_H0531CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0531CXL_NON:
                CheckChipInterface(FlashTable.SN_H0531CXL_NON, TEST_H0531CXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0532YA_NON:
                CheckChipInterface(FlashTable.SN_H0532YA_NON, TEST_H0532YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0532YXL_NON:
                CheckChipInterface(FlashTable.SN_H0532YXL_NON, TEST_H0532YXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0533MA_NON:
                CheckChipInterface(FlashTable.SN_H0533MA_NON, TEST_H0533MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0533MXL_NON:
                CheckChipInterface(FlashTable.SN_H0533MXL_NON, TEST_H0533MXL_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0540KA_NON:
                CheckChipInterface(FlashTable.SN_H0540KA_NON, TEST_H0540KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0541CA_NON:
                CheckChipInterface(FlashTable.SN_H0541CA_NON, TEST_H0541CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0542YA_NON:
                CheckChipInterface(FlashTable.SN_H0542YA_NON, TEST_H0542YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0543MA_NON:
                CheckChipInterface(FlashTable.SN_H0543MA_NON, TEST_H0543MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0740KA_NON:
                CheckChipInterface(FlashTable.SN_H0740KA_NON, TEST_H0740KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0741CA_NON:
                CheckChipInterface(FlashTable.SN_H0741CA_NON, TEST_H0741CA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0742YA_NON:
                CheckChipInterface(FlashTable.SN_H0742YA_NON, TEST_H0742YA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0743MA_NON:
                CheckChipInterface(FlashTable.SN_H0743MA_NON, TEST_H0743MA_NON, EEDataTotal);
                break;
            case    TESTCHIP_C0131U_KA:
                CheckChipInterface(FlashTable.SN_C0131U_KA, TEST_C0131U_KA, EEDataTotal);
                break;
            case    TESTCHIP_C0118U_C:
                CheckChipInterface(FlashTable.SN_C0118U_C, TEST_C0118U_C, EEDataTotal);
                break;
            case    TESTCHIP_C0118U_K:
                CheckChipInterface(FlashTable.SN_C0118U_K, TEST_C0118U_K, EEDataTotal);
                break;
            case    TESTCHIP_C0118U_M:
                CheckChipInterface(FlashTable.SN_C0118U_M, TEST_C0118U_M, EEDataTotal);
                break;
            case    TESTCHIP_C0118U_Y:
                CheckChipInterface(FlashTable.SN_C0118U_Y, TEST_C0118U_Y, EEDataTotal);
                break;
            case    TESTCHIP_C0119AU:
                CheckChipInterface(FlashTable.SN_C0119AU, TEST_C0119AU, EEDataTotal);
                break;
            case    TESTCHIP_C0119XU:
                CheckChipInterface(FlashTable.SN_C0119XU, TEST_C0119XU, EEDataTotal);
                break;
            case    TESTCHIP_C0120A:
                CheckChipInterface(FlashTable.SN_C0120A, TEST_C0120A, EEDataTotal);
                break;
            case    TESTCHIP_C0131U_C:
                CheckChipInterface(FlashTable.SN_C0131U_C, TEST_C0131U_C, EEDataTotal);
                break;
            case    TESTCHIP_C0131U_KX:
                CheckChipInterface(FlashTable.SN_C0131U_KX, TEST_C0131U_KX, EEDataTotal);
                break;
            case    TESTCHIP_C0131U_M:
                CheckChipInterface(FlashTable.SN_C0131U_M, TEST_C0131U_M, EEDataTotal);
                break;
            case    TESTCHIP_C0131U_Y:
                CheckChipInterface(FlashTable.SN_C0131U_Y, TEST_C0131U_Y, EEDataTotal);
                break;
            case    TESTCHIP_C0324A:
                CheckChipInterface(FlashTable.SN_C0324A, TEST_C0324A, EEDataTotal);
                break;
            case    TESTCHIP_C0324X:
                CheckChipInterface(FlashTable.SN_C0324X, TEST_C0324X, EEDataTotal);
                break;
            case    TESTCHIP_C0325A:
                CheckChipInterface(FlashTable.SN_C0325A, TEST_C0325A, EEDataTotal);
                break;
            case    TESTCHIP_C0328A:
                CheckChipInterface(FlashTable.SN_C0328A, TEST_C0328A, EEDataTotal);
                break;
            case    TESTCHIP_C0337:
                CheckChipInterface(FlashTable.SN_C0337, TEST_C0337, EEDataTotal);
                break;
            case    TESTCHIP_H0TRC_35N:
                CheckChipInterface(FlashTable.SN_H0TRC_35N, TEST_H0TRC_35N, EEDataTotal);
                break;
            case    TESTCHIP_H0TRK_35N:
                CheckChipInterface(FlashTable.SN_H0TRK_35N, TEST_H0TRK_35N, EEDataTotal);
                break;
            case    TESTCHIP_H0TRM_35N:
                CheckChipInterface(FlashTable.SN_H0TRM_35N, TEST_H0TRM_35N, EEDataTotal);
                break;
            case    TESTCHIP_H0TRY_35N:
                CheckChipInterface(FlashTable.SN_H0TRY_35N, TEST_H0TRY_35N, EEDataTotal);
                break;
            case    TESTCHIP_H0250U_KA:
                CheckChipInterface(FlashTable.SN_H0250U_KA, TEST_H0250U_KA, EEDataTotal);
                break;
            case    TESTCHIP_H0250U_KX:
                CheckChipInterface(FlashTable.SN_H0250U_KX, TEST_H0250U_KX, EEDataTotal);
                break;
            case    TESTCHIP_H0251U_C:
                CheckChipInterface(FlashTable.SN_H0251U_C, TEST_H0251U_C, EEDataTotal);
                break;
            case    TESTCHIP_H0252U_Y:
                CheckChipInterface(FlashTable.SN_H0252U_Y, TEST_H0252U_Y, EEDataTotal);
                break;
            case    TESTCHIP_H0253U_M:
                CheckChipInterface(FlashTable.SN_H0253U_M, TEST_H0253U_M, EEDataTotal);
                break;
            case    TESTCHIP_H0283AU:
                CheckChipInterface(FlashTable.SN_H0283AU, TEST_H0283AU, EEDataTotal);
                break;
            case    TESTCHIP_H0283XU:
                CheckChipInterface(FlashTable.SN_H0283XU, TEST_H0283XU, EEDataTotal);
                break;
            case    TESTCHIP_H0388U:
                CheckChipInterface(FlashTable.SN_H0388U, TEST_H0388U, EEDataTotal);
                break;
            case    TESTCHIP_H0400U_KA:
                CheckChipInterface(FlashTable.SN_H0400U_KA, TEST_H0400U_KA, EEDataTotal);
                break;
            case    TESTCHIP_H0400U_KX:
                CheckChipInterface(FlashTable.SN_H0400U_KX, TEST_H0400U_KX, EEDataTotal);
                break;
            case    TESTCHIP_H0401U_C:
                CheckChipInterface(FlashTable.SN_H0401U_C, TEST_H0401U_C, EEDataTotal);
                break;
            case    TESTCHIP_H0402U_Y:
                CheckChipInterface(FlashTable.SN_H0402U_Y, TEST_H0402U_Y, EEDataTotal);
                break;
            case    TESTCHIP_H0403U_M:
                CheckChipInterface(FlashTable.SN_H0403U_M, TEST_H0403U_M, EEDataTotal);
                break;
            case    TESTCHIP_H0436UA:
                CheckChipInterface(FlashTable.SN_H0436UA, TEST_H0436UA, EEDataTotal);
                break;
            case    TESTCHIP_H0436UX:
                CheckChipInterface(FlashTable.SN_H0436UX, TEST_H0436UX, EEDataTotal);
                break;
            case    TESTCHIP_H0505UA:
                CheckChipInterface(FlashTable.SN_H0505UA, TEST_H0505UA, EEDataTotal);
                break;
            case    TESTCHIP_H0505UX:
                CheckChipInterface(FlashTable.SN_H0505UX, TEST_H0505UX, EEDataTotal);
                break;
            case    TESTCHIP_H0540U_K:
                CheckChipInterface(FlashTable.SN_H0540U_K, TEST_H0540U_K, EEDataTotal);
                break;
            case    TESTCHIP_H0541U_C:
                CheckChipInterface(FlashTable.SN_H0541U_C, TEST_H0541U_C, EEDataTotal);
                break;
            case    TESTCHIP_H0542U_Y:
                CheckChipInterface(FlashTable.SN_H0542U_Y, TEST_H0542U_Y, EEDataTotal);
                break;
            case    TESTCHIP_H0543U_M:
                CheckChipInterface(FlashTable.SN_H0543U_M, TEST_H0543U_M, EEDataTotal);
                break;
            case    TESTCHIP_H0TRC_35N_NON:
                CheckChipInterface(FlashTable.SN_H0TRC_35N_NON, TEST_H0TRC_35N_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0TRK_35N_NON:
                CheckChipInterface(FlashTable.SN_H0TRK_35N_NON, TEST_H0TRK_35N_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0TRM_35N_NON:
                CheckChipInterface(FlashTable.SN_H0TRM_35N_NON, TEST_H0TRM_35N_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0TRY_35N_NON:
                CheckChipInterface(FlashTable.SN_H0TRY_35N_NON, TEST_H0TRY_35N_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0250U_KA_NON:
                CheckChipInterface(FlashTable.SN_H0250U_KA_NON, TEST_H0250U_KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0250U_KX_NON:
                CheckChipInterface(FlashTable.SN_H0250U_KX_NON, TEST_H0250U_KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0251U_C_NON:
                CheckChipInterface(FlashTable.SN_H0251U_C_NON, TEST_H0251U_C_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0252U_Y_NON:
                CheckChipInterface(FlashTable.SN_H0252U_Y_NON, TEST_H0252U_Y_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0253U_M_NON:
                CheckChipInterface(FlashTable.SN_H0253U_M_NON, TEST_H0253U_M_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0283AU_NON:
                CheckChipInterface(FlashTable.SN_H0283AU_NON, TEST_H0283AU_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0283XU_NON:
                CheckChipInterface(FlashTable.SN_H0283XU_NON, TEST_H0283XU_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0388U_NON:
                CheckChipInterface(FlashTable.SN_H0388U_NON, TEST_H0388U_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0400U_KA_NON:
                CheckChipInterface(FlashTable.SN_H0400U_KA_NON, TEST_H0400U_KA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0400U_KX_NON:
                CheckChipInterface(FlashTable.SN_H0400U_KX_NON, TEST_H0400U_KX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0401U_C_NON:
                CheckChipInterface(FlashTable.SN_H0401U_C_NON, TEST_H0401U_C_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0402U_Y_NON:
                CheckChipInterface(FlashTable.SN_H0402U_Y_NON, TEST_H0402U_Y_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0403U_M_NON:
                CheckChipInterface(FlashTable.SN_H0403U_M_NON, TEST_H0403U_M_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0436UA_NON:
                CheckChipInterface(FlashTable.SN_H0436UA_NON, TEST_H0436UA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0436UX_NON:
                CheckChipInterface(FlashTable.SN_H0436UX_NON, TEST_H0436UX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0505UA_NON:
                CheckChipInterface(FlashTable.SN_H0505UA_NON, TEST_H0505UA_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0505UX_NON:
                CheckChipInterface(FlashTable.SN_H0505UX_NON, TEST_H0505UX_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0540U_K_NON:
                CheckChipInterface(FlashTable.SN_H0540U_K_NON, TEST_H0540U_K_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0541U_C_NON:
                CheckChipInterface(FlashTable.SN_H0541U_C_NON, TEST_H0541U_C_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0542U_Y_NON:
                CheckChipInterface(FlashTable.SN_H0542U_Y_NON, TEST_H0542U_Y_NON, EEDataTotal);
                break;
            case    TESTCHIP_H0543U_M_NON:
                CheckChipInterface(FlashTable.SN_H0543U_M_NON, TEST_H0543U_M_NON, EEDataTotal);
                break;

            }
        }
        else if (KeyPressed == Key_Esc)          /* 退出底层用户界面，然后返回显示目录 */
        {
            DisplayStart = UserChoose;
            DisplayEnd = UserChoose + 6;
            if (DisplayEnd > MaxItems)
            {
                DisplayEnd = MaxItems;
                if (MaxItems >= 6)
                {
                    DisplayStart = MaxItems - 6;
                }
                else
                {
                    DisplayStart = 0;
                }
            }
            break;
        }
    }
}
