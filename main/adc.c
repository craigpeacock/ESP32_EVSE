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
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "soc/sens_reg.h"
#include "soc/sens_struct.h"

static esp_adc_cal_characteristics_t adc1_chars;

void Init_ADC(void)
{
	esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
	adc1_config_width(ADC_WIDTH_12Bit);
	adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
}

/*
 * CP_ReadAN() - Read analog value of Control Pilot; result in millivolts (mV)
 * If PWM is not running, software trigger ADC and return instantaneous value.
 * If PWM is running, obtain value from Positive Pulse (Triggered from PWM module)
 *
 */
uint32_t CP_ReadAN(void)
{
    uint32_t result;
    uint32_t rawvalue;

	rawvalue = adc1_get_raw(ADC1_CHANNEL_0);
	result = esp_adc_cal_raw_to_voltage(rawvalue, &adc1_chars);

    return (result);
}
