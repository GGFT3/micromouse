/*
 * micromouse_ver0.c
 *
 * Created: 2014/04/07 21:16:13
 *  Author: UEKI
 */ 


#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "avr_lcd.h"
#include "avr_moter.h"
#include "avr_adc.h"

//�e�X�C�b�`�̃e�X�g
void switch_test(void);

//�e�Z���T�̒l��LCD�ɕ\��
void Print_ADC(void);

//�e���[�^���[�G���R�[�_�̃J�E���g����LCD�ɕ\��
void print_RotaryEncorder(void);

//�e�Z���T�̒l�ƃ��[�^���[�G���R�[�_�̃J�E���g���𓯎���LCD�ɕ\��
void print_all_sensor(void);

//timer1�̃��W�X�^�ݒ�
void Inti_Timer1(void);

//timer3�̃��W�X�^�ݒ�
void Inti_Timer3(void);

//LCD�\���̂��߂ɃZ���T�̎擾�l�������Ƃɕ�������֐�
void S_digit_partition(void);

//LCD�\���̂��߂Ƀ��[�^���[�G���R�[�_�̎擾�l�������Ƃɕ�������֐�
void E_digit_partition(void);

//�G���R�[�_�ݒ�
void encoder(void);

//Beep�֐�
void beep(void);

//�e�Z���T�l���i�[����ϐ�
volatile unsigned char Left_Sensor_val;
volatile unsigned char LeftFront_Sensor_val;
volatile unsigned char RightFront_Sensor_val;
volatile unsigned char Right_Sensor_val;

struct{
	
	volatile unsigned char dig100;
	volatile unsigned char dig10;
	volatile unsigned char dig1;
	
}S_Left = {0, 0, 0}, S_LeftFront = {0, 0, 0}, S_RightFront ={0, 0, 0}, S_Right ={0, 0, 0};


//���[�^���[�G���R�[�_�̒l���i�[����ϐ�
volatile unsigned int Left_RotaryEncorder_val;
volatile unsigned int Right_RotaryEncorder_val;

struct{
	
	volatile unsigned int dig10000;
	volatile unsigned int dig1000;
	volatile unsigned int dig100;
	volatile unsigned int dig10;
	volatile unsigned int dig1;
	
}E_Left = {0, 0, 0, 0}, E_Right = {0, 0, 0, 0};


ISR(TIMER1_COMPA_vect){
	
	Inti_ADC_get();
	
}

ISR(TIMER3_COMPA_vect){
	
	encoder();
	Inti_CW_right(50);
	Inti_CW_left(55);
	
}

