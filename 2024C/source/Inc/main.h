#ifndef __MAIN_H
#define __MAIN_H

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

/* DriverLib Includes */
#include "stm32f4xx_hal.h"

//basic
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "math.h"

//Core
#include "sys.h"
#include "key.h"
#include "Delay.h"
#include "uart.h"
#include "timer.h"
#include "dac.h"

//Hardware
#include "AD9910.h"
#include "AD9854.h"
#include "AD9959.h"
#include "TLV5618.h"
#include "screen.h"


void signal_setting(void);

#endif 


