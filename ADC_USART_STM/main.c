#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"

void ADCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//��������� ��� ��������� ����� �
	ADC_InitTypeDef ADC_InitStructure;//��������� ��� ��������� ���

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//�������� 24���

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE ) ;//������������� ����� �
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;//������������ ���

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;//���������� ����� 1 � 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//�� ���� ��� ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//������� ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��������� ���������

	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����������� ������ ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//������������ ���������� ������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�� �������� ������� ����.
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//
	ADC_InitStructure.ADC_NbrOfChannel = 2;//����� �������, � �������� ��������
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//������������ �������
	ADC_Init(ADC1, &ADC_InitStructure);//��������� ���������

	ADC_InjectedSequencerLengthConfig(ADC1, 2);//��������� ��������� ����������� �������, ���������,��� �� 2
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);//������������ ������ ���������
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_7Cycles5);//������������ ������ ���������

	ADC_ExternalTrigInjectedConvConfig( ADC1, ADC_ExternalTrigInjecConv_None );//������� ����

	ADC_Cmd(ADC1, ENABLE);//��������� ���

	ADC_ResetCalibration(ADC1);//�������� ������� ���������� ������ ���1
	while(ADC_GetResetCalibrationStatus(ADC1));//�������� ����� �������������� �������� ������ ���1
	ADC_StartCalibration(ADC1);//������ ���������� ���1
	while(ADC_GetCalibrationStatus(ADC1));//�������� ��������� ���������� ���1

	ADC_AutoInjectedConvCmd(ADC1, ENABLE );//������ � ������������ ������
	ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);//������ ��������������
}

void USART2_Init()
{
    GPIO_InitTypeDef GPIO_Settings;//��������� ��� ��������� ����� �
    USART_InitTypeDef USART_Settrings;//��������� ��� ��������� usart2

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//������������ usart2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//������������ ����� �

    GPIO_StructInit(&GPIO_Settings);//�������������� ���������
    GPIO_Settings.GPIO_Pin = GPIO_Pin_2;//������ ����� Tx
    GPIO_Settings.GPIO_Mode = GPIO_Mode_AF_PP;//�� �����, ����. �������
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;//������� ��������
    GPIO_Init(GPIOA , &GPIO_Settings);//��������� ���������

    GPIO_Settings.GPIO_Pin = GPIO_Pin_3;// Rx
    GPIO_Settings.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�� ���� ��� ��������, ����. �������
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;//�������� ������
    GPIO_Init(GPIOA , &GPIO_Settings);//��������� ���������

    USART_StructInit(&USART_Settrings);//�������������� ���������
    USART_Settrings.USART_BaudRate = 57600;//�������� ������ �������
    USART_Settrings.USART_WordLength = USART_WordLength_8b;//������� �� 8 ���
    USART_Settrings.USART_StopBits = USART_StopBits_1;//1 ���� ���
    USART_Settrings.USART_Parity = USART_Parity_No;//�������� - ���
    USART_Settrings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//���������� ������� ���
    USART_Settrings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//����� �� ����� � ��������
    USART_Init(USART2, &USART_Settrings);//��������� ���������

    USART_Cmd(USART2, ENABLE);//��������� USART2

    __enable_irq();//��������� ���������� �����������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������� �� ������
    NVIC_EnableIRQ(USART2_IRQn);//��������� ����������
}

void usart_send(const char chr)//�������� ������
{
    while(!(USART2->SR & USART_SR_TC));//���� ���� ������� ����� ��������
	USART2->DR = chr;//�������� ������
}

void usart_send_string(const char *s)//�������� ������
{
    int i=0;//������� �������� ������
    while (s[i])//���������� �� ���� ������
    {
        usart_send(s[i++]);//�������� �� �������
    }
}

void send_num(uint32_t x)//�������� �����
{
  char value[10]; //������ ��� ������������� ��������
  int i = 0; //�������

  do
  {
    value[i++] = (char)(x % 10) + '0'; //���������� ���� �������� �� ���������
    x /= 10;
  } while(x);//���� � ������ ����

  while(i) //�������� ������, ���� ��� ����
  {
	  usart_send(value[--i]);//���������� �������
  }
}

void delay(int millis)//��������
{
    while (millis-- > 0)
    {
        volatile int x = 5971;
        while (x-- > 0)
        {
            __asm("nop");
        }
    }
}

int main(void)
{
	USART2_Init();//������������� USART2
	ADCInit();//������������� ���

	uint16_t adc_value;//���������� ��� ��������� �������� �� ���
	uint16_t adc2_value;//���������� ��� ��������� �������� �� ���

    while(1)
    {
    	adc_value = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);// define regular conversion config
    	send_num(adc_value);//�������� ������������� �������� � ������� ������
    	usart_send('v');
    	adc2_value = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2);
    	send_num(adc2_value);//�������� ������������� �������� �� ������� ������
    	usart_send(' ');//�������� ������
    	delay(200);//��������� ��������
    }
}
