#ifndef  CHECK_FUNCTION_H
#define  CHECK_FUNCTION_H

#include "menu.h"
#include "w25qxx.h"
#include "user_OS.h"
#include "user_Peripheral.h"

u16 GetSNDataLen(void);

void CheckAcl16( u32 command_code);
void CheckChipInterface( u32 command_code);
#endif
