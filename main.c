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
TTLת����ģ��3.3v���磬�뵥Ƭ�����أ�rx��tx�ֱ���PA9,PA10����
P01,P02Ϊ����֡��ʽͷ ����ӷ������У�
���ݰ�������ν�һ������  ���������������� ����lcd��ʾ�������⣬��δ���
�Ժ�������չ���ݰ���ֻ����switch��comm_proc����Ӽ���
������������жϵĿ���
������������Զ̣���Ҫ�������ֹͣλ0a



���ƣ��̿�˥��PE43703 
���ã�0.25db���� 0-31.25db˥��  7bit
��os�趨 ���ɽ���15�����񣬴���10��event 5��flag 5������ ����os_cfg�н��и���
5.13 
 ��������   :
 �����뿪�ص�ps����H����Ƭ����˥����ģ�鹲�أ��������½��߱����� �������п�������
//              ˵��: 
//              ----------------------------------------------------------------
//              data ��PC3       �������� 16bit ��λ��ַ��λ˥��ֵ  
//              clk  ��PC4         ģ��IICͨ��  SCLK-CLK SDA-DATA    
//              le   ��PC5
//              p/s  ���ã����ɲ��뿪�ؿ��� L----���� H------�̿�
//              A0/A1/A2������λ��ֵַ����һ��  ��PC0,PC1,PC2       
//              ����x����0��ʾ  110x xxxx xyyy yyyy   yΪ7λ����  ��ַĬ��110 ��A0,A1Ϊ1��A2Ϊ0��
                ����λ�ӵ�λ�𷢣���ַΪ�Ӹ�λ����
//              data = 49152;  //1100 0000 0000 0000  0db
                datamax = 49279; //1100 0000 0111 1111  31.75db
								�ϵ�Ĭ�����˥��
5.21
 ������������arduino��ͨ�����⣬Э��ΪP1/2xxxxxodoa������1ΪƵ���֣�2Ϊ������
 ��������⣺1.��������˥����ģ�飬�Ƿ�ɹ��ô���
             2.�����ƣ������򵥸�Ч�Ŀ���˥��
************************************************/

#define LENGTH 255 //���ݰ�����
extern struct buffer{
	u8	buffer[LENGTH];//����
	u16	ptr;//ָ��
	u8	NewData;//�������ݱ�־
};
//struct buffer COMM_BUF_UART1;	*/
//struct buffer *COMM_BUF_UART1_1=&COMM_BUF_UART1;  
u8 sins[LENGTH]; //���ڷ��͵�����
extern struct buffer COMM_BUF_UART1;	
int main(void)
 {		
	u8 key = 0;
	u16 data = 49279; //�����͵�����
	u8 t[20];//˥��ֵ
	float Attenuation = 31.75;//˥��ֵ db 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ115200	
	LCD_Init();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	BEEP_Init();				//��������ʼ��	
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
  PE43703_Init();    //��ʼ����˥����ģ���Ӳ���ӿ�
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
   
	LCD_ShowString(80,50,200,20,24,"READY");
	 while(1)
  {		
		//PE43703_Send(49167);  //110x xxxx x000 1111  ����x��Ϊ0
      if (COMM_BUF_UART1.NewData==TRUE)  //�ж��Ƿ�������
	  { 
		  COMM_PROC(&COMM_BUF_UART1);//���ݴ���
		  buffer_Clear(&COMM_BUF_UART1);//�������
	  }
		key = KEY_Scan(0);
		if(key)
		{
			BEEP=1;
	   	delay_ms(50);
		  BEEP=0;
		}
		switch(key)  //��x����0��ʾ  110x xxxx yyyy yyyx   yΪ7λ����   
		{
			case 1:
				data = 49279;  //1100 0000 0111 1111  31.75db
			  Attenuation = 31.75;  //��λdb
			  sprintf((char*)t, "%.2f db",Attenuation);  //����ת�ַ����������С���㣡
		    LCD_ShowString(20,100,260,16,16,t); //��ʾ˥������
			  LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 2:
				if(data < 49279)
				{
				    data +=1;  //0.25db����
			      Attenuation += 0.25;
				}
				sprintf((char*)t, "%.2f db",Attenuation);  //����ת�ַ����������С���㣡
		    LCD_ShowString(20,100,260,16,16,t); //��ʾ˥������
				LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 3:
				data = 49152;  //1100 0000 0000 0000  0db
		  	Attenuation = 0;  //��λdb
			  sprintf((char*)t, "%.2f db",Attenuation);  //����ת�ַ����������С���㣡
		    LCD_ShowString(20,100,260,16,16,t); //��ʾ˥������
			  LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			case 4:
				if(data > 49152)
				{
				    data -=1;  //0.25db������С
			      Attenuation -= 0.25;
				}
				sprintf((char*)t, "%.2f db",Attenuation);  //����ת�ַ����������С���㣡
		    LCD_ShowString(20,100,260,16,16,t); //��ʾ˥������
				LCD_ShowNum(20,150,data,8,16);
			  PE43703_Send(data);
				break;
			default:
				break;
	 }
	}
	 
 }
