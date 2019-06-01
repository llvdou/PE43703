#include "PE43703.h"
#include "delay.h"




void PE43703_Init(void) //低电平段亮 高电平位选
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOC时钟
    
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2;   //A0/A1/A2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					
		GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4|GPIO_Pin_5;//DATA/CLK/LE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					
		GPIO_ResetBits(GPIOC, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
		
   // IIC_Init();
}

//往PE43703中写入控制字 16bit

void PE43703_Send(u16 data)  //data为16位  八位地址＋八位数据
{			
   char low,high;
   high=(char)(data>>8);
   low=(char)(data&0xFF);
	
   L_LE;   //拉低LE
	 delay_us(1);//适当延时
	 H_A0;H_A1;L_A2;//固定地址位110xxxxx
	 delay_us(1);//适当延时
	 //IIC_Send_2Byte(data);//发送数据
	 //IIC_Send_Byte(low);//先发数据位 0+7bi
	 IIC_Send_2Byte(low);//先发低位
	 IIC_Send_Byte(high);//再发地址位 先发高位
	
	 delay_us(2);//适当延时
	 H_LE;   //拉高LE
}