int main(void)
{		
	cli();		//���荞�݋֎~
	
	/*
	 * �ȒP��PORT�̐���
	 *
	 * DDR_  �������W�X�^(0:���� 1:�o��)
	 * PORT_ �o�̓��W�X�^(���͂̏ꍇ 0:�v���A�b�v�֎~ 1:�v���A�b�v�L��)
	 *					 (�o�͂̏ꍇ 0:Low 1:High)
	 *
	 */
	
	
	/*
	 *	PORTA
	 *
	 * 0: �E�O�Z���T(�E)ADC����
	 * 1: �E�Z���TADC����
	 * 2: ���Z���TADC����
	 * 3: �E�O�Z���T(��)ADC����
	 *
	 * 4: ���O�Z���T��LED����
	 * 5: ���Z���T��LED���� 
	 * 6: �E�Z���T��LED����
	 * 7: �E�O�Z���T��LED����
	 *
	 */
	DDRA  = 0b11110000;
	PORTA = 0b00000000;		//ADC�Ŏg�p����ۂ̓v���A�b�v�֎~(�l���ω����邽��)
	
	/*
	 *	PORTB
	 *
	 * 0: Beep�o��
	 * 1: ��X�C�b�`
	 * 2: �X�C�b�`(INT2 �O��������)
	 * 3: �E���[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 4: �E���[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 5: �������ݗpISP�Ɏg�p(MOSI)
	 * 6: �������ݗpISP�Ɏg�p(MISO)
	 * 7: �������ݗpISP�Ɏg�p(SCK)
	 *
	 */
	DDRB  = 0b00011001;						
	PORTB = 0b11100110;
	
	/*
	 *	PORTC
	 *
	 * 0: LCD�\���p(RS�̐؂�ւ� 0:�R�}���h 1:�f�[�^)
	 * 1: LCD�\���p(E�̃t���O�ݒ�@����bit�����������LCD�Ƀf�[�^�����M�����)
	 * 2:
	 * 3:
	 * 4: LCD�\���p(�f�[�^�o�X)
	 * 5: LCD�\���p(�f�[�^�o�X)
	 * 6: LCD�\���p(�f�[�^�o�X)
	 * 7: LCD�\���p(�f�[�^�o�X)
	 *
	 */
	DDRC  = 0b11110011;					
	PORTC = 0b00000000;
	
	/*
	 *	PORTD
	 *
	 * 0: �����[�^���[�G���R�[�_�̃p���X�gA�����
	 * 1: �����[�^���[�G���R�[�_�̃p���X�gB�����
	 * 2: �E���[�^���[�G���R�[�_�̃p���X�gA�����
	 * 3: �E���[�^���[�G���R�[�_�̃p���X�gB�����
	 * 4: 
	 * 5: 
	 * 6: �����[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 7: �����[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 *
	 */
	DDRD  = 0b11000000;
	PORTD = 0b00001111;			//RE12D�̓v���A�b�v�s�v�炵�����O�̂��߃v���A�b�v�͗L����
	
	//LCD������
	lcd_init();
	
	//�^�C�}���W�X�^�ݒ�(0:�E���[�^�[PWM 1:�Z���T�p 2:�����[�^�[PWM 3:�G���R�[�_�ǂݎ��{�p������) 
	Inti_Timer0();
	Inti_Timer2();
	Inti_Timer1();
	Inti_Timer3();
	
	//AD�ϊ����W�X�^�ݒ�
	
	loop_until_bit_is_clear(PINB,PINB2);		//�X�^�[�g�X�C�b�`(�F)���������܂őҋ@
	beep();										//�u�U�[��炷
	
	sei();		//���荞�݋���
	
	while(1){
		
		print_all_sensor();
		//print_RotaryEncorder();
		//Print_ADC();
		//switch_test();
	
	}

	return 0;
}

void beep(void)
{
	PORTB = 0b11100111;
	_delay_ms(1500);
	PORTB = 0b11100110;
}

void encoder(void)
{
	static const int dir_right[] = {								
		0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0
	};
	static const int dir_left[] = {
		0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0
	};
	
	static unsigned int left, right;
	unsigned int m,n;
	
	left  = (left << 2) + (PIND & 3);
	right = (right << 2) + ((PIND & 12) >> 2);
	
	m = dir_left[left & 15];
	n = dir_right[right & 15];
	
	
	Left_RotaryEncorder_val  += m;	
	Right_RotaryEncorder_val += n;

}

//�e�Z���T�̒l�ƃ��[�^���[�G���R�[�_�̃J�E���g���𓯎���LCD�ɕ\��
void print_all_sensor(void)
{
	S_digit_partition();
	E_digit_partition();
	
	lcd_pos(0,0);
	lcd_data(0x30 + E_Left.dig10000);
	lcd_data(0x30 + E_Left.dig1000);
	lcd_data(0x30 + E_Left.dig100);
	lcd_data(0x30 + E_Left.dig10);
	lcd_data(0x30 + E_Left.dig1);
	
	lcd_pos(0,6);
	lcd_data(0x30 + E_Right.dig10000);
	lcd_data(0x30 + E_Right.dig1000);
	lcd_data(0x30 + E_Right.dig100);
	lcd_data(0x30 + E_Right.dig10);
	lcd_data(0x30 + E_Right.dig1);
	
	lcd_pos(1,0);
	lcd_data(0x30 + S_RightFront.dig100);
	lcd_data(0x30 + S_RightFront.dig10);
	lcd_data(0x30 + S_RightFront.dig1);
	
	lcd_pos(1,4);
	lcd_data(0x30 + S_LeftFront.dig100);
	lcd_data(0x30 + S_LeftFront.dig10);
	lcd_data(0x30 + S_LeftFront.dig1);
	
	lcd_pos(1,8);
	lcd_data(0x30 + S_Left.dig100);
	lcd_data(0x30 + S_Left.dig10);
	lcd_data(0x30 + S_Left.dig1);
	
	lcd_pos(1,12);
	lcd_data(0x30 + S_Right.dig100);
	lcd_data(0x30 + S_Right.dig10);
	lcd_data(0x30 + S_Right.dig1);
	
	lcd_pos(0,0);
	
}

