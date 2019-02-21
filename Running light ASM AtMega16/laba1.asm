.include "m16def.inc" ; библиотека для нашего микроконт
.def Temp = r16  ;просто даю имя регистру
.def Temp1 = r17 ;регистры, использующиеся для  задержки
.def Temp2 = r18 
.def Temp3 = r19

.def Temp4 = r23
.def Temp5 = r24 
.def Temp6 = r26

.def Cnt0 = r20
.def Cnt1 = r21 ; создаем переменную с нашим значением которое будем изменять для изменения времени
.def Cnt2 = r22

.def Cnt = r28

.cseg ;программный сегмент, все что тут есть, записывается в память программ

ldi Temp, 0xff ; записываем 1 в регистр
out DDRD, Temp ; подаем данные в регистре на порт, порт В на выход
ldi Temp, 0x00 ; записываем 0 в регистр
out DDRB, Temp ; для кнопки, которая подкл к порту A, порт А на вход
ldi Temp, 0xff 
out PORTB, Temp ; для подтяжки внутреннего резистора для кнопки 
ldi Cnt0, 13 
ldi Cnt1, 45 
ldi Cnt2, 217 
ldi Temp4, 12 
ldi Temp5, 45 
ldi Temp,HIGH(RamEnd)
out SPH,Temp
ldi Temp,Low(RamEnd)
out SPL,Temp

Proga: 

	rcall Button ; вызываем проверку на нажатие кнопки
 	
	ldi Temp, 0b11111110 ; работа с ножками порта В, на них висят светодиоды
	out PORTD, Temp ; загорается светодиод на ножке В0
	rcall Delay  ; вызываем процедуру задержки на 0.3 сек
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111111 
	out PORTD, Temp ;гасим светодиод на ножке В0 
	rcall Delay  ; ничего не горит 0.3 сек
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111101 
	out PORTD, Temp ; загорается светодиод на ножке В1
	rcall Delay ; вызываем процедуру задержки на 0.3 сек
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111111
	out PORTD, Temp ;гасим светодиод на ножке В1 
	rcall Delay ; ничего не горит 0.3 сек 
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111011 
	out PORTD, Temp ; загорается светодиод на ножке В2
	rcall Delay ; вызываем процедуру задержки на 0.3 сек
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111111 
	out PORTD, Temp ;гасим светодиод на ножке В2 
	rcall Delay ; ничего не горит 0.3 сек 
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11110111 
	out PORTD, Temp ; загорается светодиод на ножке В3
	rcall Delay ; вызываем процедуру задержки на 0.3 сек
	rcall Button ; вызываем проверку на нажатие кнопки

	ldi Temp, 0b11111111 
	out PORTD, Temp ;гасим светодиод на ножке В3 
	rcall Delay ; ничего не горит 0.3 сек 
	rcall Button ; вызываем проверку на нажатие кнопки

	rjmp Proga 

;****************************************************
; Задержка общая и для кнопок, работают одинаково, только для кнопок мы еще в конце увеличиваем время

Delay:    out PortD,Temp

          mov Temp1, Cnt2 
          mov Temp2, Cnt1
		  mov Temp3, Cnt0

Loop:     dec Temp1
          brne Loop

      	  dec Temp2
          brne Loop

          dec Temp3
          brne Loop


		  ret	

;****************************************************
; Небольшая задержка для правой кнопки, да бы избежать дребезга
RButton:
          ldi Temp1,0
          ldi Temp2,0

r_but:
          dec Temp1
          brne r_but

          dec Temp2
          brne r_but

		  add Cnt0, Temp4
		  cpi Cnt0, 61
		  brlo T4
		  ldi Cnt0, 61
		  
T4:       	  	  	
		  add Cnt1, Temp5
		  cpi Cnt1, 225
		  brlo T5
		  ldi Cnt1, 225				

T5:       
		  inc Cnt
		  cpi Cnt, 5
		  breq cnt_1
		  cpi Cnt, 1
		  breq T6_1
		  cpi Cnt, 2
		  breq T6_2
		  cpi Cnt, 3
		  breq T6_1
		  cpi Cnt, 4
		  breq T6_2

T6_1:
		  ldi Temp6, 37
		  sub Cnt2, Temp6
		  cpi Cnt2, 65
		  brge exit
		  ldi Cnt2, 65
		  jmp exit

T6_2:
		  ldi Temp6, 39
		  sub Cnt2, Temp6
		  cpi Cnt2, 65
		  brge exit
		  ldi Cnt2, 65
		  jmp exit

cnt_1:
		  ldi Cnt, 4

exit:
		  ret
;****************************************************
; Небольшая задержка для левой кнопки, да бы избежать дребезга
LButton:
          ldi Temp1,0
          ldi Temp2,0

l_but:
          dec Temp1
          brne l_but

          dec Temp2
          brne l_but
		  
		  sub Cnt0, Temp4
		  cpi Cnt0, 13
		  brge  T4_1
		  ldi Cnt0, 13

T4_1:
		  sub Cnt1, Temp5
		  cpi Cnt1, 45
		  brge T5_1
		  ldi Cnt1, 45

T5_1:
		  dec Cnt
		  cpi Cnt, 255
		  breq cnt_2
		  cpi Cnt, 3
		  breq T6_1_2
		  cpi Cnt, 2
		  breq T6_2_2
		  cpi Cnt, 1
		  breq T6_1_2
		  cpi Cnt, 0
		  breq T6_2_2

T6_1_2:
		  ldi Temp6, 39
		  add Cnt2, Temp6
		  cpi Cnt2, 217
		  brge exit_1
		  ldi Cnt2, 217
		  jmp exit_1

T6_2_2:
		  ldi Temp6, 37
		  add Cnt2, Temp6
		  cpi Cnt2, 217
		  brge exit_1
		  ldi Cnt2, 217
		  jmp exit_1

cnt_2:
		  ldi Cnt, 0

exit_1:
		  ret

;****************************************************
; Проверка на кнопки
Button:
          sbic PINB, PB2 ; Пропустить следующую команду если бит PА0 регистра PINА = 1
		  rcall LButton ; вызываем задержку и уменьшаем наше время
 
    	  sbic PINB, PB3 ; Пропустить следующую команду если бит PА1 регистра PINА = 1
		  rcall RButton ; вызываем задержку и увеличиваем время

		  ret
