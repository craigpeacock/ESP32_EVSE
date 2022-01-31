/*
ESP32 EVSE Demo Code
IEC61851/SAE J1772

Copyright (C) 2022 Craig Peacock

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "pwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

static void IRAM_ATTR mcpwm_isr_handler()
{
	if (MCPWM0.int_st.cmpr0_tea_int_st) {		// Falling Edge
		gpio_set_level(GPIO_NUM_25, 1);
		gpio_set_level(GPIO_NUM_25, 0);
		MCPWM0.int_clr.cmpr0_tea_int_clr = 1; 	// Clear Flag
	}

	if (MCPWM0.int_st.cmpr0_teb_int_st) {		// Rising Edge
		gpio_set_level(GPIO_NUM_25, 1);
		gpio_set_level(GPIO_NUM_25, 0);
		MCPWM0.int_clr.cmpr0_teb_int_clr = 1; 	// Clear Flag
	}
}

void Init_PWM(void)
{
	gpio_config_t io_conf = {};

	io_conf.intr_type = GPIO_INTR_DISABLE; 	   			// No interrupt
	io_conf.pin_bit_mask = GPIO_SEL_25;					// Bit mask of the pins
	io_conf.mode = GPIO_MODE_OUTPUT;					// Set as Input
	io_conf.pull_up_en = 0;								// Enable pull-ups
	io_conf.pull_down_en = 0;
	gpio_config(&io_conf);								// Configure pin

	ESP_ERROR_CHECK(mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_NUM_18));

    mcpwm_config_t pwm_config = {
        .frequency = 1000,					// Frequency
        .cmpr_a = 50,						// Init Duty Cycle for MCPWMXA
        .counter_mode = MCPWM_UP_COUNTER,	// Counter Type
        .duty_mode = MCPWM_DUTY_MODE_0,		// Active High Duty
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    //MCPWM0.int_ena.cmpr0_tea_int_ena = 1;	// Interrupt when TEA Timer Equal reg A (Falling Edge)
    MCPWM0.int_ena.cmpr0_teb_int_ena = 1;	// Interrupt when TEB Timer Equal reg B (Rising Edge)
    mcpwm_isr_register(MCPWM_UNIT_0, mcpwm_isr_handler, NULL, ESP_INTR_FLAG_IRAM, NULL);
}

/*
 *  CP_Set() is used to set the Control Pilot signal. Valid options are:
 *   CP_Set(HIGH)    - Set the Pilot High
 *   CP_Set(LOW)     - Set the Pilot Low
 *   CP_Set(DIGITAL) - Set the Pilot to indicate digital
 *                     communications (5% Duty Cycle)
 *   CP_Set([amps])  - Set current where amps is tenths of an amp - i.e. 10A = 100
 */
void CP_Set(int16_t amps)
{
    if (amps == HIGH) {
    	// Set Digital Pin High
    	mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A);
        return;
    }

    if (amps == LOW) {
        // Set Digital Pin Low
    	mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A);
        return;
    }

    if (amps == DIGITAL) {
        // Set Control Pilot to use Digital Communication (5% Duty Cycle)
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, 5);
    	// Resume PWM Operation:
    	mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, MCPWM_DUTY_MODE_0 );
        return;
    }

    if (amps < 60) {
        // Minimum value is 6 amps.
        amps = 60;
    }

    if ((amps >= 60) && (amps < 510)) {
    	// Set Duty Cycle:
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, amps / 6 );
    	// Resume PWM Operation:
    	mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, MCPWM_DUTY_MODE_0 );
    	//print_timestamp();
        printf("Setting Control Pilot to %02d.%01d Amps\r\n",amps/10, amps%10);
    } else {
    	// Set Duty Cycle:
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, amps / 25 + 64 );
    	// Resume PWM Operation:
    	mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, MCPWM_DUTY_MODE_0 );
    }
}
