.include "m16def.inc" ; ���������� ��� ������ ���������
.def Temp = r16  ;������ ��� ��� ��������
.def Temp1 = r17 ;��������, �������������� ���  ��������
.def Temp2 = r18 
.def Temp3 = r19

.def Temp4 = r23
.def Temp5 = r24 
.def Temp6 = r26

.def Cnt0 = r20
.def Cnt1 = r21 ; ������� ���������� � ����� ��������� ������� ����� �������� ��� ��������� �������
.def Cnt2 = r22

.def Cnt = r28

.cseg ;����������� �������, ��� ��� ��� ����, ������������ � ������ ��������

ldi Temp, 0xff ; ���������� 1 � �������
out DDRD, Temp ; ������ ������ � �������� �� ����, ���� � �� �����
ldi Temp, 0x00 ; ���������� 0 � �������
out DDRB, Temp ; ��� ������, ������� ����� � ����� A, ���� � �� ����
ldi Temp, 0xff 
out PORTB, Temp ; ��� �������� ����������� ��������� ��� ������ 
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

	rcall Button ; �������� �������� �� ������� ������
 	
	ldi Temp, 0b11111110 ; ������ � ������� ����� �, �� ��� ����� ����������
	out PORTD, Temp ; ���������� ��������� �� ����� �0
	rcall Delay  ; �������� ��������� �������� �� 0.3 ���
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111111 
	out PORTD, Temp ;����� ��������� �� ����� �0 
	rcall Delay  ; ������ �� ����� 0.3 ���
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111101 
	out PORTD, Temp ; ���������� ��������� �� ����� �1
	rcall Delay ; �������� ��������� �������� �� 0.3 ���
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111111
	out PORTD, Temp ;����� ��������� �� ����� �1 
	rcall Delay ; ������ �� ����� 0.3 ��� 
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111011 
	out PORTD, Temp ; ���������� ��������� �� ����� �2
	rcall Delay ; �������� ��������� �������� �� 0.3 ���
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111111 
	out PORTD, Temp ;����� ��������� �� ����� �2 
	rcall Delay ; ������ �� ����� 0.3 ��� 
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11110111 
	out PORTD, Temp ; ���������� ��������� �� ����� �3
	rcall Delay ; �������� ��������� �������� �� 0.3 ���
	rcall Button ; �������� �������� �� ������� ������

	ldi Temp, 0b11111111 
	out PORTD, Temp ;����� ��������� �� ����� �3 
	rcall Delay ; ������ �� ����� 0.3 ��� 
	rcall Button ; �������� �������� �� ������� ������

	rjmp Proga 

;****************************************************
; �������� ����� � ��� ������, �������� ���������, ������ ��� ������ �� ��� � ����� ����������� �����

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
; ��������� �������� ��� ������ ������, �� �� �������� ��������
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
; ��������� �������� ��� ����� ������, �� �� �������� ��������
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
; �������� �� ������
Button:
          sbic PINB, PB2 ; ���������� ��������� ������� ���� ��� P�0 �������� PIN� = 1
		  rcall LButton ; �������� �������� � ��������� ���� �����
 
    	  sbic PINB, PB3 ; ���������� ��������� ������� ���� ��� P�1 �������� PIN� = 1
		  rcall RButton ; �������� �������� � ����������� �����

		  ret
