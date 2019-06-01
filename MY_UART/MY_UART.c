#include "MY_UART.h"
#include "sys.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"


extern struct buffer COMM_BUF_UART1;	
int data_length;  //�յ����ݵĳ��ȣ���ȥ0d0a
int Buffer(struct buffer *buf)
 {
	 if(buf->NewData==1)
		 return 1;
	 else if(buf->NewData==0)
		 return 0;
 }

 void INT_USART1(u8 res)
{
	if (res==0)   //����ascii��0
	{
		// Ϊ����ֱ�ӷ���
		return;
	}
	else
	{
		Buffer_Write(&COMM_BUF_UART1,res);
		if (res==10)//LF  OA
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�
		}
	}
	/*  Buffer_Write(&COMM_BUF_UART1,res);     //�����㣬ƥ��FPGA������32λ��������
		if (res==10)//LF
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�
		}*/
}

void Buffer_Write(struct buffer *buf,u8 c) //���ַ�cд������
{
	if (buf->ptr < LENGTH)//�ж��Ƿ�С�������С
	{
		buf->buffer[buf->ptr]=c;
		data_length = buf->ptr - 1; //��ȥ0d0a��ʵ�ʵĳ���
		buf->ptr++;
		
	}
	if (buf->ptr >= LENGTH)
	{
		buf->NewData=TRUE;
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رմ��ڽ����ж�
	}
	return;
}

void buffer_Clear(struct buffer *buf){
int i;
	for (i=0;i<LENGTH;i++){
		buf->buffer[i]=0;
	}
	buf->ptr=0;	buf->NewData=FALSE;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	return;
}

void COMM_PROC(struct buffer *BUF)   //���ݴ���
{
	
u8 c;
u8 subcmd;
u32 result; //FPGA���͵�32λ��������
	c=BUF->buffer[0];   //֡ͷ
	subcmd=BUF->buffer[1]-'0';//����λ
	/*result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);
	if(result)
	  LCD_ShowNum(210,100,result,10,16);   //��ʾ�յ������*/
	if (c=='P')
	{
		switch(subcmd)
		{
			case 1:	COMM_PC_P01(BUF);  //�յ���Ƶ��
		 //LCD_ShowNum(50,50,1,5,24);
			break;
			case 2:	COMM_PC_P02(BUF);  //�յ��ķ���
			default: ;
		}
	}
}
void COMM_PC_P01(struct buffer *BUF)  //�յ���Ƶ��
{
	int i;
	double data_freq = 0;//�յ���ʮ��������
	u8 t[20];//Ƶ��
  for(i = 2;i < data_length;i++)   //���յ�������תΪʮ����
	{
			data_freq += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	//LCD_ShowNum(100,100,data,10,16);   //��ʾ�յ�������
	sprintf((char*)t, "%.4f MHz  ",data_freq*1.0/1000000);  //����ת�ַ����������С���㣡
	LCD_ShowString(100,100,260,16,16,t); //��ʾƵ��
	//LCD_ShowNum(100,120,data_length,10,16);//��ʾ�յ������ݴ�С*/
	//LCD_ShowString(210,100,200,20,16,BUF->buffer); //��ʾ�յ������ݷ���
	//LCD_ShowNum(210,120,data_length,10,16);//��ʾ�յ������ݴ�С
	/*u32 result;
	u8 t[20];//����
	result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);  //��32λ��������ת��Ϊ10������
	if(result)
	{
		LCD_ShowString(210,80,200,20,16,"Receive_1!"); //��ʾ�յ������ݷ���
		if(result < 1000)
		{
			sprintf((char*)t, "%.4f Hz",result*1.0);  //����ת�ַ����������С���㣡
		  LCD_ShowString(110,90,260,12,12,t); //��ʾƵ��
			
			sprintf((char*)t, "%.4f ms", 1.0/result*1000);  //����ת�ַ����������С���㣡
		  LCD_ShowString(10,90,260,12,12,t); //��ʾ����
		}
	   // LCD_ShowString(180,120,20,20,16,"Hz"); //��ʾ��λ
		else if(result >= 1000 && result < 1000000)
		{
			sprintf((char*)t, "%.4f KHz",result*1.0/1000);  //����ת�ַ����������С���㣡
		  LCD_ShowString(110,90,260,12,12,t); //��ʾƵ��
			
			sprintf((char*)t, "%.4f us", 1.0/result*1000000);  //����ת�ַ����������С���㣡
	  	LCD_ShowString(10,90,260,12,12,t); //��ʾ����
		}
			else if(result >= 1000000)
		{
			sprintf((char*)t, "%.4f MHz",result*1.0/1000000);  //����ת�ַ����������С���㣡
		  LCD_ShowString(110,90,260,12,12,t); //��ʾƵ��
			
			sprintf((char*)t, "%.4f ns", 1.0/result*1000000000);  //����ת�ַ����������С���㣡
		 LCD_ShowString(10,90,260,12,12,t); //��ʾ����
		}
		
		
		//sprintf((char*)t, "%.8f", 1.0/result);  //����ת�ַ����������С���㣡
		//LCD_ShowString(10,90,260,12,16,t); //��ʾ����
		//LCD_ShowString(80,120,20,20,16,"S"); //��ʾ��λ
		
		*/
		
	
	
}
 
void COMM_PC_P02(struct buffer *BUF)  //�յ��ķ���
{
	int i;
	u8 t[20];//����
	double data_vpp = 0;//�յ���ʮ��������
  for(i = 2;i < data_length;i++)   //���յ�������תΪʮ����
	{
			data_vpp += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	sprintf((char*)t, "%.2f v  ",data_vpp*1.0/1000);  //����ת�ַ����������С���㣡  ��λ��ӿո񣬿ɽ���ַ��������⣨���º��޷���ȫ������ǰ�������⣩
	LCD_ShowString(100,200,260,16,16,t); //��ʾƵ��
}

