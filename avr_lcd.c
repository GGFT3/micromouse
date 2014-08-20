/*
 * avr_lcd.c
 *
 * Created: 2014/04/04 16:51:06
 *  Author: UEKI
 */ 

/*
 * specification
 *
 * �ڂ����͌��Hp����̗납��̓d�q�H���55-58����Q�Ƃ��ꂽ���B
 * http://www.nicovideo.jp/watch/sm15413302 (�납��̓d�q�H���55�񃊃��N)
 * LCD�̃f�[�^�V�[�g�������������ɗ��B
 *
 * �̗p����LCD
 * http://akizukidenshi.com/catalog/g/gP-02985/ (�H���d�q�����N)
 *
 *
 * PORT�̌q����
 * 
 * 0: RS
 * 1: E
 * 2
 * 3
 * 4: DB4
 * 5: DB5
 * 6: DB6
 * 7: DB7
 *
 * PORT��ߖ񂷂邽�߂�8bit�]�����[�h�ł͂Ȃ�4bit�]�����[�h���g�p���Ă���B
 * �g��Ȃ�DB0-3�͂ǂ��Ƃ��ڑ����Ȃ��悤�ɂ��Ă�������(�������Ă���)
 *
 */


#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include "avr_lcd.h"


//������S��������
void lcd_clear(void)
{
	lcd_cmd(0x01);		//�N���A�R�}���h
}

//������𑗐M����
void lcd_str(char *str)
{
	while(*str != '\0'){
		lcd_data(*str);
		str++;
	}
}

/*�����̈ʒu�����߂�(���W��0�I���W��)
 *
 *	LCD��16*2
 *	(��,�s)�ŕ����̏ꏊ�����肵�Ă���
 *
 *	������̍Ō�͈ʒu��(0,0)�ɖ߂��Ă�����(���[�v���邽�߂�)	
 *
 */

void lcd_pos(int line, int col)
{
	if(line == 0){
		lcd_cmd(0x80 + col);
	}
	else if(line == 1){
		lcd_cmd(0xC0 + col);
	}
}

//�����ݒ�
void lcd_init(void)
{
	_delay_ms(15);
	lcd_out(0x30, 0);
	_delay_ms(5);
	lcd_out(0x30, 0);
	_delay_ms(1);
	lcd_out(0x30, 0);
	_delay_ms(1);
	lcd_out(0x20, 0);
	_delay_ms(1);
	lcd_cmd(0x28);
	lcd_cmd(0x08);
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x02);
	lcd_cmd(0x01);
	
}

/* �R�}���h�Ȃ̂��f�[�^�����肵LCD�ɑ��M
 *
 *	1�ڂ̈����Œl���A
 *	2�ڂ̈����ł��̒l���f�[�^���R�}���h����I������B(0:�R�}���h 1:�f�[�^)
 *
 */

void lcd_out(int code, int rs)
{
	PORTC = (code & 0xF0) | (PORTC & 0x0F);		//PD2,3���g���ꍇ�ɒl���ς��Ȃ��悤�ɂ��邽�߂̏��u
	
	if(rs == 0){
		PORTC = code & 0b11111110;				//�R�}���h�𑗐M����
	}
	else{
		PORTC = code | 0b00000001;				//�f�[�^�𑗐M����
	}
	
	_delay_ms(1);
	PORTC = PORTC | 0b00000010;					//E�̃t���O�𗧂Ă�
	_delay_ms(1);
	PORTC = PORTC & 0b11111101;					//E�̃t���O��߂�
	
}


//�R�}���h�𑗐M����
void lcd_cmd(int cmd)							//4bit�����M
{
	lcd_out(cmd, 0);
	lcd_out(cmd << 4, 0);
	_delay_ms(2);
}

//�f�[�^�𑗐M����
void lcd_data(int asci)							//4bit�����M
{
	lcd_out(asci, 1);
	lcd_out(asci << 4, 1);
	_delay_ms(0.05);
}