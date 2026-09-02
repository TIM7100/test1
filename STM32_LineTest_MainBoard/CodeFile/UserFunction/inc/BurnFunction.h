#ifndef  BURN_FUNCTION_H
#define  BURN_FUNCTION_H

#include "menu.h"
#include "w25qxx.h"

#include "user_OS.h"
#include "user_Peripheral.h"


void BurnInterface( u16 type_code, u32 command_code, u8 AreaCode);

void BurnAcl16(u16 type_code, u32 command_code, u8 AreaCode);


#endif