//�e�Z���T�̒l��LCD�ɕ\��
void Print_ADC(void)
{
	S_digit_partition();
	
	lcd_str("RF  LF  L   R");
	
	lcd_pos(1,0);
	lcd_data(0x30 + S_RightFront.dig100);
	lcd_data(0x30 + S_RightFront.dig10);
	lcd_data(0x30 + S_RightFront.dig1);
	
	lcd_pos(1,4);
	lcd_data(0x30 + S_LeftFront.dig100);
	lcd_data(0x30 + S_LeftFront.dig10);
	lcd_data(0x30 + S_LeftFront.dig1);
	
	lcd_pos(1,8);
	lcd_data(0x30 + S_Left.dig100);
	lcd_data(0x30 + S_Left.dig10);
	lcd_data(0x30 + S_Left.dig1);
	
	lcd_pos(1,12);
	lcd_data(0x30 + S_Right.dig100);
	lcd_data(0x30 + S_Right.dig10);
	lcd_data(0x30 + S_Right.dig1);
	
	lcd_pos(0,0);

}

//�Z���T�l�̌����킯��(LCD�̕�����\���̂���) 
void S_digit_partition(void)
{	
	//�O(������)
	S_Left.dig1			=  Left_Sensor_val % 10;
	S_Left.dig10		= (Left_Sensor_val / 10) % 10;
	S_Left.dig100		= (Left_Sensor_val / 100) % 10; 

	//���̃Z���T
	S_LeftFront.dig1    =  LeftFront_Sensor_val % 10;
	S_LeftFront.dig10   = (LeftFront_Sensor_val / 10) % 10;
	S_LeftFront.dig100	= (LeftFront_Sensor_val/ 100) % 10;

	//�E�̃Z���T
	S_RightFront.dig1   =  RightFront_Sensor_val % 10;
	S_RightFront.dig10  = (RightFront_Sensor_val / 10) % 10;
	S_RightFront.dig100 = (RightFront_Sensor_val / 100) % 10;

	//�O(�E��)
	S_Right.dig1		=  Right_Sensor_val % 10;
	S_Right.dig10		= (Right_Sensor_val / 10) % 10;
	S_Right.dig100		= (Right_Sensor_val / 100) % 10;

}

void print_RotaryEncorder(void)
{
	E_digit_partition();
	
	lcd_str("rotary encorder");
	
	lcd_pos(1,0);
	lcd_data(0x30 + E_Left.dig10000);
	lcd_data(0x30 + E_Left.dig1000);
	lcd_data(0x30 + E_Left.dig100);
	lcd_data(0x30 + E_Left.dig10);
	lcd_data(0x30 + E_Left.dig1);
	
	lcd_pos(1,6);
	lcd_data(0x30 + E_Right.dig10000);
	lcd_data(0x30 + E_Right.dig1000);
	lcd_data(0x30 + E_Right.dig100);
	lcd_data(0x30 + E_Right.dig10);
	lcd_data(0x30 + E_Right.dig1);
	
	lcd_pos(0,0);
	
}

void E_digit_partition(void)
{
	//��
	E_Left.dig1			=  Left_RotaryEncorder_val % 10;
	E_Left.dig10		= (Left_RotaryEncorder_val / 10) % 10;
	E_Left.dig100		= (Left_RotaryEncorder_val / 100) % 10;
	E_Left.dig1000		= (Left_RotaryEncorder_val / 1000) % 10;
	E_Left.dig10000		= (Left_RotaryEncorder_val / 10000) % 10;
	
	//�E
	E_Right.dig1		=  Right_RotaryEncorder_val % 10;
	E_Right.dig10		= (Right_RotaryEncorder_val / 10) % 10;
	E_Right.dig100		= (Right_RotaryEncorder_val / 100) % 10;
	E_Right.dig1000		= (Right_RotaryEncorder_val / 1000) % 10;
	E_Right.dig10000	= (Right_RotaryEncorder_val / 10000) % 10;
	
}

