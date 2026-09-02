#ifndef __SHADOWE_H
#define __SHADOWE_H

#include "fxx_std.h"

typedef struct _UPDATA_COUNT_DATA_
{
    u32 Lock;
    u32 Version;
    u32 HP95_90Count;
    u32 HP97Count;
}UpdataCount_t;    

enum{
    SHADOW_SUCCESS,
    SHADOW_GET_FAIL,
    SHADOW_VERSION_ALIKE,
    SHADOW_VERSION_ERR,
};

void UpdataCountParameterInit(void);
u8 GetShadowInfo(void);
void GetUpdataCount(u32* HP95_90Count, u32* HP97Count);
void ThingUploadUpdataCount(void);
#endif
