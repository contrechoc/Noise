#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
PB3 = analog input (ADC2)
*/

#define BIT_LED 4
#define BIT_KEY 1


#define UTIL_BIN4(x)        (unsigned char)((0##x & 01000)/64 + (0##x & 0100)/16 + (0##x & 010)/4 + (0##x & 1))
#define UTIL_BIN8(hi, lo)   (unsigned char)(UTIL_BIN4(hi) * 16 + UTIL_BIN4(lo))


unsigned short adc_read(unsigned char pin)
{
	unsigned char l, h, r;

	r = (ADMUX & 0xf0) | (pin & 0x0f);
	ADMUX = r; /* select the input channel */
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);

	/* must read the low ADC byte before the high ADC byte */
	l = ADCL;
	h = ADCH;

	return ((unsigned short)h << 8) | l;
}

void adc_init()
{
	/* internal pull-ups interfere with the ADC. disable the
	 * pull-up on the pin if it's being used for ADC. either
	 * writing 0 to the port register or setting it to output
	 * should be enough to disable pull-ups. */
	PORTB &= ~_BV(PB2);
	//DDRB = 0x00;
	DDRB &= ~_BV(PB2);
	/* unless otherwise configured, arduinos use the internal Vcc
	 * reference. MUX 0x0f samples the ground (0.0V). */
	ADMUX = 0x0D;//B1101 = GND attiny45 page 139
	/*
	 * Enable the ADC system, use 128 as the clock divider on a 16MHz
	 * arduino (ADC needs a 50 - 200kHz clock) and start a sample. the
	 * AVR needs to do some set-up the first time the ADC is used; this
	 * first, discarded, sample primes the system for later use.
	 */
	ADCSRA |= _BV(ADEN) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADSC);
	/* wait for the ADC to return a sample */
	loop_until_bit_is_clear(ADCSRA, ADSC);
}

int main(void)
{
    	DDRB |= 1 << BIT_LED;   /* output for LED */

 	adc_init();

unsigned short valueADC = adc_read(3);
unsigned short valueADC_old = valueADC;
unsigned short diff = abs(valueADC - valueADC_old);

unsigned char c = 0; 

	 sei();
    for(;;){

        char i, j, k;

	valueADC = adc_read(3);
	diff = abs(valueADC - valueADC_old);

	if ( diff > 2 ) 
		while (diff > 0)
{
 			for(j = 0; j <   5+diff ; j++){
				{
				
				PORTB |= 1 << BIT_LED; 
        			for(i = 0; i <   1 ; i++){
					if (diff > 64 )  _delay_ms(  20 ); 
					if (diff > 32 )  _delay_ms(  4 ); 
            				if (diff > 16 )  _delay_ms(  10 ); 
 					if (diff > 8 )  _delay_ms(  3 );
					if (diff > 4 ) _delay_ms(  2 );
					if (diff > 1 ) _delay_ms(  1); 
      					  }

				PORTB &= ~(1 << BIT_LED);
				for(k = 0; k <   1+ c/90 ; k++)_delay_ms(  1 );
 				 }
				c+=diff;
				
				}
		diff/=2;
		valueADC_old = valueADC;
       }

    }
    return 0;               /* never reached */
}