void switch_test(void)
{
	/*if(bit_is_clear(PINB,PINB1)){
		lcd_clear();
		lcd_str("orange");
		lcd_pos(0,0);
	}*/
	if(bit_is_clear(PINB,PINB2)){
		
		beep();
		
	}
	else{
		lcd_pos(0,0);
		lcd_str("Please");
		lcd_pos(1,0);
		lcd_str("press the button");
		lcd_pos(0,0);
	}
	
}

/*
 *	Function Name : Inti_Timer1
 *	Tittle        : �^�C�}�[1�̃��W�X�^�ݒ�
 *	Input		  :	�Ȃ�
 *	output        :	�Ȃ�
 *	Descripution  : CTC���g���Ď�y�ɃJ�E���g����
 *					ISR(TIMER1_COMPA_vect)
 */
 
void Inti_Timer1(void)
{
	//TCCR1A(Timer Counter1 Control Register A)
	//	7,6: OC1A����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//		 #7 = 0, #6 = 0
	//
	//	5,4: OC1B����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//       #5 = 0, #4 = 0
	//
	//	3,2: ���U�[�u�r�b�g
	//       #3 = 0, #2 = 0
	//
	//	1,0: PWM�g�`�̎�ނ̐ݒ�(���L��TCCR1B�ɂ��ݒ肪�ׂ��Ă���̂Œ���)
	//		 CTC���[�h
	//       #1 = 1, #0 = 0
	TCCR1A = 0b00000010;
	
	//TCCR1B(Timer Counter1 Control register B)
	//	7,6: OC1A,OC1B �����ύX�ݒ�
	//		 ����͔�PWM���[�h���g�p����ۂɐݒ肷��@����͎g�p���Ȃ�
	//		 #7 = 0, #6 = 0
	//
	//	5,4: ���U�[�u�r�b�g
	//		 #5 = 0, #4 = 0
	//
	//	3  : PWM�g�`�̎�ނ̐ݒ�(��L�ɏq�ׂ��ݒ�̎c��)
	//       #3 = 0
	//
	//	2,1,0: ������ݒ�
	//         ATmega1284P-AU�̓���N���b�N��20MHz(�q���[�Y�r�b�g�ŕ����ݒ��������)
	//         ������1/1024
	//         20MHz/1024 ==> ��20kHz
	//         #2 = 1, #1 = 0, #0 = 1
	TCCR1B = 0b0000101;
	
	//TCNT1(Timer Counter1)
	//		�^�C�}�J�E���^(16bit)�ɒ��ڃA�N�Z�X�ł���
	//		�����l�������
	TCNT1 = 0;
	
	
	//OCR1A(Timer Counter1 Output Compare A Register)
	//      ���R���y�A�}�b�`A�������邩��ݒ肷��(16bit)
	//
	//		�f�[�^�V�[�g��AD�ϊ��̂Ƃ��������ƁA
	//		�ϊ����Ԃ�13-260us(50k-1MHz)�Ə����Ă���B
	//		����AD�ϊ��̓���N���b�N��156kHz�Ȃ̂Ő��`�ɐ��ڂ���Ɖ��肷��Ɩ�240us�ɂȂ�B
	//
	//		AD�ϊ����Ԃ͖�240us �܂��}���`�v���N�T�̐؂�ւ����Ԃ�100us����Ă���B
	//		�����4��J�肩���肵�Ă���̂ŁA
	//		(240+100)*4 = 1360us �����ł�1360us�Ƃ���B
	//
	//		AD�ϊ�����������O�Ɋ��荞��ł��Ӗ����Ȃ��̂ŁA���荞�݊Ԋu��us�ȏ�ɂ���K�v������B
	//
	//      1�N���b�N��20kHz(50us)�ɐݒ肵�Ă���̂ŁA
	//		3000us/50us = 60�ƂȂ�B
	//
	OCR1A = 2000;
	
	//OCR1B(Timer Counter1 Output Compare B Register)
	//		���R���y�A�}�b�`B�������邩��ݒ肷��(16bit)
	//		����͎g�p���Ȃ��B
	OCR1B = 0;

	//TIMSK1(Timer Counter 1 Interrupt Mask Register)
	//		�^�C�}���荞�݂������邽�߂̃��W�X�^
	//	7,6,5,4,3: ���U�[�u�r�b�g
	//		#7-3 = 0
	//
	//  2 : B��r�̋���
	//		�g�p���Ȃ��̂�
	//		#2 = 0
	//
	//  1 : A��r�̋���
	//		�g�p����̂�
	//		#1 = 1
	//
	//	0 : �R�ꊄ�荞�݋���
	//		�g�p���Ȃ��̂�
	//		#0 = 0
	TIMSK1 = 0b00000010;

}

