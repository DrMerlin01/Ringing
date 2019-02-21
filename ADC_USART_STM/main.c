#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"

void ADCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//структура для настройки порта А
	ADC_InitTypeDef ADC_InitStructure;//структура для настройки АЦП

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//максимум 24Мгц

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE ) ;//тактироваание порта А
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE ) ;//тактирование АЦП

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;//определяем порты 1 и 4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//на вход без потдтяжки
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//высокая скорость
	GPIO_Init(GPIOA, &GPIO_InitStructure);//загружаем настройки

	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//непрерывная работа АЦП
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//выравнивание результата вправо
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//по внешнему событию откл.
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//
	ADC_InitStructure.ADC_NbrOfChannel = 2;//число каналов, с которыми работаем
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//сканирование каналов
	ADC_Init(ADC1, &ADC_InitStructure);//загружаем настройки

	ADC_InjectedSequencerLengthConfig(ADC1, 2);//настройка регистров инжекторных каналов, указываем,что их 2
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);//сопоставляем каналы регистрам
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_7Cycles5);//сопоставляем каналы регистрам

	ADC_ExternalTrigInjectedConvConfig( ADC1, ADC_ExternalTrigInjecConv_None );//триггер выкл

	ADC_Cmd(ADC1, ENABLE);//запускаем АЦП

	ADC_ResetCalibration(ADC1);//включить регистр калибровки сброса АЦП1
	while(ADC_GetResetCalibrationStatus(ADC1));//проверка конца калибровочного регистра сброса АЦП1
	ADC_StartCalibration(ADC1);//начать калибровку АЦП1
	while(ADC_GetCalibrationStatus(ADC1));//проверка окончания калибровки АЦП1

	ADC_AutoInjectedConvCmd(ADC1, ENABLE );//запуск с инжекторного канала
	ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);//запуск преобразования
}

void USART2_Init()
{
    GPIO_InitTypeDef GPIO_Settings;//структура для настройки порта А
    USART_InitTypeDef USART_Settrings;//структура для настройки usart2

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//тактирование usart2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//тактирование порта А

    GPIO_StructInit(&GPIO_Settings);//инициализируем структуру
    GPIO_Settings.GPIO_Pin = GPIO_Pin_2;//второй вывод Tx
    GPIO_Settings.GPIO_Mode = GPIO_Mode_AF_PP;//на выход, альт. функция
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;//средняя скорость
    GPIO_Init(GPIOA , &GPIO_Settings);//загружаем настройки

    GPIO_Settings.GPIO_Pin = GPIO_Pin_3;// Rx
    GPIO_Settings.GPIO_Mode = GPIO_Mode_IN_FLOATING;//на вход без подтяжки, альт. функция
    GPIO_Settings.GPIO_Speed = GPIO_Speed_2MHz;//скорость обмена
    GPIO_Init(GPIOA , &GPIO_Settings);//загружаем анстройки

    USART_StructInit(&USART_Settrings);//инициализируем структуру
    USART_Settrings.USART_BaudRate = 57600;//скорость обмена данными
    USART_Settrings.USART_WordLength = USART_WordLength_8b;//посылки по 8 бит
    USART_Settrings.USART_StopBits = USART_StopBits_1;//1 стоп бит
    USART_Settrings.USART_Parity = USART_Parity_No;//четность - нет
    USART_Settrings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//управления потоком нет
    USART_Settrings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//режим на прием и передачу
    USART_Init(USART2, &USART_Settrings);//загружаем настройки

    USART_Cmd(USART2, ENABLE);//запускаем USART2

    __enable_irq();//разрешить глобальные прерываания
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//прерывание по приему
    NVIC_EnableIRQ(USART2_IRQn);//разрешаем прерывание
}

void usart_send(const char chr)//посылаем символ
{
    while(!(USART2->SR & USART_SR_TC));//ждем пока регистр будет свободен
	USART2->DR = chr;//посылаем символ
}

void usart_send_string(const char *s)//передача строки
{
    int i=0;//подсчет элемента строки
    while (s[i])//проходимся по всей строке
    {
        usart_send(s[i++]);//посылаем по символу
    }
}

void send_num(uint32_t x)//отправка число
{
  char value[10]; //массив для отцифрованных значений
  int i = 0; //счетчик

  do
  {
    value[i++] = (char)(x % 10) + '0'; //записываем наше значение по символьно
    x /= 10;
  } while(x);//пока х больше нуля

  while(i) //отправка данных, пока они есть
  {
	  usart_send(value[--i]);//отправляем символы
  }
}

void delay(int millis)//задержка
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
	USART2_Init();//инициализация USART2
	ADCInit();//инициализация АЦП

	uint16_t adc_value;//переменная для получения значения из АЦП
	uint16_t adc2_value;//переменная для получения значения из АЦП

    while(1)
    {
    	adc_value = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);// define regular conversion config
    	send_num(adc_value);//посылаем отцифрованное значение с первого канала
    	usart_send('v');
    	adc2_value = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2);
    	send_num(adc2_value);//посылаем отцифрованное значение со второго канала
    	usart_send(' ');//посылаем пробел
    	delay(200);//небольшая задержка
    }
}
