#ifndef __APP_H__
#define __APP_H__

#include "hw.h"
#include "boot_common.h"

void app_init(void);
void app_main(void);


bool check_boot_mode(void);

#endif /* __APP_H__ */
