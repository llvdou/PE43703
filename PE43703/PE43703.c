#include "PE43703.h"
#include "delay.h"




void PE43703_Init(void) //�͵�ƽ���� �ߵ�ƽλѡ
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
    
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2;   //A0/A1/A2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					
		GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4|GPIO_Pin_5;//DATA/CLK/LE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					
		GPIO_ResetBits(GPIOC, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
		
   // IIC_Init();
}

//��PE43703��д������� 16bit

void PE43703_Send(u16 data)  //dataΪ16λ  ��λ��ַ����λ����
{			
   char low,high;
   high=(char)(data>>8);
   low=(char)(data&0xFF);
	
   L_LE;   //����LE
	 delay_us(1);//�ʵ���ʱ
	 H_A0;H_A1;L_A2;//�̶���ַλ110xxxxx
	 delay_us(1);//�ʵ���ʱ
	 //IIC_Send_2Byte(data);//��������
	 //IIC_Send_Byte(low);//�ȷ�����λ 0+7bi
	 IIC_Send_2Byte(low);//�ȷ���λ
	 IIC_Send_Byte(high);//�ٷ���ַλ �ȷ���λ
	
	 delay_us(2);//�ʵ���ʱ
	 H_LE;   //����LE
}