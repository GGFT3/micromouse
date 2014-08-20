/*
 * avr_lcd.h
 *
 * Created: 2014/04/04 16:27:33
 *  Author: UEKI
 */ 


#ifndef AVR_LCD_H_
#define AVR_LCD_H_

//LCD�R���g���[���̏����ݒ�
void lcd_init(void);

//LCD���M�ݒ�
void lcd_out(int code, int rs);

//�R�}���h���M�֐�
void lcd_cmd(int cmd);

//�f�[�^���M�֐�
void lcd_data(int asci);

//�\���ʒu�����֐�
void lcd_pos(int line, int col);

//�����񑗐M�֐�
void lcd_str(char *str);

//�N���A�֐�
void lcd_clear(void);


#endif /* AVR_LCD_H_ */