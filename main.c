#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "math.h"
#include "PE43703.h"
#include "beep.h" 
#include "stdio.h"
/************************************************
TTL转串口模块3.3v供电，与单片机共地，rx，tx分别与PA9,PA10相连
P01,P02为数据帧格式头 （需加发送新行）
数据包长度如何进一步扩大？  扩大后可以正常发送 但是lcd显示出现问题，暂未解决
以后若需扩展数据包，只需在switch与comm_proc中添加即可
可以软件控制中断的开闭
如果发送数据略短，需要额外添加停止位0a



名称：程控衰减PE43703 
作用：0.25db步进 0-31.25db衰减  7bit
本os设定 最大可进行15个任务，创建10个event 5个flag 5个队列 可在os_cfg中进行更改
5.13 
 功能描述   :
 将拨码开关的ps拨至H，单片机与衰减器模块共地，按照以下接线表连线 按键进行控制增益
//              说明: 
//              ----------------------------------------------------------------
//              data 接PC3       串行输入 16bit 高位地址低位衰减值  
//              clk  接PC4         模拟IIC通信  SCLK-CLK SDA-DATA    
//              le   接PC5
//              p/s  空置，可由拨码开关控制 L----键控 H------程控
//              A0/A1/A2需与三位地址值保持一致  接PC0,PC1,PC2       
//              所有x均用0表示  110x xxxx xyyy yyyy   y为7位数据  地址默认110 （A0,A1为1，A2为0）
                数据位从低位起发，地址为从高位起发送
//              data = 49152;  //1100 0000 0000 0000  0db
                datamax = 49279; //1100 0000 0111 1111  31.75db
								上电默认最大衰减
5.21
 解决与服务器（arduino）通信问题，协议为P1/2xxxxxodoa，其中1为频率字，2为幅度字
 待解决问题：1.驱动三个衰减器模块，是否可共用串口
             2.如何设计，尽量简单高效的控制衰减
************************************************/

#define LENGTH 255 //数据包长度
extern struct buffer{
	u8	buffer[LENGTH];//数组
	u16	ptr;//指针
	u8	NewData;//完整数据标志
};
//struct buffer COMM_BUF_UART1;	*/
//struct buffer *COMM_BUF_UART1_1=&COMM_BUF_UART1;  
u8 sins[LENGTH]; //串口发送的数据
extern struct buffer COMM_BUF_UART1;	
int main(void)
 {		
	u8 key = 0;
	u16 data = 49279; //待发送的数据
	u8 t[20];//衰减值
	float Attenuation = 31.75;//衰减值 db 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为115200	
	LCD_Init();
 	LED_Init();			     //LED端口初始化
	BEEP_Init();				//蜂鸣器初始化	
	KEY_Init();          //初始化与按键连接的硬件接口
  PE43703_Init();    //初始化与衰减器模块的硬件接口
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
   
	LCD_ShowString(80,50,200,20,24,"READY");
	 while(1)
  {		
		//PE43703_Send(49167);  //110x xxxx x000 1111  所有x均为0
      if (COMM_BUF_UART1.NewData==TRUE)  //判断是否接受完毕
	  { 
		  COMM_PROC(&COMM_BUF_UART1);//数据处理
		  buffer_Clear(&COMM_BUF_UART1);//清楚缓存
	  }
		key = KEY_Scan(0);
		if(key)
		{
			BEEP=1;
	   	delay_ms(50);
		  BEEP=0;
		}
		switch(key)  //有x均用0表示  110x xxxx yyyy yyyx   y为7位数据   
		{
			case 1:
				data = 49279;  //1100 0000 0111 1111  31.75db
			  Attenuation = 31.75;  //单位db
			  sprintf((char*)t, "%.2f db",Attenuation);  //数字转字符串，可输出小数点！
		    LCD_ShowString(20,100,260,16,16,t); //显示衰减倍数
			  LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 2:
				if(data < 49279)
				{
				    data +=1;  //0.25db步进
			      Attenuation += 0.25;
				}
				sprintf((char*)t, "%.2f db",Attenuation);  //数字转字符串，可输出小数点！
		    LCD_ShowString(20,100,260,16,16,t); //显示衰减倍数
				LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 3:
				data = 49152;  //1100 0000 0000 0000  0db
		  	Attenuation = 0;  //单位db
			  sprintf((char*)t, "%.2f db",Attenuation);  //数字转字符串，可输出小数点！
		    LCD_ShowString(20,100,260,16,16,t); //显示衰减倍数
			  LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 4:
				if(data > 49152)
				{
				    data -=1;  //0.25db步进减小
			      Attenuation -= 0.25;
				}
				sprintf((char*)t, "%.2f db",Attenuation);  //数字转字符串，可输出小数点！
		    LCD_ShowString(20,100,260,16,16,t); //显示衰减倍数
				LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			default:
				break;
	 }
	}
	 
 }
