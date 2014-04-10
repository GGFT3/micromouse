/*
 * micromouse_ver0.c
 *
 * Created: 2014/04/07 21:16:13
 *  Author: takemichi
 */ 


#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "avr_lcd.h"

void Inti_Timer(void);					//�^�C�}�̏����ݒ�
void Init_ADC(void);					//AD�ϊ��̏������ݒ�
void Inti_ADC_get(void);				//AD�ϊ���ch��؂�ւ���֐�
void digit_partition(void);				//LCD�\���̂��߂Ɏ擾�l�������Ƃɕ���

//void encoder(void);

//�e�Z���T�l���i�[����֐�
volatile unsigned char Left_cnt;
volatile unsigned char LeftFront_cnt;
volatile unsigned char RightFront_cnt;
volatile unsigned char Right_cnt;

struct{
	
	volatile unsigned char dig100;
	volatile unsigned char dig10;
	volatile unsigned char dig1;
	
}Left = {0, 0, 0}, LeftFront = {0, 0, 0}, RightFront ={0, 0, 0}, Right ={0, 0, 0};

ISR(TIMER1_COMPA_vect){
		
		Inti_ADC_get();
}

int main(void)
{		
	cli();
	
	DDRA  = 0xF0;
	PORTA = 0x00;
	DDRC  = 0xFF;					
	PORTC = 0x00;
	DDRD  = 0xFF;						//PORTD���o�͂ɐݒ�
	PORTD = 0x00;						
	DDRB  = 0xFF;						//PORTB���o�͂ɐݒ�						
	PORTB = 0x00;						//�R���f���T�`���[�W�J�n
	
	lcd_init();							//LCD������
	Inti_Timer();						//�^�C�}�[���W�X�^�ݒ�
	Init_ADC();							//AD�ϊ����W�X�^�ݒ�
	
	sei();
	
	while(1){
		
		digit_partition();
		
		lcd_str("left front right");
		
		lcd_pos(2,1);
		lcd_data(0x30 + Left.dig100);
		lcd_data(0x30 + Left.dig10);
		lcd_data(0x30 + Left.dig1);
		
		lcd_pos(2,5);
		lcd_data(0x30 + LeftFront.dig100);
		lcd_data(0x30 + LeftFront.dig10);
		lcd_data(0x30 + LeftFront.dig1);
		
		lcd_pos(2,9);
		lcd_data(0x30 + RightFront.dig100);
		lcd_data(0x30 + RightFront.dig10);
		lcd_data(0x30 + RightFront.dig1);
		
		lcd_pos(2,13);
		lcd_data(0x30 + Right.dig100);
		lcd_data(0x30 + Right.dig10);
		lcd_data(0x30 + Right.dig1);
		
		lcd_pos(1,1);

	}

	return 0;
}

/*
void encoder(void)
{
	static const int dir[] = {								
		0, 1, -1, 0, -1, 0, 0, 1, 1,
		0, 0, -1, 0, -1, 1, 0
	};
	static int i;
	int n;
	
	i = (i << 2) + (PINB & 3);
	n = dir[i & 15];
	cnt += n;	//cnt�̒l��LCD�ɕ\��
}
*/

//�Z���T�l�̌����킯��(LCD�̕�����\���̂���) 
void digit_partition(void)
{	
	//�O(������)
	Left.dig1    =  Left_cnt % 10;
	Left.dig10   = (Left_cnt / 10) % 10;
	Left.dig100  = (Left_cnt / 100) % 10; 

	//���̃Z���T
	LeftFront.dig1    =  LeftFront_cnt % 10;
	LeftFront.dig10   = (LeftFront_cnt / 10) % 10;
	LeftFront.dig100  = (LeftFront_cnt / 100) % 10;

	//�E�̃Z���T
	RightFront.dig1    =  RightFront_cnt % 10;
	RightFront.dig10   = (RightFront_cnt / 10) % 10;
	RightFront.dig100  = (RightFront_cnt / 100) % 10;

	//�O(�E��)
	Right.dig1    =  Right_cnt % 10;
	Right.dig10   = (Right_cnt / 10) % 10;
	Right.dig100  = (Right_cnt / 100) % 10;

}

