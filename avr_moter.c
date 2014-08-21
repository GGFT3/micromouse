/*
 * avr_moter.c
 *
 * Created: 2014/04/18 20:20:03
 *  Author: UEKI
 */ 

/*
 * specification
 *
 * ����̃}�E�X�Ŏg�p����MD�� http://www.maizuru-ct.ac.jp/control/machida/elec/motor/index.htm ��TLP250���g�p����H�u���b�W��H�ł���B
 *
 * ��H���番����悤�ɓ���LED���z�����ݎ��ŋ쓮�����Ă��邽��duty���ʏ�Ƌt�ɂȂ�B(��: duty��100%�Ń��[�^�[��~)
 * ���i�Ɠ��l�Ɏg���������ߍ���͔g�`�o�͂̃��W�X�^��ύX���A�R���y�A�}�b�`����low���o�͂��锼�]�o�͓�����g�p�����B
 * �ڂ����̓f�[�^�V�[�g���Q�ƁB�����������������e�L�X�g�Ƃ��Ďc���Ă��������B
 *
 * �܂��T�C�g�̋쓮���@���番����悤��PWM�o�̓��[�h��GPIO��؂�ւ��Ďg�p���Ă��邽�߂��̂�����𗯈ӂ��邱�ƁB
 *
 */

#include <avr/io.h>
#define F_CPU 2000000
#include <util/delay.h>
#include "avr_moter.h"


//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Init_CW_left(unsigned char pwm)
{
	TCCR0A = 0b10100011;		//�o�͐ݒ菉����(A��B��PWM�o��)
	TCCR0A = 0b11000011;		//OC0B��ʏ�|�[�g�ɕύX
	PORTB  = 0b00010110;		//PB4��high��
	OCR0A  = pwm;
}

//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Init_CCW_left(unsigned char pwm)
{
	TCCR0A = 0b11110011;		//�o�͐ݒ菉����(A��B��PWM�o��)
	TCCR0A = 0b00110011;		//OC0A��ʏ�|�[�g�ɕύX
	PORTB  = 0b00001110;		//PB3��high��
	OCR0B  = pwm;
}


//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Init_CW_right(unsigned char pwm)
{
	TCCR2A = 0b11110011;		//�o�͐ݒ菉����(A��B��PWM�o��)
	TCCR2A = 0b00110011;		//OC0A��ʏ�|�[�g�ɕύX
	PORTD  = 0b10001111;		//PD7��high��
	OCR2B  = pwm;
}

//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Init_CCW_right(unsigned char pwm)
{
	TCCR2A = 0b11110011;		//�o�͐ݒ菉����(A��B��PWM�o��)
	TCCR2A = 0b11000011;		//OC0B��ʏ�|�[�g�ɕύX
	PORTD  = 0b01001111;		//PD6��high��
	OCR2A  = pwm;
}

/*
 *	Function Name : Init_Timer0
 *	Tittle        : �^�C�}�[0�̃��W�X�^�ݒ�
 *	Input		  :	�Ȃ�
 *	output        :	OCR0A, OCR0B
 *	Descripution  : ����PWM���[�h�g�p��PWM�g���o��
 *					
 */
 
void Init_Timer0(void)
{
	/*
	 * TCCR0A(Timer Counter0 Control Register A)
	 *
	 *	7,6: OC0A����o�͂���PWM�g�̐ݒ�
	 *       �R���y�A�}�b�`��HIGH�ABOTTOM��LOW���o��
	 *		 #7 = 1, #6 = 1
	 *
	 *	5,4: OC0B����o�͂���PWM�g�̐ݒ�
	 *       �R���y�A�}�b�`��HIGH�ABOTTOM��LOW���o��
	 *       #5 = 1, #4 = 1
	 *
	 *	3,2: ���U�[�u�r�b�g
	 *       #3 = 0, #2 = 0
	 *
	 *	1,0: PWM�g�`�̎�ނ̐ݒ�(���L��TCCR0B�ɂ��ݒ肪�ׂ��Ă���̂Œ���)
	 *		 8bit����PWM���샂�[�h
	 *       #1 = 1, #0 = 1
	 */
	TCCR0A = 0b11110011;
	
	/*
	 * TCCR0B(Timer Counter0 Control register B)
	 *
	 *	7,6: OC0A,OC0B �����ύX�ݒ�
	 *		 ����͔�PWM���[�h���g�p����ۂɐݒ肷��@����͎g�p���Ȃ�
	 *		 #7 = 0, #6 = 0
	 *
	 *	5,4: ���U�[�u�r�b�g
	 *		 #5 = 0, #4 = 0
	 *
	 *	3  : PWM�g�`�̎�ނ̐ݒ�(��L�ɏq�ׂ��ݒ�̎c��)
	 *       #3 = 0
	 *
	 *	2,1,0: ������ݒ�
	 *			ATmaga88P�̓���N���b�N��20MHz(�q���[�Y�r�b�g�ŕ����ݒ��������)
	 *			MNCT�̃T�C�g�ł͎��p��48kHz�������Ƃ��邪�A�����̊֌W��10k��78k�ɂȂ��Ă��܂��B
	 *			78k���Ɠd�����Ȃ���ɂ����Ȃ�FET�����M����̂ŁA�����͏o�邪10k�œ��������Ƃɂ���B
	 *			20MHz/0xFF(255)/8 ==> ��10kHz�@�v�Z����Datasheet���Q��
	 *			#2 = 0, #1 = 1, #0 = 0
	 */
	TCCR0B = 0b00000010;
	
	/*
	 * TCNT0(Timer Counter0)
	 *
	 *		�^�C�}�J�E���^(8bit)�ɒ��ڃA�N�Z�X�ł���
	 *		�����l�������(����͎g�p���Ȃ�)
	 */
	TCNT0 = 0b00000000;
	
	/*
	 * OCR0A(Timer Counter0 Output Compare A Register)
	 *
	 *		�R���y�A�}�b�`A�̃^�C�~���O�ݒ�(8bit)
	 *		default duty 0%
	 */
	OCR0A = 0b00000000;
	
	/*
	 * OCR0B(Timer Counter0 Output Compare B Register)
	 *		
	 *		�R���y�A�}�b�`B�̃^�C�~���O�ݒ�(8bit)
	 *		default dety 0%
	 */
	OCR0B = 0b00000000;
}

