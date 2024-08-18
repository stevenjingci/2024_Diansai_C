#ifndef __DAC_H
#define __DAC_H

#include "main.h"

extern DAC_HandleTypeDef hdac;


void MX_DAC_Init(void);
void dac_value_set(void);

#endif