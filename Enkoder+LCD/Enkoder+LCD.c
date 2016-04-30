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

float power(uint32_t base, uint32_t ex){
	// power of 0
	if (ex == 0){
		return 1;
		// negative exponent
		}else if( ex < 0){
		return 1 / pow(base, -ex);
		// even exponent
		}else if ((uint32_t)ex % 2 == 0){
		float half_pow = pow(base, ex/2);
		return half_pow * half_pow;
		//integer exponent
		}else{
		return base * pow(base, ex - 1);
	}
}

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

int main(){
DDRB = 0x00;
PORTB|=_BV(PB0) | _BV(PB7) | _BV(PB6) | _BV(PB5);

//PB6 - zmiana parametru
//PB5 - zmiana skoku parametru

lcd_init();
for(uint32_t i = 0; i < 5; i++){
	lcd_goto(1,0);
	lcd_puttext(PSTR("GIT GUD FAGGET"));
	lcd_goto(2,1);
	lcd_puttext(PSTR("420 BLAZE IT"));
	_delay_ms(100);
	lcd_cls();
	_delay_ms(200);
}


uint32_t amp = 0, freq = 0, testf, testa, testfj, testaj, fjump = 1000, ajump = 10;

/*
freq, amp - warto�ci czestotliwo�ci i amplitudy
test - zmienne do sprawdzania zmian w warto�ciach do test�w refresh
fjump, ajump - zmienne warto�ci kroku zmiany amplitudy / cz�stotliwo�ci
*/

bool select = 0, tests = 0;
refresh(freq, amp, fjump, ajump, select);

while(1){
	testf = freq;
	testa = amp;
	tests = select;
	
	//////////INPUT///////////
	if(!(PINB & _BV(PB6))){
		select=!select;
		_delay_ms(300);
	}
	while(!(PINB & _BV(PB6)));
	
	if(!(PINB & _BV(PB5))){ //wcisn�� i przekr�ci�
		_delay_ms(100);
		while(!(PINB & _BV(PB5)))
		{
			testfj = fjump;
			testaj = ajump;
			if(select){
				fjump *= power(10, Read2StepEncoder());
				if(fjump < 1) fjump ++;
			}else{
				ajump *= power(10, Read2StepEncoder());
				if(ajump < 1) ajump ++;
			}
			if((testfj!=fjump) | (testaj!=ajump)){
				refresh(freq, amp, fjump, ajump, select);
			}
		}
	}
	
	if(select){
		freq += (fjump * (uint32_t)Read2StepEncoder());
	}else{
		amp += (ajump * (uint32_t)Read2StepEncoder());
	}
	
	///////////REFRESH///////////
	if((freq!=testf) | (amp!=testa) | (select!=tests)){
		refresh(freq, amp, fjump, ajump, select);
	}
}
}