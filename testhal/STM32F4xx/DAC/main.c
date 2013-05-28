/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

static void daccb(DACDriver *dacp);

/*
 * DAC test buffer
 */
const uint16_t dac_buffer[32] = {
                        2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                        3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                        599, 344, 155, 38, 0, 38, 155, 344, 599, 0, 0, 1400};


/*
 * DAC config, with a callback.
 */
static const DACConfig daccfg = {
  84000000,
  daccb,
  DAC_DHRM_12BIT_RIGHT,
  (STM32_DAC_CR_BOFF_ENABLE |STM32_DAC_CR_WAVE_NONE | \
  STM32_DAC_CR_TSEL_TIM6 | STM32_DAC_MAMP_1)
};

/*
 * Turn on the red LED once DMA ends the transmission. 
 */
static void daccb(DACDriver *dacp) {
  (void)dacp;
  palSetPad(GPIOD, GPIOD_LED5);
}


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Set LED Pins mode.
   */
  palSetPadMode(GPIOD, GPIOD_LED5, PAL_MODE_OUTPUT_PUSHPULL);  /* Red   */

  /*
   * DAC pin setup.
   */
  palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG); /* Manual says it prevents parasitic consumption,
                                                  the mode is ignored when the DAC_CR_ENx is activated */
  
  /*
   * Starting the DAC driver threads.
   */
  dacStart(&DACD1, &daccfg);

  /*
   * Sending the dac_buffer
   */
  dacStartSend(&DACD1, 32, dac_buffer);
  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
