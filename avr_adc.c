/*
 * avr_adc.c
 *
 * Created: 2014/04/22 14:37:38
 *  Author: takemichi
 */ 

#include <avr/io.h>
#define F_CPU 20000000
#include <avr/interrupt.h>
#include <util/delay.h>
#include "avr_adc.h"


volatile unsigned char Left_Sensor_val = 0;
volatile unsigned char LeftFront_Sensor_val = 0;
volatile unsigned char RightFront_Sensor_val = 0;
volatile unsigned char Right_Sensor_val = 0;
volatile unsigned char adc_chanel = 0;


ISR(ADC_vect){
	
	switch(adc_chanel){
		
		case 0:
		RightFront_Sensor_val = ADCH;
		break;
		
		case 1:
		LeftFront_Sensor_val = ADCH;
		break;
		
		case 2:
		Left_Sensor_val = ADCH;
		break;
		
		case 3:
		Right_Sensor_val= ADCH;
		break;
	}
	
	PORTA = 0b00000000;			//LED������~
	
	adc_chanel++;
	
	if(adc_chanel == 4){
		adc_chanel = 0;
	}
	
}

void Inti_ADC_get(void)
{
	const unsigned char LEDPORT[] = { 0b10000000, 0b00010000, 0b00100000, 0b01000000 };
	const unsigned char MUXREG[]  = { 0b00100000, 0b00100001, 0b00100010, 0b00100011 };

	

	PORTA = LEDPORT[adc_chanel];			//LED(ch0)���s
	
	ADMUX = MUXREG[adc_chanel];			//���͂�ch0�ɐ؂�ւ�
	_delay_us(50);						//�؂�ւ������肷��܂őҋ@

	ADCSRA = 0b11001111;				//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����

	
}

/*
 *
 *	Function Name :	Init_ADC														
 *	Titlle        : AD�ϊ��p���W�X�^�̐ݒ�											
 *  Input         :	ADC0, ADC1, ADC2, ADC3, ADC4, ADC5								
 *  output        :	�Ȃ�																
 *	Description   :	�ʏ퓮�샂�[�h
 *
 */

void Init_ADC(void)
{
	/*
	 * ADMUX(ADC Multiplexer Selct Register)
	 *	
	 *	7: ���U�[�u�r�b�g
	 *		#7 = 0
	 *
	 *	6: ��d���I��
	 *		��d���Ƃ���Vcc(5V)���g�p����
	 *		#6 = 0
	 *
	 *	5: �ϊ����ʂ��E�񂹂ɂ��邩���񂹂ɂ��邩��ݒ肷��
	 *		���񂹂ɂ���
	 *		#5 = 1
	 *
	 *		4: ���U�[�u�r�b�g
	 *		#4 = 0
	 *
	 *	3,2,1,0: AD�`�����l���I��
	 *		���̃r�b�g��AD�ϊ����ɂ��Ă��ϊ������܂ł͎��s����Ȃ�
	 *		�Ƃ肠����ADC0�ɐݒ�
	 *		#3 = 0, #2 = 0, #1 = 0, #0 = 0
	 */
	ADMUX = 0b01100000;
	
	/*
	 * ADCSRA(ADC Control and Status Register A)
	 *	
	 *	7: AD�ϊ�����
	 *		AD�ϊ���������
	 *		#7 = 1
	 *
	 *	6: ADSC(ADC Start Conversion)	AD�ϊ��J�n
	 *		�Ƃ肠�����J�n�͂܂����Ȃ�
	 *		#6 = 0
	 *
	 *	5: AD�ϊ������N������
	 *		#5 = 0
	 *
	 *	4: AD�ϊ��������荞�ݗv���t���O
	 *		AD�ϊ������������ʂ̃��W�X�^���X�V�����Ƃ��̃t���O��'1'�ɂȂ�
	 *		�Ƃ肠���������l����͂��Ă���
	 *		#4 = 1
	 *
	 *	3: AD�ϊ��������荞�݋���
	 *		���荞�݂��g�p����ꍇ��'1'�ɂ��Ă���
	 *		#3 = 1
	 *
	 *	2,1,0: AD�ϊ��N���b�N�I��
	 *		ADC�͕ϊ��X�s�[�h�𑁂����������10�r�b�g����������
	 *		�@�\���Ȃ��̂�50kHz�`200KHz�̃N���b�N���g���ɐݒ肷��
	 *		ATmega88P�̓���N���b�N��20MHz�Ȃ̂ŁA
	 *		20M/128 ==> 156kHz�Ƃ���@������/��128
	 * 		#2 = 1, #1 = 1, #0 = 1
	 */
	ADCSRA = 0b10000111;
	
	/*
	 * ADCSRB(ADC Control and Status Register B)
	 *	7:	���U�[�u�r�b�g
	 *		#7 = 0
	 *
	 *	6:	�悭�킩���
	 *		#6 = 0
	 *
	 *	5,4,3:	���U�[�u�r�b�g
	 *		#5 = 0, #4 = 0, #3 = 0
	 *
	 *	2,1,0:	AD�ϊ������N���v���I��
	 *		�A���ϊ�����
	 *		#2 = 0, #1 = 0, #0 = 0
	 */
	ADCSRB = 0b00000000;
	
	/*
	 * DIDR0(Digital Input Disable Register 0)
	 *
	 * 7,6,5,4,3,2,1,0: �f�W�^�����͋֎~
	 *	
	 */
	DIDR0 = 0b00001111;
}