/*
 *	Function Name : Init_Timer2
 *	Tittle        : �^�C�}�[2�̃��W�X�^�ݒ�
 *	Input		  :	�Ȃ�
 *	output        :	OCR2A, OCR2B
 *	Descripution  : ����PWM���[�h���g�p��PWM�g���o�͂���
 *
 */
 
void Init_Timer2(void)
{
	/*
	 * TCCR2A(Timer Counter2 Control Register A)
	 *
	 *	7,6: OC0A����o�͂���PWM�g�̐ݒ�
	 *       �R���y�A�}�b�`��HIGH�ABOTTOM��LOW���o��
	 *		 #7 = 1, #6 = 1
	 *
	 *	5,4: OC0B����o�͂���PWM�g�̐ݒ�
	 *       �R���y�A�}�b�`��HIGH�ABOTTOM��LOW���o��
	 *       #5 = 1, #4 = 1
	 *
	 *	3,2: ���U�[�u�r�b�g
	 *       #3 = 0, #2 = 0
	 *
	 *	1,0: PWM�g�`�̎�ނ̐ݒ�(���L��TCCR0B�ɂ��ݒ肪�ׂ��Ă���̂Œ���)
	 *		 8bit����PWM���샂�[�h
	 *       #1 = 1, #0 = 1
	 */
	TCCR2A = 0b11110011;
	
	/*
	 * TCCR2B(Timer Counter2 Control register B)
	 *
	 *	7,6: OC0A,OC0B �����ύX�ݒ�
	 *		 ����͔�PWM���[�h���g�p����ۂɐݒ肷��@����͎g�p���Ȃ�
	 *		 #7 = 0, #6 = 0
	 *
	 *	5,4: ���U�[�u�r�b�g
	 *		 #5 = 0, #4 = 0
	 *
	 *	3  : PWM�g�`�̎�ނ̐ݒ�(��L�ɏq�ׂ��ݒ�̎c��)
	 *       #3 = 0
	 *
	 *	2,1,0: ������ݒ�
	 *         ATmaga88P�̓���N���b�N��20MHz(�q���[�Y�r�b�g�ŕ����ݒ��������)
	 *         MNCT�̃T�C�g�ł͎��p��48kHz�������Ƃ��邪�A�����̊֌W��10k��78k�ɂȂ��Ă��܂��B
	 *		   78k���Ɠd�����Ȃ���ɂ����Ȃ�FET�����M����̂ŁA�����͏o�邪10k�œ��������Ƃɂ���B
	 *         20MHz/0xFF(255)/8 ==> ��10kHz�@�v�Z����Datasheet���Q��
	 *         #2 = 0, #1 = 1, #0 = 0
	 */
	TCCR2B = 0b00000010;
	
	/*
	 * TCNT2(Timer Counter2)
	 *
	 *		�^�C�}�J�E���^(8bit)�ɒ��ڃA�N�Z�X�ł���
	 *		�����l�������(����͎g�p���Ȃ�)
	 */
	TCNT2 = 0b00000000;
	
	/*
	 * OCR2A(Timer Counter2 Output Compare A Register)
	 *
	 *      ���R���y�A�}�b�`A�������邩��ݒ肷��(8bit)
	 *		default duty 0%
	 */
	OCR2A = 0b00000000;
	
	/*
	 * OCR2B(Timer Counter2 Output Compare B Register)
	 *		
	 *		���R���y�A�}�b�`B�������邩��ݒ肷��(8bit)
	 *		default dety 0%
	 */
	OCR2B = 0b00000000;
}
