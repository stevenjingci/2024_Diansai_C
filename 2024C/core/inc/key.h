#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#define KEY1 1   //add
#define KEY0 2   //subtract

void KEY_Init(void);
uint8_t key_scan(void);
void key_proc(void);
#endif
