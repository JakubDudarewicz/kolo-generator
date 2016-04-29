/*
 * LCDAlfaAnim.c
 *
 * Created: 2014-08-15 11:03:19
 *  Author: tmf
 */ 

#include <stdbool.h>
#include <stdint.h>
#include "defines.h"
#include "hd44780.h"
#include "lcd.h"
#include "enkoder.c"
#include <avr\pgmspace.h>
#include <util\delay.h>
#include <stdlib.h>

void refresh(uint32_t freq, uint32_t amp, uint32_t fjump, uint32_t ajump, bool select){
	char str[8];
	lcd_cls();
	lcd_goto(0,0);
	if(select){
		lcd_puttext(PSTR("FREQ:"));
		itoa(freq, str, 10);
	}
	else{
		lcd_puttext(PSTR("AMP:"));
		itoa(amp, str, 10);
	}
	lcd_goto(5,0);
	lcd_puttext(str);
	lcd_goto(14,0);
	if(select){
		lcd_puttext(PSTR("HZ"));
		itoa(fjump, str, 10);
	}
	else{
		lcd_puttext(PSTR("A"));
		itoa(ajump, str, 10);
	}
	lcd_goto(0,1);
	lcd_puttext(PSTR("JUMP"));
	lcd_goto(5,1);
	lcd_puttext(str);
}

int main()
{
DDRB = 0x00;
PORTB|=_BV(PB0) | _BV(PB7) | _BV(PB6);

lcd_init();
for(uint8_t i = 0; i < 5; i++){
	lcd_goto(1,0);
	lcd_puttext(PSTR("GIT GUD FAGGET"));
	lcd_goto(2,1);
	lcd_puttext(PSTR("420 BLAZE IT"));
	_delay_ms(100);
	lcd_cls();
	_delay_ms(100);
}


uint32_t amp = 0, testf, testa, fjump = 1000, ajump = 10;
bool select = 0, tests = 0;
uint32_t freq;
for(uint8_t i = 0; i < 6; i++)
	freq = 0;
refresh(freq, amp, fjump, ajump, select);
while(1){
	testf = freq;
	testa = amp;
	tests = select;
	
	//////////PRZYCISK///////////
	if(!(PINB & _BV(PB6))){
		select=!select;
		_delay_ms(100);
	}
	while(!(PINB & _BV(PB6)));
	
	////////////ENKODER//////////
	if(select){
		freq += (fjump * Read2StepEncoder());
	}else{
		amp += (ajump * Read2StepEncoder());
	}
	
	///////////REFRESH///////////
	if((freq!=testf) | (amp!=testa) | (select!=tests)){
		refresh(freq, amp, fjump, ajump, select);
	}
}
}