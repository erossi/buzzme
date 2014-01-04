/* Copyright (C) 2013 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "arduino_led.h"

volatile uint8_t beeps;

ISR(TIMER0_COMPB_vect) {
        beeps--;
}

ISR(TIMER0_OVF_vect) {
        beeps--;
}

void buzz_play(const uint16_t freq, const uint8_t duty)
{
	uint8_t c;

	/* Fstep = 16Mhz / 256 = 62500hz
	 * 40 step = 62500hz / 40 = 1562.5 hz (Ftop)
	 * Duty 90% = _______|-|
	 */
	c = (uint8_t)(62500/freq);
	OCR0A = c;
	OCR0B = (uint8_t)(c*duty/100);
	/* Clear counter */
	TCNT0 = 0x00;
	/* prescaler 256 */
	TCCR0B |= _BV(CS02);
}

void buzz_stop(void)
{
	/* stop the counter, leave the WGM set */
	TCCR0B = _BV(WGM02);
}

void buzz_init(void)
{
	/* OC0A PD6
	 * OC0B PD5
	 */
	DDRD |= _BV(PD5);
	TCCR0A = _BV(COM0B1) | _BV(COM0B0) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(WGM02);
	/* Ena IRQ Compare Match B and Overflow */
	/* TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);
	 */
}

void buzz_shut(void)
{
	TIMSK0 = 0;
	TCCR0B = 0;
	TCCR0A = 0;
	OCR0A = 0;
	OCR0B = 0;
	DDRD &= ~_BV(PD5);
}

void beep(uint16_t tone)
{
	uint8_t i;

	led(ON);
	buzz_play(tone, 20);
	i = (uint8_t)((3000-tone)/10);
	i++;

	while (i) {
		_delay_ms(10);
		i--;
	}

	led(OFF);
	buzz_stop();
	i = (uint8_t)((3000-tone)/10);
	i++;

	while (i) {
		_delay_ms(10);
		i--;
	}
}

int main(void)
{
	uint8_t i;
	uint16_t tone;

	led_init();
	buzz_init();

	while(1) {
		for (i=0; i<50; i++) {
			tone = (uint16_t)(1000+i*100);
			beep(tone);
		}
	}

	return(0);
}
