#include "MY_UART.h"
#include "sys.h"
#include "lcd.h"
#include "math.h"
#include "stdio.h"


extern struct buffer COMM_BUF_UART1;	
int data_length;  //ÊÕµ½Êı¾İµÄ³¤¶È£¬¼õÈ¥0d0a
int Buffer(struct buffer *buf)
 {
	 if(buf->NewData==1)
		 return 1;
	 else if(buf->NewData==0)
		 return 0;
 }

 void INT_USART1(u8 res)
{
	if (res==0)   //ÎŞÊÓasciiÂë0
	{
		// Îª¿ÕÔòÖ±½Ó·µ»Ø
		return;
	}
	else
	{
		Buffer_Write(&COMM_BUF_UART1,res);
		if (res==10)//LF  OA
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//¹Ø±Õ´®¿Ú½ÓÊÜÖĞ¶Ï
		}
	}
	/*  Buffer_Write(&COMM_BUF_UART1,res);     //±£ÁôÁã£¬Æ¥ÅäFPGA·¢À´µÄ32Î»¶ş½øÖÆÊı
		if (res==10)//LF
		{
			COMM_BUF_UART1.NewData=TRUE;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//¹Ø±Õ´®¿Ú½ÓÊÜÖĞ¶Ï
		}*/
}

void Buffer_Write(struct buffer *buf,u8 c) //½«×Ö·ûcĞ´ÈëÊı×é
{
	if (buf->ptr < LENGTH)//ÅĞ¶ÏÊÇ·ñĞ¡ÓÚÊı×é´óĞ¡
	{
		buf->buffer[buf->ptr]=c;
		data_length = buf->ptr - 1; //¼õÈ¥0d0aºóÊµ¼ÊµÄ³¤¶È
		buf->ptr++;
		
	}
	if (buf->ptr >= LENGTH)
	{
		buf->NewData=TRUE;
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//¹Ø±Õ´®¿Ú½ÓÊÜÖĞ¶Ï
	}
	return;
}

void buffer_Clear(struct buffer *buf){
int i;
	for (i=0;i<LENGTH;i++){
		buf->buffer[i]=0;
	}
	buf->ptr=0;	buf->NewData=FALSE;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆô´®¿Ú½ÓÊÜÖĞ¶Ï
	return;
}

