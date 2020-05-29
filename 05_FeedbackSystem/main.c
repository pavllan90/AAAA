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
	ADCSRA |= (1<<ADEN)						//Разрешение использования АЦП
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);		//Делитель 128 = 64 кГц
	ADMUX |= (0<<REFS1)|(1<<REFS0);			//Внешний источник напряжения
}

unsigned int ADC_convert(void)
{
	ADCSRA |= (1<<ADSC);					//Начинаем преобразование
	while((ADCSRA & (1<<ADSC)));			//Проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}

void Timer_init(void)
{
	TCCR1A |= (0<<WGM11)|(1<<WGM10);		//Выбор режима работы таймера
	TCCR1B |= (0<<WGM13)|(1<<WGM12);		//Выбор режима работы таймера
	TCCR0 |= (1<<CS00);						//Включение таймера
	TCNT0 = 0x00;							//Инициализация счетчика таймера
	
	TIMSK = 0x01;							//Включить прерывание по переполнению таймера
	sei();									//Глобальное разрешение прерываний
}

int main(void)
{
	port_init();							//Инициализируем порты
	ADC_init();								//Инициализируем АЦП
	Timer_init();							//Инициализация таймера
	
	while(1)
	{
		if (port == 5) {
			port = 0;
		}
		switch (port) {
			case 0: ADMUX = 0b01000000;		//Выбор входного канала
			adc_value = ADC_convert();		//Вызов преобразования
			motor1 = adc_value / 4;
			if (motor1 == 256) motor1--;
			break;
			case 1: ADMUX = 0b01000001;		//Выбор входного канала
			adc_value = ADC_convert();		//Вызов преобразования
			motor2 = adc_value / 4;
			if (motor2 == 256) motor2--;
			break;
			case 2: ADMUX = 0b01000010;		//Выбор входного канала
			adc_value = ADC_convert();		//Вызов преобразования
			motor3 = adc_value / 4;
			if (motor3 == 256) motor3--;
			break;
			case 3: ADMUX = 0b01000011;		//Выбор входного канала
			adc_value = ADC_convert();		//Вызов преобразования
			motor4 = adc_value / 4;
			if (motor4 == 256) motor4--;
			break;
			case 4: ADMUX = 0b01000100;		//Выбор входного канала
			adc_value = ADC_convert();		//Вызов преобразования
			motor5 = adc_value / 4;
			if (motor5 == 256) motor5--;
			break;
		}
		port++;
	}
}

ISR (TIMER0_OVF_vect)						//Обработка прерывания по переполнению таймера
{
	counter++;
	if (counter == 0) {						//Счетчик переполнения таймера
		motor1_b = motor1;					//Обновление текущего значения длительности ШИМ
		motor2_b = motor2;
		motor3_b = motor3;
		motor4_b = motor4;
		motor5_b = motor5;
		
		PORTD = 0b00011111;					//Установка 1 на всех выходных каналах
	}
	
	if (motor1_b == counter) PORTD &= ~(1<<PD0);	//Установка 0 на каждом выходном канале по достижении заданной длительности ШИМ
	if (motor2_b == counter) PORTD &= ~(1<<PD1);
	if (motor3_b == counter) PORTD &= ~(1<<PD2);
	if (motor4_b == counter) PORTD &= ~(1<<PD3);
	if (motor5_b == counter) PORTD &= ~(1<<PD4);
}

