/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1276 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "radio.h"
#include "gpio.h"
#include "spi.h"
//#include "sx1276/sx1276.h"
#include "sx1276-board.h"
//#include "delay.h"
#include "stdio.h"
#include "user_timer.h"

//��λ��
#define SX1278_RST_PIN_INDEX	GPIO_Pin_10
#define SX1278_RST_PIN_GROUP	GPIOB

#define SX1278_CS_PIN_INDEX	GPIO_Pin_4
#define SX1278_CS_PIN_GROUP	GPIOA
#define SX1278_SCK_PIN_INDEX	GPIO_Pin_5
#define SX1278_SCK_PIN_GROUP	GPIOA
#define SX1278_MISO_PIN_INDEX	GPIO_Pin_6
#define SX1278_MISO_PIN_GROUP	GPIOA
#define SX1278_MOSI_PIN_INDEX	GPIO_Pin_7
#define SX1278_MOSI_PIN_GROUP	GPIOA

#define SX1278_DIO0_PIN_INDEX	GPIO_Pin_0
#define SX1278_DIO0_PIN_GROUP	GPIOB

#define SX1278_DIO1_PIN_INDEX	GPIO_Pin_1
#define SX1278_DIO1_PIN_GROUP	GPIOB


#define DELAY_MS(time)				HAL_Delay(time);



/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby, 
    SX1276SetRx,
    SX1276StartCad,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength
};
/**
!
 * Antenna switch GPIO pins objects
 */
/* ���ǵ�RA-01/02ģ��������л�������Ӳ���Զ����ƣ�����Ҫ��������
Gpio_t AntSwitchLf;
Gpio_t AntSwitchHf;*/

/*!
 * Tx and Rx timers
 */
/*TimerEvent_t TxTimeoutTimer;
TimerEvent_t RxTimeoutTimer;
TimerEvent_t RxTimeoutSyncWord;*/

//ʵ��ms����ʱ
//delayMs����ʱ��ms��

void SX1276DelayMs(uint32_t delayMs){
	DELAY_MS(delayMs);
}

//��ʱ����ʼ��(RX,TX,SyncWord��ʱ��ɺ���Ҫ���� SX1276OnTimeoutIrq() ����)
void SX1276TimerInit(void){
}

void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
	Init_Timer(&Timer2_Cfg);
}
//��ʱ��3�жϷ������
/*
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
		TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx
		SX1276OnTimeoutIrq();	//�����жϺ���Ҫ���� SX1276OnTimeoutIrq ����
		//printf("\r\ntimer irq\r\n");
	}
}*/
//���� tx ��ʱ��ʱ������ʱtimeoutMs ����
void SX1276TxTimeoutTimerStart( uint32_t timeoutMs ){
	//�����ʹ����һ����ʱ����ʵ�ʹ��������ʹ�ö�����3����ʱ��������ʹ��������ʱ��
	TIM3_Int_Init((timeoutMs * 10) -1,7199);//10Khz�ļ���Ƶ�ʣ�������10��Ϊ1ms 
}

//�ر� tx ��ʱ��ʱ��
void SX1276TxTimeoutTimerStop(void){
	TIM_CMD(TIM2, DISABLE);  //ʧ��TIMx
}


void SX1276RxTimeoutTimerStart( uint32_t timeoutMs ){
	TIM3_Int_Init((timeoutMs * 10) -1,7199);
}



void SX1276RxTimeoutTimerStop(void){
	TIM_CMD(TIM2, DISABLE);  //ʧ��TIMx
}

void SX1276SyncWordTimeoutTimerStart( uint32_t timeoutMs ){
	TIM3_Int_Init((timeoutMs * 10) -1,7199);//10Khz�ļ���Ƶ�ʣ�������10��Ϊ1ms 
}

//�ر� SyncWord ��ʱ��ʱ��
void SX1276SyncWordTimeoutTimerStop(void){
	///�����ʹ����һ����ʱ����ʵ�ʹ��������ʹ�ö�����3����ʱ��������ʹ��������ʱ��
	TIM_CMD(TIM2, DISABLE);  //ʧ��TIMx
}
/*
//��ʼ��SPI(cs��ʹ�ܣ�����SPI_CPOL_High����λSPI_CPHA_2Edge��SPI_FirstBit_MSB)��Rst��
void SX1276IoInit( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	//��ʼ��һ��gpio��������spi1��cs
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��ָ���˿�ʱ�ӣ�������Ϊ�˷���ABCȫ���ˣ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz�����ﲻ�ô��Σ�ֱ��д��������ٶ�50MHZ
	GPIO_InitStructure.GPIO_Pin = SX1278_CS_PIN_INDEX;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//����Ϊ�������
	GPIO_Init(SX1278_CS_PIN_GROUP, &GPIO_InitStructure);	//��ʼ��GPIO

	//SPI�ĳ�ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//SPIʱ��ʹ��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	//Ƭ������ģʽ
	GPIO_InitStructure.GPIO_Pin = SX1278_SCK_PIN_INDEX|SX1278_MISO_PIN_INDEX|SX1278_MOSI_PIN_INDEX;
	GPIO_Init(SX1278_SCK_PIN_GROUP, &GPIO_InitStructure);	//��ʼ��GPIO

	//����SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ʱ�Ӽ���(����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ���ǵ͵�ƽ)
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//������λ(����ͬ��ʱ�ӵĵڼ��������أ��������½������ݱ�����)
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:����Ϊ��������(SSI����)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256(256�����,�����������֮������ٶ�,����ٶȹ��쵼��ͨ��ʧ���ٵ�С)
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ(CRCУ�����)
	SPI_Init(SPI1,&SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI����
}*/

