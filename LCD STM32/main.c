#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#define port    	GPIOB
#define init_port 	RCC_APB2Periph_GPIOB
#define pin_e 		GPIO_Pin_10
#define pin_rw		GPIO_Pin_11
#define pin_rs		GPIO_Pin_12

#define lcd_shift	6			///номер последниего бита в 4-х битной шине
#define use_gpio	GPIO_Pin_9  //старший бит								9
#define pin_d7		use_gpio    //старший бит								9
#define pin_d6		use_gpio>>1 //следующий бит по убыванию					8
#define pin_d5		use_gpio>>2 //следующий бит по убыванию					7
#define pin_d4		use_gpio>>3 //последний бит в 4-х битной шине			6

#define Function_set 				0b00100000//4-bit,2 - line mode, 5*8 dots
#define Display_on_off_control		0b00001111/// display on,cursor off,blink off
#define Display_clear				0b00000001
#define Entry_mode_set				0b00000100//

#define rs_1	port->ODR |=  pin_rs
#define rs_0	port->ODR &=~ pin_rs
#define e_1 	port->ODR |=  pin_e
#define e_0		port->ODR &=~ pin_e
#define rw_1	port->ODR |=  pin_rw
#define rw_0	port->ODR &=~ pin_rw
u32 del;
const unsigned char russian[]={ 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45,
0xA3, 0xA4, 0xA5,0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50,0x43,
0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD,0xAE, 0x62,
0xAF, 0xB0, 0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63,0xBF,
0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2,0xC3, 0xC4, 0xC5,
0xC6, 0xC7 };//все русские символы

void Init_pin_out(void);//настройка портов на выход
void Init_pin_in(void);//настройка портов на вход
void Init_lcd(void);//инициализация дисплея
void Lcd_write_data(uint8_t byte);//запись данных
void Lcd_write_cmd(uint8_t byte);//запись команд
void Lcd_clear(void);//очистка экрана
void Return_home(void);//вернуться в начало
void Lcd_goto(uc8 x, uc8 y);//положение курсора
void Lcd_write_str(uc8 *STRING);//запись строки

void lcd_definechar(const uint8_t *pc,uint8_t char_code);//создание пользовательского символа


int second = 55;//Текущее время
int minute = 44;
int hours = 8;
const uint8_t smile[]= { 0b00100, 0b00010, 0b10001, 0b00001, 0b10001, 0b00010, 0b00100, 0b0 };//пользовательский символ

void clockTime(int x)//определяем какое число вывести
{
	switch(x)
	{
		case 0:
		{
			Lcd_write_str("0");
			break;
		}
		case 1:
		{
			Lcd_write_str("1");
			break;
		}
		case 2:
		{
			Lcd_write_str("2");
			break;
		}
		case 3:
		{
			Lcd_write_str("3");
			break;
		}
		case 4:
		{
			Lcd_write_str("4");
			break;
		}
		case 5:
		{
			Lcd_write_str("5");
			break;
		}
		case 6:
		{
			Lcd_write_str("6");
			break;
		}
		case 7:
		{
			Lcd_write_str("7");
			break;
		}
		case 8:
		{
			Lcd_write_str("8");
			break;
		}
		case 9:
		{
			Lcd_write_str("9");
			break;
		}
	}
}

void TIM3_IRQHandler(void)//часы
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//Очищаем бит прерывания
	second++;//увеличивает секунды
	if(second>59)//подсчет реальных часов
	{
		second=0;
		minute++;//увеличиваем минуты
		if(minute>59)
		{
			minute=0;
			hours++;//увеличиваем часы
			if(hours>23)
			{
				hours=0;
			}
		}
	}
}

