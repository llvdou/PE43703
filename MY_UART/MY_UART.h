#ifndef _MY_UART_H
#define _MY_UART_H
#include "sys.h"
#define LENGTH 255 //���ݰ�����
#define TRUE 1
#define FALSE 0	  
struct buffer{
	u8	buffer[LENGTH];//����
	u16	ptr;//ָ��
	u8	NewData;//�������ݱ�־
};  
struct buffer COMM_BUF_UART1;	
void INT_USART1(u8 res);//���ݽ���
void buffer_Clear(struct buffer *buf);//��ջ���
void Buffer_Write(struct buffer *buf,u8 c); //����д��
void COMM_PROC(struct buffer *BUF); //���ݴ���
void COMM_PC_P01(struct buffer *BUF);  //����1
void COMM_PC_P02(struct buffer *BUF);//����2
void COMM_PC_P03(struct buffer *BUF);  //����1
void COMM_PC_P04(struct buffer *BUF);//����2
#endif