void COMM_PROC(struct buffer *BUF)   //Êı¾İ´¦Àí
{
	
u8 c;
u8 subcmd;
u32 result; //FPGA·¢ËÍµÄ32Î»¶ş½øÖÆÊı
	c=BUF->buffer[0];   //Ö¡Í·
	subcmd=BUF->buffer[1]-'0';//¹¦ÄÜÎ»
	/*result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);
	if(result)
	  LCD_ShowNum(210,100,result,10,16);   //ÏÔÊ¾ÊÕµ½µÄÊı¾*/
	if (c=='P')
	{
		switch(subcmd)
		{
			case 1:	COMM_PC_P01(BUF);  //ÊÕµ½µÄÆµÂÊ
		 //LCD_ShowNum(50,50,1,5,24);
			break;
			case 2:	COMM_PC_P02(BUF);  //ÊÕµ½µÄ·ù¶È
			default: ;
		}
	}
}
void COMM_PC_P01(struct buffer *BUF)  //ÊÕµ½µÄÆµÂÊ
{
	int i;
	double data_freq = 0;//ÊÕµ½µÄÊ®½øÖÆÊı¾İ
	u8 t[20];//ÆµÂÊ
  for(i = 2;i < data_length;i++)   //½«ÊÕµ½µÄÊı¾İ×ªÎªÊ®½øÖÆ
	{
			data_freq += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	//LCD_ShowNum(100,100,data,10,16);   //ÏÔÊ¾ÊÕµ½µÄÊı¾İ
	sprintf((char*)t, "%.4f MHz  ",data_freq*1.0/1000000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
	LCD_ShowString(100,100,260,16,16,t); //ÏÔÊ¾ÆµÂÊ
	//LCD_ShowNum(100,120,data_length,10,16);//ÏÔÊ¾ÊÕµ½µÄÊı¾İ´óĞ¡*/
	//LCD_ShowString(210,100,200,20,16,BUF->buffer); //ÏÔÊ¾ÊÕµ½µÄÊı¾İ·ÖÀà
	//LCD_ShowNum(210,120,data_length,10,16);//ÏÔÊ¾ÊÕµ½µÄÊı¾İ´óĞ¡
	/*u32 result;
	u8 t[20];//ÖÜÆÚ
	result = (BUF->buffer[2] << 24) | (BUF->buffer[3] << 16 )|(BUF->buffer[4] << 8)|(BUF->buffer[5]);  //½«32Î»¶ş½øÖÆÊı×ª»¯Îª10½øÖÆÊı
	if(result)
	{
		LCD_ShowString(210,80,200,20,16,"Receive_1!"); //ÏÔÊ¾ÊÕµ½µÄÊı¾İ·ÖÀà
		if(result < 1000)
		{
			sprintf((char*)t, "%.4f Hz",result*1.0);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		  LCD_ShowString(110,90,260,12,12,t); //ÏÔÊ¾ÆµÂÊ
			
			sprintf((char*)t, "%.4f ms", 1.0/result*1000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		  LCD_ShowString(10,90,260,12,12,t); //ÏÔÊ¾ÖÜÆÚ
		}
	   // LCD_ShowString(180,120,20,20,16,"Hz"); //ÏÔÊ¾µ¥Î»
		else if(result >= 1000 && result < 1000000)
		{
			sprintf((char*)t, "%.4f KHz",result*1.0/1000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		  LCD_ShowString(110,90,260,12,12,t); //ÏÔÊ¾ÆµÂÊ
			
			sprintf((char*)t, "%.4f us", 1.0/result*1000000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
	  	LCD_ShowString(10,90,260,12,12,t); //ÏÔÊ¾ÖÜÆÚ
		}
			else if(result >= 1000000)
		{
			sprintf((char*)t, "%.4f MHz",result*1.0/1000000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		  LCD_ShowString(110,90,260,12,12,t); //ÏÔÊ¾ÆµÂÊ
			
			sprintf((char*)t, "%.4f ns", 1.0/result*1000000000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		 LCD_ShowString(10,90,260,12,12,t); //ÏÔÊ¾ÖÜÆÚ
		}
		
		
		//sprintf((char*)t, "%.8f", 1.0/result);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡
		//LCD_ShowString(10,90,260,12,16,t); //ÏÔÊ¾ÖÜÆÚ
		//LCD_ShowString(80,120,20,20,16,"S"); //ÏÔÊ¾µ¥Î»
		
		*/
		
	
	
}
 
void COMM_PC_P02(struct buffer *BUF)  //ÊÕµ½µÄ·ù¶È
{
	int i;
	u8 t[20];//·ù¶È
	double data_vpp = 0;//ÊÕµ½µÄÊ®½øÖÆÊı¾İ
  for(i = 2;i < data_length;i++)   //½«ÊÕµ½µÄÊı¾İ×ªÎªÊ®½øÖÆ
	{
			data_vpp += (BUF->buffer[i]-'0')*pow(10.0,(data_length-i-1));
	}
	sprintf((char*)t, "%.2f v  ",data_vpp*1.0/1000);  //Êı×Ö×ª×Ö·û´®£¬¿ÉÊä³öĞ¡Êıµã£¡  µ¥Î»ºó¼Ó¿Õ¸ñ£¬¿É½â¾ö×Ö·ûÂÒĞòÎÊÌâ£¨¸üĞÂºóÎŞ·¨ÍêÈ«¸²¸ÇÒÔÇ°µÄÊıÎÊÌâ£©
	LCD_ShowString(100,200,260,16,16,t); //ÏÔÊ¾ÆµÂÊ
}

