/*
 * avr_moter.h
 *
 * Created: 2014/04/18 20:21:16
 *  Author: UEKI
 */ 


#ifndef AVR_MOTER_H_
#define AVR_MOTER_H_

//�^�C�}�[0�̃��W�X�^�ݒ�(8bit����PWM���[�h)
void Inti_Timer0(void);

//�^�C�}�[2�̃��W�X�^�ݒ�(8bit����PWM���[�h)
void Inti_Timer2(void);

//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Inti_CW_right(unsigned char pwm);

//�Ƃ肠�����̉�]�e�X�g�p�֐�
void Inti_CCW_right(unsigned char pwm);

void Inti_CW_left(unsigned char pwm);

void Inti_CCW_left(unsigned char pwm);

#endif /* AVR_MOTER_H_ */