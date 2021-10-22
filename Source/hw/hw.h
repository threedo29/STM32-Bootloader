#ifndef __HW_H__
#define __HW_H__

#include "hw_common.h"

/* Module Include */
#include "delay.h"
#include "watchdog.h"
#include "timer.h"
#include "uart.h"

void hw_init(void);
static void board_init(void);

#endif /* __HW_H__ */
