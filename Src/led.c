/*******************************************************************************
 * Copyright (c) 2017 FLEx Lighting II, LLC "FLEx Lighting". 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted, provided that the following conditions are met:
 * 
 * 1. Redistribution of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of FLEx Lighting nor the names of other 
 *    contributors to this software may be used to endorse or promote products 
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this 
 *    software, must execute solely and exclusively on devices manufactured by
 *    or for FLEx Lighting.
 * 5. Redistribution and use of this software other than as permitted under 
 *    this license is void and will automatically terminate your rights under 
 *    this license. 
 * 
 * THIS SOFTWARE IS PROVIDED BY FLEX LIGHTING AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
 * SHALL FLEX LIGHTING OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include "stm32l1xx_hal.h"
#include "led.h"

extern DAC_HandleTypeDef hdac;
static uint8_t brightness;
static uint16_t LED_Current;

static const uint16_t brightable[256] = {
    740, 800, 860, 920, 990, 1050, 1120, 1200, 1270, 1340, 1420, 1500, 1580, 1670, 1760,
    1840, 1940, 2030, 2120, 2220, 2320, 2420, 2530, 2630, 2740, 2850, 2970, 3080, 3200,
    3320, 3440, 3570, 3690, 3820, 3950, 4080, 4220, 4360, 4490, 4640, 4780, 4930, 5070,
    5220, 5380, 5530, 5690, 5850, 6010, 6170, 6340, 6510, 6680, 6850, 7020, 7200, 7380,
    7560, 7740, 7930, 8120, 8310, 8500, 8690, 8890, 9090, 9290, 9490, 9700, 9900,
    10110, 10330, 10540, 10760, 10970, 11190, 11420, 11640, 11870, 12100, 12330,
    12560, 12800, 13040, 13280, 13520, 13770, 14010, 14260, 14510, 14770, 15020,
    15280, 15540, 15800, 16070, 16330, 16600, 16870, 17150, 17420, 17700, 17980,
    18260, 18550, 18830, 19120, 19410, 19700, 20000, 20300, 20600, 20900, 21200,
    21510, 21820, 22130, 22440, 22760, 23070, 23390, 23710, 24040, 24360, 24690,
    25020, 25350, 25690, 25690, 25350, 25020, 24690, 24360, 24040, 23710, 23390,
    23070, 22760, 22440, 22130, 21820, 21510, 21200, 20900, 20600, 20300, 20000,
    19700, 19410, 19120, 18830, 18550, 18260, 17980, 17700, 17420, 17150, 16870,
    16600, 16330, 16070, 15800, 15540, 15280, 15020, 14770, 14510, 14260, 14010,
    13770, 13520, 13280, 13040, 12800, 12560, 12330, 12100, 11870, 11640, 11420,
    11190, 10970, 10760, 10540, 10330, 10110, 9900, 9700, 9490, 9290, 9090, 8890,
    8690, 8500, 8310, 8120, 7930, 7740, 7560, 7380, 7200, 7020, 6850, 6680, 6510, 6340,
    6170, 6010, 5850, 5690, 5530, 5380, 5220, 5070, 4930, 4780, 4640, 4490, 4360, 4220,
    4080, 3950, 3820, 3690, 3570, 3440, 3320, 3200, 3080, 2970, 2850, 2740, 2630, 2530,
    2420, 2320, 2220, 2120, 2030, 1940, 1840, 1760, 1670, 1580, 1500, 1420, 1340, 1270,
    1200, 1120, 1050, 990, 920, 860, 800, 740
};


void LED_write_current(uint16_t current) {
    if(current > 25000) current = 25000;
    LED_Current = current;
    uint16_t dac_val = ((uint32_t)current+120)*4481/32768;
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, dac_val);
}

void LED_set_current(uint16_t current) {
    if(current < 30) { /* LED current in mA */
        current *= 1000;
    }
    /* Clamp the brightness value to the absolute maximum */
    if (current > 25000) current = 25000;
    /* Find the closest brightness value for manual setting */
    for (brightness = 0;
         brightable[brightness] < current && brightness < 127;
         brightness++);
    LED_write_current(current);
}

uint16_t LED_get_current() {
    return LED_Current;
}

void LED_set_brightness(uint8_t new_brightness) {
    brightness = new_brightness;
    LED_write_current(brightable[brightness]);
}

void LED_change_brightness(int8_t amount) {
    brightness += amount;
    LED_write_current(brightable[brightness]);
}
