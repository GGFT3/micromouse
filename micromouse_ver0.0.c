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

//timer1�̃��W�X�^�ݒ�
void Inti_Timer1(void);

//ADC�̃��W�X�^�ݒ�
void Init_ADC(void);

//AD�ϊ���ch��؂�ւ��Ēl���擾���Ă����֐�(������ʃt�@�C����)
void Inti_ADC_get(void);

//LCD�\���̂��߂Ɏ擾�l�������Ƃɕ�������֐�
void digit_partition(void);

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
	 * 0: �E�O�Z���TADC����
	 * 1: �E�Z���TADC����
	 * 2: ���Z���TADC����
	 * 3: ���O�Z���TADC����
	 *
	 * 4: �E�O�Z���T��LED�z������
	 * 5: �E�Z���T��LED�z������ 
	 * 6: ���Z���T��LED�z������
	 * 7: ���O�Z���T��LED�z������
	 *
	 */
	DDRA  = 0b11110000;		//�O�̂��߁c���g���G���f�B�A���ł�
	PORTA = 0b00000000;		//ADC�Ŏg�p����ۂ̓v���A�b�v�֎~(�l���ω����邽��)
	
	/*
	 *	PORTB
	 *
	 * 0: �X�C�b�`(��)
	 * 1: �X�C�b�`(��)
	 * 2: �X�C�b�`(��)
	 * 3: �E���[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 4: �E���[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 5: �������ݗpISP�Ɏg�p(MOSI)
	 * 6: �������ݗpISP�Ɏg�p(MISO)
	 * 7: �������ݗpISP�Ɏg�p(SCK)
	 *
	 */
	DDRB  = 0b00011000;						
	PORTB = 0b00000111;
	
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
	 * 0: �E���[�^���[�G���R�[�_�̃p���X�gA�����
	 * 1: �E���[�^���[�G���R�[�_�̃p���X�gB�����
	 * 2: �����[�^���[�G���R�[�_�̃p���X�gA�����
	 * 3: �����[�^���[�G���R�[�_�̃p���X�gB�����
	 * 4: 
	 * 5: 
	 * 6: �����[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 * 7: �����[�^�[�pPWM�o��(PWM�o�͂ɂ���Ƃ��͕K��DDR��1�ɂ��邱��)
	 *
	 */
	DDRD  = 0b11000000;
	PORTD = 0b00001111;
	
	//LCD������
	lcd_init();
	
	//�^�C�}���W�X�^�ݒ�(0:�E���[�^�[PWM 1:�p������֌W�Ăяo�� 2:�����[�^�[PWM) 
	Inti_Timer0();
	Inti_Timer2();
	
	//AD�ϊ����W�X�^�ݒ�
	//Init_ADC();
	
	sei();		//���荞�݋���
	
	while(1){
		
		
		Inti_CW_right(150);
		Inti_CW_left(150);
		_delay_ms(5000);
		Inti_CCW_right(150);
		Inti_CCW_left(150);
		_delay_ms(5000);
		
		/*digit_partition();
		
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
		
		lcd_pos(1,1);*/

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

	PORTA = 0b00000001;			//LED(ch0)���s
	
	ADMUX = 0b00100000;			//���͂�ch0�ɐ؂�ւ�
	_delay_ms(5);				//�؂�ւ������肷��܂őҋ@

	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	// �ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	Left_cnt = ADCH;			//�l���m��

	PORTA = 0b00000000;			//���`���[�W

	//--------------------------------------------------------------------------------------------------------
	//AD_ch1
	
	PORTA = 0b00000010;			//LED(ch1)����
	
	ADMUX = 0b00100001;			//���͂�ch1�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@
	
	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	//�ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	LeftFront_cnt = ADCH;		//�l���m��

	PORTA = 0b00000000;			//���`���[�W
	
	//--------------------------------------------------------------------------------------------------------
	//AD_ch2
	
	PORTA = 0b00000100;			//LED(ch2)����
	
	ADMUX = 0b00100010;			//���͂�ch2�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@

	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	//�ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�
	
	RightFront_cnt = ADCH;		//�l���m��

	PORTA = 0b00000000;			//���`���[�W
	
	//-------------------------------------------------------------------------------------------------------
	//AD_ch3
	
	PORTA = 0b00001000;			//LED(ch3)����
	
	ADMUX = 0b00100011;			//���͂�ch3�ɐ؂�ւ�
	_delay_us(5);				//�؂�ւ������肷��܂őҋ@
	
	ADCSRA = 0b11000111;		//AD�ϊ��X�^�[�g		#6 = 1 �ɂ���ƕϊ����X�^�[�g����
	while(ADCSRA & 0b01000000);	// �ϊ����I������܂ő҂�	�ϊ����ʂ��I���Ǝ����I��#6 = 0�ɂȂ�

	Right_cnt = ADCH;			//�l���m��

	PORTA = 0b00000000;			//���`���[�W

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
	ADMUX = 0b00100000;
	
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
	 *		#4 = 0
	 *
	 *	3: AD�ϊ��������荞�݋���
	 *		���荞�݂��g�p����ꍇ��'1'�ɂ��Ă���
	 *		#3 = 0
	 *
	 *	2,1,0: AD�ϊ��N���b�N�I��
	 *		ADC�͕ϊ��X�s�[�h�𑁂����������10�r�b�g����������
	 *		�@�\���Ȃ��̂�50kHz�`200KHz�̃N���b�N���g���ɐݒ肷��
	 *		ATmega88P�̓���N���b�N��20MHz�Ȃ̂ŁA
	 *		20M/128 ==> 156kHz�Ƃ���@������/��128
	 * 		#2 = 1, #1 = 1, #0 = 1
	 */
	ADCSRA = 0b10000110;
	
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
	 * 7,6: ���U�[�u�r�b�g
	 *		#7 = 0, #6 = 0
	 *
	 * 5,4,3,2,1,0: �f�W�^�����͋֎~
	 *		�����AD�ϊ��݂̂Ŏg�p����̂�
	 *		���ׂ�1�ɂ���
	 */
	DIDR0 = 0b00001111;
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
	//         20MHz/1024 ==> ��19kHz
	//         #2 = 1, #1 = 0, #0 = 1
	TCCR1B = 0b0000101;
	
	//TCNT1(Timer Counter1)
	//		�^�C�}�J�E���^(8bit)�ɒ��ڃA�N�Z�X�ł���
	//		�����l�������
	TCNT1 = 0;
	
	
	//OCR1A(Timer Counter1 Output Compare A Register)
	//      ���R���y�A�}�b�`A�������邩��ݒ肷��(8bit)
	//		�����1�b�J�E���g������
	//      1�N���b�N��19kHz(5*10^-5) �����100ms������B
	//		100m/5*10^-5 = ��
	OCR1A = 5;
	
	//OCR1B(Timer Counter1 Output Compare B Register)
	//		���R���y�A�}�b�`B�������邩��ݒ肷��(8bit)
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

