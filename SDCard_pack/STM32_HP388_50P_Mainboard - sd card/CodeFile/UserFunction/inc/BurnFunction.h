#ifndef  BURN_FUNCTION_H
#define  BURN_FUNCTION_H

#include "menu.h"
#include "SD.h"

#include "user_OS.h"
#include "user_Peripheral.h"


void ModifBurnInterface(TypeInfrom Type_sel, u16 type_code, u32 command_code, u8 EEDataSum);

void ModifBurnAcl16(TypeInfrom Type_sel, u16 type_code, u32 command_code, u8 EEDataSum);

#endif
