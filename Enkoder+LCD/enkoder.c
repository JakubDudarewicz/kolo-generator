/*
 * Enkoder.c
 *
 * Created: 2014-08-13 17:15:32
 *  Author: tmf
 */ 

#define F_CPU 1000000L

#include <avr\io.h>
#include <avr/interrupt.h>
#include <util\delay.h>

#define GLUE(a, b)     a##b

int8_t enc_delta;

static inline void ReadEncoder();
static inline int8_t ReadStepEncoder();
static inline int8_t Read2StepEncoder();

void ReadEncoder()
{
	static int8_t last;
	static uint8_t laststate;
	static uint8_t counters[2];	//Tablica zawieraj¹ca liczniki
	int8_t newpos, diff;
	
	uint8_t state=PINB;
	if(((state^laststate) & _BV(PB0)) && (counters[0]==0))
	{
		counters[0]=200;
		laststate&=(~_BV(PB0));
		laststate|=(state & _BV(PB0));
	}

	if(((state^laststate) & _BV(PB7)) && (counters[1]==0))
	{
		counters[1]=200;
		laststate&=(~_BV(PB7));
		laststate|=(state & _BV(PB7));
	}
	
	for(uint8_t c=0;c<sizeof(counters)/sizeof(counters[0]);c++)
	    if(counters[c])	counters[c]--;

	newpos=0;
	if((PINB & _BV(PB0))==0) newpos=3;
	if((PINB & _BV(PB7))==0) newpos^=1;	// konwersja kodu Graya na binarny
	diff=last-newpos;
	if(diff & 1)
	{				// bit 0 = krok
		last=newpos;
		enc_delta+=(diff & 2)-1;	//bit 1 - kierunek
	}
}

int8_t ReadStepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=0;
	return val;
}

int8_t Read2StepEncoder()
{
	ReadEncoder();
	int8_t val=enc_delta;
	enc_delta=val & 1;
	return val>>1;
}
//
//int8_t Read4StepEncoder()
//{
	//ReadEncoder();
	//int8_t val=enc_delta;
	//enc_delta=val & 3;
	//return val>>2;
//}
