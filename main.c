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

void init(void)
{
	/* OC0A PD6
	 * OC0B PD5
	 */
	DDRD |= _BV(PD5);
}

void shut(void)
{
	DDRD &= ~_BV(PD5);
}

int main(void)
{
	uint8_t i;

	led_init();
	init();

	TCCR0A = _BV(COM0B1) | _BV(COM0B0) | _BV(WGM02) | _BV(WGM01) | _BV(WGM00);
	/* prescaler 256 */
	TCCR0B = _BV(CS02);
	/* Fstep = 16Mhz / 256 = 62500hz
	 * 40 step = 62500hz / 40 = 1562.5 hz (Ftop)
	 * Duty 90% = _______|-|
	 */
	OCR0A = 40;
	OCR0B = 35;
	/* Ena IRQ Compare Match B and Overflow */
	TIMSK0 = _BV(OCIE0B) | _BV(TOIE0);

	while(1) {
		led(ON);
		_delay_ms(100);
		led(OFF);
		_delay_ms(900);
	}

	return(0);
}