void Inti_ADC_get(void)
{
	
//--------------------------------------------------------------------------------------------------------
	//AD_ch0

	PORTB = 0b00000001;			//LED(ch0)���s
	
	ADMUX = 0b00100000;			//���͂�ch0�ɐ؂�ւ�
	_delay_ms(5);				//�؂�ւ������肷��܂őҋ@

	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	// �ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	Left_cnt = ADCH;			//�l���m��

	PORTB = 0b00000000;			//���`���[�W

	//--------------------------------------------------------------------------------------------------------
	//AD_ch1
	
	PORTB = 0b00000010;			//LED(ch1)����
	
	ADMUX = 0b00100001;			//���͂�ch1�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@
	
	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	//�ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	LeftFront_cnt = ADCH;		//�l���m��

	PORTB = 0b00000000;			//���`���[�W
	
	//--------------------------------------------------------------------------------------------------------
	//AD_ch2
	
	PORTB = 0b00000100;			//LED(ch2)����
	
	ADMUX = 0b00100010;			//���͂�ch2�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@

	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	//�ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	RightFront_cnt = ADCH;		//�l���m��

	PORTB = 0b00000000;			//���`���[�W
	
	//-------------------------------------------------------------------------------------------------------
	//AD_ch3
	
	PORTB = 0b00001000;			//LED(ch3)����
	
	ADMUX = 0b00100011;			//���͂�ch3�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@
	
	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	// �ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�

	Right_cnt = ADCH;			//�l���m��

	PORTB = 0b00000000;			//���`���[�W

}


//***********************************************************************************//
//	Function Name :	Init_ADC														*//
//	Titlle        : AD�ϊ��p���W�X�^�̐ݒ�											*//
//  Input         :	ADC0, ADC1, ADC2, ADC3, ADC4, ADC5								*//
//  output        :																	*//
//	Description   :	�ʏ퓮�샂�[�h													*//
//***********************************************************************************//
void Init_ADC(void)
{
	//ADMUX(ADC Multiplexer Selct Register)
	//	7: ���U�[�u�r�b�g
	//		#7 = 0
	
	//	6: ��d���I��
	//		��d���Ƃ���Vcc(5V)���g�p����
	//		#6 = 0
	
	//	5: �ϊ����ʂ��E�񂹂ɂ��邩���񂹂ɂ��邩��ݒ肷��
	//		���񂹂ɂ���
	//		#5 = 1
	
	//	4: ���U�[�u�r�b�g
	//		#4 = 0
	
	//	3,2,1,0: AD�`�����l���I��
	//		���̃r�b�g��AD�ϊ����ɂ��Ă��ϊ������܂ł͎��s����Ȃ�
	//		�Ƃ肠����ADC0�ɐݒ�
	//		#3 = 0, #2 = 0, #1 = 0, #0 = 0
	ADMUX = 0b00100000;
	
	//ADCSRA(ADC Control and Status Register A)
	//	7: AD�ϊ�����
	//		AD�ϊ���������
	//		#7 = 1
	
	//	6: ADSC(ADC Start Conversion)	AD�ϊ��J�n
	//		�Ƃ肠�����J�n�͂܂����Ȃ�
	//		#6 = 0
	
	//	5: AD�ϊ������N������
	//		#5 = 0
	
	//	4: AD�ϊ��������荞�ݗv���t���O
	//		AD�ϊ������������ʂ̃��W�X�^���X�V�����Ƃ��̃t���O��'1'�ɂȂ�
	//		�Ƃ肠���������l����͂��Ă���
	//		#4 = 0
	
	//	3: AD�ϊ��������荞�݋���
	//		���荞�݂��g�p����ꍇ��'1'�ɂ��Ă�����
	//		#3 = 0
	
	//	2,1,0: AD�ϊ��N���b�N�I��
	//		ADC�͕ϊ��X�s�[�h�𑁂����������10�r�b�g����������
	//		�@�\���Ȃ��̂�50kHz�`200KHz�̃N���b�N���g���ɐݒ肷��
	//		ATmega88P�̓���N���b�N��20MHz�Ȃ̂ŁA
	//		20M/128 ==> 156kHz�Ƃ���@������/��128
	//		#2 = 1, #1 = 1, #0 = 1
	ADCSRA = 0b10000110;
	
	//ADCSRB(ADC Control and Status Register B)
	//	7:	���U�[�u�r�b�g
	//		#7 = 0
	
	//	6:	�悭�킩���
	//		#6 = 0
	
	//	5,4,3:	���U�[�u�r�b�g
	//		#5 = 0, #4 = 0, #3 = 0
	
	//	2,1,0:	AD�ϊ������N���v���I��
	//		�A���ϊ�����
	//		#2 = 0, #1 = 0, #0 = 0
	ADCSRB = 0b00000000;
	
	//DIDR0(Digital Input Disable Register 0)
	// 7,6: ���U�[�u�r�b�g
	//		#7 = 0, #6 = 0
	
	// 5,4,3,2,1,0: �f�W�^�����͋֎~
	//		�����AD�ϊ��݂̂Ŏg�p����̂�
	//		���ׂ�1�ɂ���
	DIDR0 = 0b00001111;
}


