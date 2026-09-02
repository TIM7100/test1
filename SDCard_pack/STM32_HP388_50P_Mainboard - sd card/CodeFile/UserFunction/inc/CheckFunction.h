#ifndef  CHECK_FUNCTION_H
#define  CHECK_FUNCTION_H

#include "menu.h"
#include "SD.h"
#include "user_OS.h"
#include "user_Peripheral.h"

void CheckChipInterface(TypeInfrom Type_sel, u32 command_code, u8 EEDataSum);

void ModifCheckAcl16(TypeInfrom Type_sel, u32 command_code, u8 EEDataSum);

#endif