void Inti_Timer3(void)
{
	//TCCR3A(Timer Counter3 Control Register A)
	//	7,6: OC3A����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//			#7 = 0, #6 = 0
	//
	//	5,4: OC3B����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//			#5 = 0, #4 = 0
	//
	//	3,2: ���U�[�u�r�b�g
	//			#3 = 0, #2 = 0
	//
	//	1,0: PWM�g�`�̎�ނ̐ݒ�(���L��TCCR3B�ɂ��ݒ肪�ׂ��Ă���̂Œ���)
	//			CTC���[�h
	//			#1 = 1, #0 = 0
	TCCR3A = 0b00000010;
	
	//TCCR3B(Timer Counter3 Control register B)
	//	7,6: OC1A,OC1B �����ύX�ݒ�
	//		 ����͔�PWM���[�h���g�p����ۂɐݒ肷��@����͎g�p���Ȃ�
	//			#7 = 0, #6 = 0
	//
	//	5,4: ���U�[�u�r�b�g
	//			#5 = 0, #4 = 0
	//
	//	3  : PWM�g�`�̎�ނ̐ݒ�(��L�ɏq�ׂ��ݒ�̎c��)
	//			#3 = 0
	//
	//	2,1,0: ������ݒ�
	//			ATmega1284P-AU�̓���N���b�N��20MHz(�q���[�Y�r�b�g�ŕ����ݒ��������)
	//			���[�^���[�G���R�[�_�̉�]��ǂނ̂ŁA�J�E���g���[�g���T���v�����O���g�������A
	//			�傫���Ȃ��Ă͂����Ȃ��̂ō���̃T���v�����O���g����100kHz(10us)�Ƃ���
	//			20MHz/64 ==> ��312.5kHz(3.2us)
	//			#2 = 0, #1 = 1, #0 = 1
	TCCR3B = 0b0000010;
	
	//TCNT3(Timer Counter3)
	//			�^�C�}�J�E���^(16bit)�ɒ��ڃA�N�Z�X�ł���
	//			�����l�������
	TCNT3 = 0;
	
	
	//OCR3A(Timer Counter3 Output Compare A Register)
	//			���R���y�A�}�b�`A�������邩��ݒ肷��(16bit)
	//			�T���v�����O���g����100kHz(10us)�ɂ������̂�
	//			10/3.2 = 3.125 �����ł͖�4�Ƃ���
	//			
	OCR3A = 10;
	
	//OCR3B(Timer Counter3 Output Compare B Register)
	//			���R���y�A�}�b�`B�������邩��ݒ肷��(16bit)
	//			����͎g�p���Ȃ��B
	OCR3B = 0;

	//TIMSK3(Timer Counter 3 Interrupt Mask Register)
	//		�^�C�}���荞�݂������邽�߂̃��W�X�^
	//	7,6,5,4,3: ���U�[�u�r�b�g
	//		#7-3 = 0
	//
	//  2 : B��r�̋���
	//		�g�p���Ȃ��̂�
	//		#2 = 0
	//
	//  1 : A��r�̋���
	//		�g�p����̂�
	//		#1 = 1
	//
	//	0 : �R�ꊄ�荞�݋���
	//		�g�p���Ȃ��̂�
	//		#0 = 0
	TIMSK3 = 0b00000010;

}