#ifndef __PE43703_H__
#define __PE43703_H__
#include "sys.h"
#include "myiic.h"

#define H_LE GPIO_SetBits(GPIOC,GPIO_Pin_5);  //LE
#define L_LE GPIO_ResetBits(GPIOC,GPIO_Pin_5);

#define H_A0 GPIO_SetBits(GPIOC,GPIO_Pin_0);  //A0
#define L_A0 GPIO_ResetBits(GPIOC,GPIO_Pin_0);
#define H_A1 GPIO_SetBits(GPIOC,GPIO_Pin_1);//A1
#define L_A1 GPIO_ResetBits(GPIOC,GPIO_Pin_1);
#define H_A2 GPIO_SetBits(GPIOC,GPIO_Pin_2);//A2
#define L_A2 GPIO_ResetBits(GPIOC,GPIO_Pin_2);
void PE430703_Init(void);
void PE43703_Send(u16 data);  //data为16位  八位地址＋八位数据

#endif