//***************************************************************************//
//*	Function Name : ���W�X�^�������֐�										*//
//*	Tittle        : �^�C�}�[0�̃��W�X�^�ݒ�									*//
//*	Input		  :	�Ȃ�													*//
//*	output        :	�Ȃ�												�@�@*//
//*	Descripution  : CTC���g���Ď�y�ɃJ�E���g����							*//
//*					ISR(TIMER0_COMPA_vect)									*//
//***************************************************************************//
void Inti_Timer(void)
{
	//TCCR0A(Timer Counter0 Control Register A)
	//	7,6: OC0A����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//		 #7 = 0, #6 = 0
	//
	//	5,4: OC0B����o�͂���PWM�g�̐ݒ�
	//       �����o�͂��Ȃ��̂�
	//       #5 = 0, #4 = 0
	//
	//	3,2: ���U�[�u�r�b�g
	//       #3 = 0, #2 = 0
	//
	//	1,0: PWM�g�`�̎�ނ̐ݒ�(���L��TCCR0B�ɂ��ݒ肪�ׂ��Ă���̂Œ���)
	//		 CTC���[�h
	//       #1 = 1, #0 = 0
	TCCR1A = 0b00000010;
	
	//TCCR0B(Timer Counter0 Control register B)
	//	7,6: OC0A,OC0B �����ύX�ݒ�
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
	//         ATmega88P�̓���N���b�N��20MHz(�q���[�Y�r�b�g�ŕ����ݒ��������)
	//         ������1/1024
	//         20MHz/1024 ==> ��19kHz
	//         #2 = 1, #1 = 0, #0 = 1
	TCCR1B = 0b0000101;
	
	//TCNT0(Timer Counter0)
	//		�^�C�}�J�E���^(8bit)�ɒ��ڃA�N�Z�X�ł���
	//		�����l�������
	TCNT1 = 0;
	
	
	//OCR0A(Timer Counter0 Output Compare A Register)
	//      ���R���y�A�}�b�`A�������邩��ݒ肷��(8bit)
	//		�����1�b�J�E���g������
	//      1�N���b�N��19kHz(5*10^-5) �����100ms������B
	//		100m/5*10^-5 = ��
	OCR1A = 5;
	
	//OCR0B(Timer Counter0 Output Compare B Register)
	//		���R���y�A�}�b�`B�������邩��ݒ肷��(8bit)
	//		����͎g�p���Ȃ��B
	OCR1B = 0;

	//TIMSK0(Timer Counter 0 Interrupt Mask Register)
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
