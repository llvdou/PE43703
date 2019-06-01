#ifndef _MY_UART_H
#define _MY_UART_H
#include "sys.h"
#define LENGTH 255 //数据包长度
#define TRUE 1
#define FALSE 0	  
struct buffer{
	u8	buffer[LENGTH];//数组
	u16	ptr;//指针
	u8	NewData;//完整数据标志
};  
struct buffer COMM_BUF_UART1;	
void INT_USART1(u8 res);//数据接收
void buffer_Clear(struct buffer *buf);//清空缓存
void Buffer_Write(struct buffer *buf,u8 c); //数据写入
void COMM_PROC(struct buffer *BUF); //数据处理
void COMM_PC_P01(struct buffer *BUF);  //进程1
void COMM_PC_P02(struct buffer *BUF);//进程2
void COMM_PC_P03(struct buffer *BUF);  //进程1
void COMM_PC_P04(struct buffer *BUF);//进程2
#endif
