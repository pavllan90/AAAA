/*
 * Feedback.c
 *
 * Created: 10.04.2020 0:54:04
 * Author : Gubanov
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int adc_value;
unsigned char port = 0, counter = 0;
unsigned int motor1, motor2, motor3, motor4, motor5;
unsigned int motor1_b, motor2_b, motor3_b, motor4_b, motor5_b;

void port_init(void)
{
	DDRD=0xFF;
	PORTD=0x00;
}

void ADC_init(void)
{
	ADCSRA |= (1<<ADEN)						//���������� ������������� ���
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);		//�������� 128 = 64 ���
	ADMUX |= (0<<REFS1)|(1<<REFS0);			//������� �������� ����������
}

unsigned int ADC_convert(void)
{
	ADCSRA |= (1<<ADSC);					//�������� ��������������
	while((ADCSRA & (1<<ADSC)));			//�������� ����������� �� �������-�������� ��������������
	return (unsigned int) ADC;
}

void Timer_init(void)
{
	TCCR1A |= (0<<WGM11)|(1<<WGM10);		//����� ������ ������ �������
	TCCR1B |= (0<<WGM13)|(1<<WGM12);		//����� ������ ������ �������
	TCCR0 |= (1<<CS00);						//��������� �������
	TCNT0 = 0x00;							//������������� �������� �������
	
	TIMSK = 0x01;							//�������� ���������� �� ������������ �������
	sei();									//���������� ���������� ����������
}

int main(void)
{
	port_init();							//�������������� �����
	ADC_init();								//�������������� ���
	Timer_init();							//������������� �������
	
	while(1)
	{
		if (port == 5) {
			port = 0;
		}
		switch (port) {
			case 0: ADMUX = 0b01000000;		//����� �������� ������
			adc_value = ADC_convert();		//����� ��������������
			motor1 = adc_value / 4;
			if (motor1 == 256) motor1--;
			break;
			case 1: ADMUX = 0b01000001;		//����� �������� ������
			adc_value = ADC_convert();		//����� ��������������
			motor2 = adc_value / 4;
			if (motor2 == 256) motor2--;
			break;
			case 2: ADMUX = 0b01000010;		//����� �������� ������
			adc_value = ADC_convert();		//����� ��������������
			motor3 = adc_value / 4;
			if (motor3 == 256) motor3--;
			break;
			case 3: ADMUX = 0b01000011;		//����� �������� ������
			adc_value = ADC_convert();		//����� ��������������
			motor4 = adc_value / 4;
			if (motor4 == 256) motor4--;
			break;
			case 4: ADMUX = 0b01000100;		//����� �������� ������
			adc_value = ADC_convert();		//����� ��������������
			motor5 = adc_value / 4;
			if (motor5 == 256) motor5--;
			break;
		}
		port++;
	}
}

ISR (TIMER0_OVF_vect)						//��������� ���������� �� ������������ �������
{
	counter++;
	if (counter == 0) {						//������� ������������ �������
		motor1_b = motor1;					//���������� �������� �������� ������������ ���
		motor2_b = motor2;
		motor3_b = motor3;
		motor4_b = motor4;
		motor5_b = motor5;
		
		PORTD = 0b00011111;					//��������� 1 �� ���� �������� �������
	}
	
	if (motor1_b == counter) PORTD &= ~(1<<PD0);	//��������� 0 �� ������ �������� ������ �� ���������� �������� ������������ ���
	if (motor2_b == counter) PORTD &= ~(1<<PD1);
	if (motor3_b == counter) PORTD &= ~(1<<PD2);
	if (motor4_b == counter) PORTD &= ~(1<<PD3);
	if (motor5_b == counter) PORTD &= ~(1<<PD4);
}

