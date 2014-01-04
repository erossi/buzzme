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

#include <avr/io.h>
#include <util/delay.h>
#include "arduino_led.h"

void led_init(void)
{
	/* Arduino led L */
	PORTB &= ~_BV(PB5);
	DDRB |= _BV(PB5);
}

/** set or clear the led L
 *
 * @param status ON or OFF
 */
void led(const char status)
{
	switch (status) {
		case ON:
			PORTB |= _BV(PB5);
			break;
		default:
			PORTB &= ~_BV(PB5);
	}
}

void led_shut(void)
{
	PORTB &= ~_BV(PB5);
	DDRB &= ~_BV(PB5);
}
