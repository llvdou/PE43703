#include "MY_UART.h"
#include "sys.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"


extern struct buffer COMM_BUF_UART1;	
int data_length;  //收到数据的长度，减去0d0a
int Buffer(struct buffer *buf)
 {
	 if(buf->NewData==1)
		 return 1;
	 else if(buf->NewData==0)
		 return 0;
 }

 void INT_USART1(u8 res)
{
	if (res==0)   //无视ascii码0
	{
		// 为空则直接返回
		return;
	}
	else
	{
		Buffer_Write(&COMM_BUF_UART1,res);
		if (res==10)//LF  OA
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭串口接受中断
		}
	}
	/*  Buffer_Write(&COMM_BUF_UART1,res);     //保留零，匹配FPGA发来的32位二进制数
		if (res==10)//LF
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭串口接受中断
		}*/
}

void Buffer_Write(struct buffer *buf,u8 c) //将字符c写入数组
{
	if (buf->ptr < LENGTH)//判断是否小于数组大小
	{
		buf->buffer[buf->ptr]=c;
		data_length = buf->ptr - 1; //减去0d0a后实际的长度
		buf->ptr++;
		
	}
	if (buf->ptr >= LENGTH)
	{
		buf->NewData=TRUE;
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭串口接受中断
	}
	return;
}

void buffer_Clear(struct buffer *buf){
int i;
	for (i=0;i<LENGTH;i++){
		buf->buffer[i]=0;
	}
	buf->ptr=0;	buf->NewData=FALSE;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	return;
}

void COMM_PROC(struct buffer *BUF)   //数据处理
{
	
u8 c;
u8 subcmd;
u32 result; //FPGA发送的32位二进制数
	c=BUF->buffer[0];   //帧头
	subcmd=BUF->buffer[1]-'0';//功能位
	/*result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);
	if(result)
	  LCD_ShowNum(210,100,result,10,16);   //显示收到的数�*/
	if (c=='P')
	{
		switch(subcmd)
		{
			case 1:	COMM_PC_P01(BUF);  //收到的频率
		 //LCD_ShowNum(50,50,1,5,24);
			break;
			case 2:	COMM_PC_P02(BUF);  //收到的幅度
			default: ;
		}
	}
}
void COMM_PC_P01(struct buffer *BUF)  //收到的频率
{
	int i;
	double data_freq = 0;//收到的十进制数据
	u8 t[20];//频率
  for(i = 2;i < data_length;i++)   //将收到的数据转为十进制
	{
			data_freq += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	//LCD_ShowNum(100,100,data,10,16);   //显示收到的数据
	sprintf((char*)t, "%.4f MHz  ",data_freq*1.0/1000000);  //数字转字符串，可输出小数点！
	LCD_ShowString(100,100,260,16,16,t); //显示频率
	//LCD_ShowNum(100,120,data_length,10,16);//显示收到的数据大小*/
	//LCD_ShowString(210,100,200,20,16,BUF->buffer); //显示收到的数据分类
	//LCD_ShowNum(210,120,data_length,10,16);//显示收到的数据大小
	/*u32 result;
	u8 t[20];//周期
	result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);  //将32位二进制数转化为10进制数
	if(result)
	{
		LCD_ShowString(210,80,200,20,16,"Receive_1!"); //显示收到的数据分类
		if(result < 1000)
		{
			sprintf((char*)t, "%.4f Hz",result*1.0);  //数字转字符串，可输出小数点！
		  LCD_ShowString(110,90,260,12,12,t); //显示频率
			
			sprintf((char*)t, "%.4f ms", 1.0/result*1000);  //数字转字符串，可输出小数点！
		  LCD_ShowString(10,90,260,12,12,t); //显示周期
		}
	   // LCD_ShowString(180,120,20,20,16,"Hz"); //显示单位
		else if(result >= 1000 && result < 1000000)
		{
			sprintf((char*)t, "%.4f KHz",result*1.0/1000);  //数字转字符串，可输出小数点！
		  LCD_ShowString(110,90,260,12,12,t); //显示频率
			
			sprintf((char*)t, "%.4f us", 1.0/result*1000000);  //数字转字符串，可输出小数点！
	  	LCD_ShowString(10,90,260,12,12,t); //显示周期
		}
			else if(result >= 1000000)
		{
			sprintf((char*)t, "%.4f MHz",result*1.0/1000000);  //数字转字符串，可输出小数点！
		  LCD_ShowString(110,90,260,12,12,t); //显示频率
			
			sprintf((char*)t, "%.4f ns", 1.0/result*1000000000);  //数字转字符串，可输出小数点！
		 LCD_ShowString(10,90,260,12,12,t); //显示周期
		}
		
		
		//sprintf((char*)t, "%.8f", 1.0/result);  //数字转字符串，可输出小数点！
		//LCD_ShowString(10,90,260,12,16,t); //显示周期
		//LCD_ShowString(80,120,20,20,16,"S"); //显示单位
		
		*/
		
	
	
}
 
void COMM_PC_P02(struct buffer *BUF)  //收到的幅度
{
	int i;
	u8 t[20];//幅度
	double data_vpp = 0;//收到的十进制数据
  for(i = 2;i < data_length;i++)   //将收到的数据转为十进制
	{
			data_vpp += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	sprintf((char*)t, "%.2f v  ",data_vpp*1.0/1000);  //数字转字符串，可输出小数点！  单位后加空格，可解决字符乱序问题（更新后无法完全覆盖以前的数问题）
	LCD_ShowString(100,200,260,16,16,t); //显示频率
}

