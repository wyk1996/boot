#include "at32_board.h"
#include "485_1.h"
#include "delay.h"



//-------------------------------------------------
void Uart1_Init(uint32_t BaudRate, uint16_t DataBits, uint16_t Parity, uint16_t StopBits)
{
    GPIO_InitType GPIO_InitStructure;
    USART_InitType USART_InitStructure;
    NVIC_InitType NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);	//ʹ��USART3��GPIOCʱ��

	 GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2;			
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    //USART3_485TX    GPIOB.10
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART3_485RX	  GPIOB.11
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    RCC_APB2PeriphResetCmd(RCC_APB1PERIPH_USART3,ENABLE);//��λ����1
    RCC_APB2PeriphResetCmd(RCC_APB1PERIPH_USART3,DISABLE);//ֹͣ��λ

    //USART ��ʼ������
    USART_InitStructure.USART_BaudRate = BaudRate;//���ڲ�����
    USART_InitStructure.USART_WordLength = DataBits;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = StopBits;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = Parity;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure);//��ʼ������3
    
  	USART_INTConfig(USART3, USART_INT_RDNE, ENABLE);
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;  //��ռ���ȼ�2
    //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		    //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//	USART_INTConfig(USART6, USART_INT_RDNE, ENABLE);
  //  BSP_IntVectSet(BSP_INT_ID_USART3, BSP_IntHandlerUSART6); //????1?????,?????
  // BSP_IntEn(BSP_INT_ID_USART3);

    USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3

}



/********************************************************************************
* ������ : Uart3_SendData
* ��  �� : ���ڷ�������
* ˵  �� : none
* ��  �� : none
* ��  �� : none
* ��  �� : none                       ʱ  �� : none
********************************************************************************/
void Uart1_SendData(uint8_t *buf,uint8_t len)
{
	uint8_t i=0;

	GPIO_SetBits(GPIOB, GPIO_Pins_2);    //����Ϊ����ģʽ
	delay_u(5);
  	for(i=0;i<len;i++)		//ѭ����������
	{
        while(USART_GetFlagStatus(USART3, USART_FLAG_TRAC)==RESET); //�ȴ����ͽ���
        USART_SendData(USART3, buf[i]); //��������
	}
	while(USART_GetFlagStatus(USART3, USART_FLAG_TRAC)==RESET){} //�ȴ����ͽ���
	GPIO_ResetBits(GPIOB, GPIO_Pins_2);    //����Ϊ����ģʽ

}


void USART3_IRQHandler(void)
{
	uint8_t ch;
	
	if(USART_GetITStatus(USART3, USART_INT_RDNE) != RESET)
	{ 	
		USART_ClearITPendingBit(USART3, USART_INT_RDNE);
		ch = USART_ReceiveData(USART3);
		#warning "YXY���ݴ���"	
	} 
}

