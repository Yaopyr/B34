#ifndef __HW_REMOTE_H
#define __HW_REMOTE_H

#include "stm32f10x.h"

// 类型重定义（兼容不同编译器）
typedef uint8_t  u8;
typedef uint32_t u32;

// 函数声明
void Drv_Hw_Init(void);
u8 Med_Hw_ReadKeyValue(void);

// 全局变量声明（volatile修饰）
extern volatile u32 hw_jsm;
extern volatile u8 hw_jsbz;

// 按键值宏定义（提高可读性）
#define KEY_UP      13
#define KEY_DOWN    14
#define KEY_LEFT    15
#define KEY_RIGHT   16
// ...其他按键宏

#endif