void TIM4_IRQHandler(void)//выводим время нажатия во второй строке
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//Очищаем бит прерывания

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)//если нажата кнопка
	{
		int Hour=hours;//переписываем текущее время
		int Min=minute;
		int Sec=second;

		Lcd_goto(1,8);//выводим время нажатия
		clockTime(Hour/10);
		Lcd_goto(1,9);
		clockTime(Hour%10);
		Lcd_goto(1,10);
		Lcd_write_str(":");
		Lcd_goto(1,11);
		clockTime(Min/10);
		Lcd_goto(1,12);
		clockTime(Min%10);
		Lcd_goto(1,13);
		Lcd_write_str(":");
		Lcd_goto(1,14);
		clockTime(Sec/10);
		Lcd_goto(1,15);
		clockTime(Sec%10);
	}
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)//если нажата кнопка
	{
		int Hour=hours;//переписываем текущее время
		int Min=minute;
		int Sec=second;

		Lcd_goto(2,8);//выводим время нажатия
		clockTime(Hour/10);
		Lcd_goto(2,9);
		clockTime(Hour%10);
		Lcd_goto(2,10);
		Lcd_write_str(":");
		Lcd_goto(2,11);
		clockTime(Min/10);
		Lcd_goto(2,12);
		clockTime(Min%10);
		Lcd_goto(2,13);
		Lcd_write_str(":");
		Lcd_goto(2,14);
		clockTime(Sec/10);
		Lcd_goto(2,15);
		clockTime(Sec%10);
	}
}

void TIM2_IRQHandler(void)//Вывод на экран раз в секунду времени
{
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//Очищаем бит прерывания

	Lcd_goto(0,8);
	clockTime(hours/10);
	Lcd_goto(0,9);
	clockTime(hours%10);
	Lcd_goto(0,10);
	Lcd_write_str(":");
	Lcd_goto(0,11);
	clockTime(minute/10);
	Lcd_goto(0,12);
	clockTime(minute%10);
	Lcd_goto(0,13);
	Lcd_write_str(":");
	Lcd_goto(0,14);
	clockTime(second/10);
	Lcd_goto(0,15);
	clockTime(second%10);
}

void main(void)
{
	SystemInit();
	Init_lcd();//инициализация экрана
	Lcd_clear();//очистка экрана
	Lcd_definechar(smile,0x06);//записывваем пользовательский символ по указанному адресу
	Lcd_write_cmd(0b00001100);

	GPIO_InitTypeDef PORTA;//создание структр

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//тактирование порта B
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//тактирование порта А
	//Порт B
	GPIO_StructInit (&PORTA);
	PORTA.GPIO_Mode = GPIO_Mode_IPU;//на выход
	PORTA.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//0 и 1 порты
	PORTA.GPIO_Speed = GPIO_Speed_2MHz;//скорость средняя
	GPIO_Init (GPIOA,&PORTA);//настраиваем согласно заполненной структуре

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//включаем тактирование таймера 4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//включаем тактирование таймера 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//включаем тактирование таймера 2

	//Таймер 4
	TIM4->PSC = 2400;//предделитель
	TIM4->ARR = 10000;//период, примерно каждые 40 мсек
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM4, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM4_IRQn);//разрешаем прерывание таймера 4

	//Таймер 3
	TIM3->PSC = 2400;//предделитель
	TIM3->ARR = 10000;//период равный 1 секунде
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM3, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM3_IRQn);//разрешаем прерывание таймера 3

	//Таймер 2
	TIM2->PSC = 2400;//предделитель
	TIM2->ARR = 10000;//период равный 1 секунде
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//Настраиваем таймер для генерации прерывания по обновлению (переполнению)
	TIM_Cmd(TIM2, ENABLE);//Запускаем таймер
	NVIC_EnableIRQ(TIM2_IRQn);//разрешаем прерывание таймера 2

	Lcd_goto(0,0);//в первой строке
	Lcd_write_str("Кригер");//первая фамилия
	Lcd_goto(1,0);//во второй строке
	Lcd_write_str("Хлызов");//вторая фамилия
	Lcd_goto(2,0);//в третей строке
	Lcd_write_str("Козлова");//третья фамилия
	Lcd_goto(3,0);// в четвертой строке
	Lcd_write_str("Савинков");//четвертая фамилия
	Lcd_goto(3,9);//т в четвертой строке символ
	Lcd_write_data(0x06);//выводим символ наш

    while (1){}
}

void Lcd_write_str(uc8 *STRING)//запись строки
{
	char c; //символ из строки
	while (c=*STRING++){//проходимся по всей строке
	if(c>=192) Lcd_write_data(russian[c-192]);//если у нас русские символы
	else Lcd_write_data(c);//иначе прочие
	}
}

