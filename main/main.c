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
#include "sdkconfig.h"
#include "pwm.h"
#include "adc.h"

#include "driver/mcpwm.h"

void app_main(void)
{
	printf("ESP32 IEC61851/SAE J1772 Demo EVSE Code\r\n");
	Init_ADC();
	Init_PWM();
	CP_Set(100);

	while (1) {
		printf("ADC = %dmV\r\n", CP_ReadAN());
		vTaskDelay( 1000 / portTICK_PERIOD_MS);
	}
}