DioIrqHandler **g_irqHandlers;
/*
void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	g_irqHandlers=irqHandlers;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ��RCC_APB2Periph_AFIO��ʱ��
	
	//��ʼ��DIO0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	    //ʹ��ָ���˿�ʱ�ӣ�������Ϊ�˷���ABCȫ���ˣ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz�����ﲻ�ô��Σ�ֱ��д��������ٶ�50MHZ
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//��������
	GPIO_InitStructure.GPIO_Pin = SX1278_DIO0_PIN_INDEX;
	GPIO_Init(SX1278_DIO0_PIN_GROUP, &GPIO_InitStructure);	//��ʼ��GPIO
	
	//�����ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);//����ӳ���߹�ϵ(����� GPIO_PortSourceGPIOX �պ���0-5������ֱ����pinName/16��ʾ��GPIO_PinSourceXX�պ���0-15��������pinName%16��ʾ��)
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	//�ж���u8_pinNum
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ⲿ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�������ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж�
	EXTI_Init(&EXTI_InitStructure);//�����ж�

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;	//�ⲿ�ж�XX
	//stm32f103c8������������ȼ���Ӧ��ռ���ȼ�2�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);   //�������ȼ�
	
	EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־λ
	
	//DIO1�ж�
	GPIO_InitStructure.GPIO_Pin = SX1278_DIO1_PIN_INDEX;
	GPIO_Init(SX1278_DIO1_PIN_GROUP, &GPIO_InitStructure);	//��ʼ��GPIO
	
	//�����ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);//����ӳ���߹�ϵ(����� GPIO_PortSourceGPIOX �պ���0-5������ֱ����pinName/16��ʾ��GPIO_PinSourceXX�պ���0-15��������pinName%16��ʾ��)
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	//�ж���u8_pinNum
	EXTI_Init(&EXTI_InitStructure);//�����ж�

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;	//�ⲿ�ж�XX
	NVIC_Init(&NVIC_InitStructure);   //�������ȼ�
	
	EXTI_ClearITPendingBit(EXTI_Line1);//����жϱ�־λ
	
	//DIO2~5ʡ����
}*/

//�ⲿ�ж���0�жϴ���������������Ҫ�ص���Ӧ�ĺ���
/*void EXTI0_IRQHandler(void){
	g_irqHandlers[0]();
	EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־λ
}

//�ⲿ�ж���1�жϴ���������������Ҫ�ص���Ӧ�ĺ���
void EXTI1_IRQHandler(void){
	g_irqHandlers[1]();
	EXTI_ClearITPendingBit(EXTI_Line1);//����жϱ�־λ
}*/

//��SPI��DIO0~5��Rst�ŵ��ж�ȥ��ʼ��
void SX1276IoDeInit( void )
{

}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}


void SX1276Reset( void )
{
	HAL_GPIO_WritePin(SX1278_RST_PIN_INDEX_GPIO_Port, SX1278_RST_PIN_INDEX_Pin, GPIO_PIN_RESET);
	SX1276DelayMs(1);
	HAL_GPIO_WritePin(SX1278_RST_PIN_INDEX_GPIO_Port, SX1278_RST_PIN_INDEX_Pin, GPIO_PIN_SET);
	SX1276DelayMs(6);
}


uint8_t Sx1276SpiInOut(uint8_t transmit_data)
{
	uint8_t receive_data;
	HAL_SPI_TransmitReceive(&hspi1,&transmit_data,&receive_data,1,2000);
	return receive_data;
}

void Sx1276SetNSS(bool lev )
{
	if(lev){
		HAL_GPIO_WritePin(SPI_CSS_Pin_GPIO_Port, SPI_CSS_Pin_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(SPI_CSS_Pin_GPIO_Port, SPI_CSS_Pin_Pin, GPIO_PIN_RESET);
	}
}





