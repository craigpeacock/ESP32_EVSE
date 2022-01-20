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
#include "pwm.h"

void Init_PWM(void)
{
	ESP_ERROR_CHECK(mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_NUM_18));

    mcpwm_config_t pwm_config = {
        .frequency = 1000,
        .cmpr_a = 50,
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
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
        return;
    }

    if (amps == LOW) {
        // Set Digital Pin Low
        return;
    }

    if (amps == DIGITAL) {
        // Set Control Pilot to use Digital Communication (5% Duty Cycle)
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, 5);
        return;
    }

    if (amps < 60) {
        // Minimum value is 6 amps.
        amps = 60;
    }

    if ((amps >= 60) && (amps < 510)) {
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, amps / 6 );
    	//print_timestamp();
        //printf("Setting Control Pilot to %02d.%01d Amps, SDC4 = 0x%04X\r\n",amps/10, amps%10, SDC4);
    } else {
    	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, amps / 25 + 64 );
    }
}