void Lcd_goto(uc8 x,uc8 y)//позиция курсора
{
	switch(x)
		{
			case 0://определяем строку первая
				Lcd_write_cmd(y|0x80);//позиция
				break;
			case 1://вторая
				Lcd_write_cmd(0x40+y|0x80);//позиция по счету
				break;
			case 2://третья
				Lcd_write_cmd(0x10+y|0x80);//позиция по счету согласно даташиту
				break;
			case 3://ну и четвертая
				Lcd_write_cmd(0x50+y|0x80);//позиция по счету
				break;
		}
}

void Init_pin_out()//настройка портов на выход
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//тактирование порта B
	GPIO_InitTypeDef init_pin;//создание структуры
	init_pin.GPIO_Pin  = pin_e | pin_rs | pin_rw | pin_d7 | pin_d6 | pin_d5 | pin_d4;//4, 5, 6, 7 , 12, 11, 10 выводы
	init_pin.GPIO_Mode = GPIO_Mode_Out_PP;//на выход
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//скорость
	GPIO_Init (port, &init_pin);//запись в структуру текущих настроек
}
void Init_pin_in()//настройка портов на вход
{
	RCC_APB2PeriphClockCmd(init_port, ENABLE);//тактирование порта B
	GPIO_InitTypeDef init_pin;//создание структуры
	init_pin.GPIO_Pin  =  pin_d7 | pin_d6 | pin_d5 | pin_d4 ;//4, 5, 6 и 7 выводы
	init_pin.GPIO_Mode = GPIO_Mode_IPD;//на вход
	init_pin.GPIO_Speed = GPIO_Speed_50MHz;//скорость
	GPIO_Init (port, &init_pin);//запись в структуру текущих настроек
}

void Lcd_write_cmd(uc8 cmd )
{
	Init_pin_out();//настроить порты на выход
	del=24000; while (del--){}//задержка
	rs_0;//показываем что будут передаваться команды
	GPIO_Write(port,((cmd>>4)<<lcd_shift));//передаем старшую часть
	e_1;//начинаем передачу
	del=10; while (del--){}//задержка
	e_0;//заканчиваем передачу
	GPIO_Write(port,(0x00)<<lcd_shift);//
	GPIO_Write(port,((cmd&0x0F)<<lcd_shift));//передаем младшую часть
	del=10; while (del--){}//задержка
	e_1;//начинаем передачу
	del=10; while (del--){}//задержка
	e_0;rs_0;rw_0;//заканчиваем передачу
}

void Lcd_write_data(uint8_t data)
{
	Init_pin_out();//настроить порты на выход
	GPIO_Write(port,((data>>4)<<lcd_shift));//передаем старшую часть
	e_1;rs_1;//начинаем передачу данных
	del=10; while (del--){}//задержка
	e_0;//заканчиваем передачу
	GPIO_Write(port,(0x00)<<lcd_shift);
	GPIO_Write(port,((data&0x0F)<<lcd_shift));//передаем младшую часть
	del=10; while (del--){}//задержка
	e_1;rs_1;//начинаем передачу
	del=10; while (del--){}//задержка
	e_0;rs_0;rw_0;//заканчиваем
	GPIO_Write(port,(0x00)<<lcd_shift);
}

void Init_lcd()//инициализация дисплея
{
	Init_pin_out();//настроить порты на выход

	del=72000; while (del--){}//задержка
	Lcd_write_cmd(Function_set);//включить 4 битный режим
	del=72000; while (del--){}//задержка

	Lcd_write_cmd(Display_on_off_control);//включить дисплей, выключить курсор и мигание
	del=72000; while (del--){}//задержка

	Lcd_write_cmd(Display_clear);//очистка экрана
	del=72000; while (del--){}//задержка

    Lcd_write_cmd(Entry_mode_set);//выбрать режим установки
}

void Lcd_clear()
{
	Lcd_write_cmd(Display_clear);//передаем команду очистки дисплея
}

void Return_home()
{
	Lcd_write_cmd(0b0000001);//команда вернуться в начало дисплея
}

void Lcd_definechar(const uint8_t *pc,uint8_t char_code)//создание собственного символа
{
	uint8_t a;//храним здесь код символа
	uint16_t i;//проходимся битам
	a=(char_code<<3)|0x40;//записываем код символа
	for (i=0; i<8; i++)
	{
		Lcd_write_cmd(a++);//передаем код в память
		Lcd_write_data(pc[i]);//запсиываем данные
	}
}
