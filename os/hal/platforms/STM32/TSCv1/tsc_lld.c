/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    templates/tsc_lld.c
 * @brief   TSC Driver subsystem low level driver source template.
 *
 * @addtogroup TSC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_TSC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   TSC1 driver identifier.
 */
#if STM32_TSC_USE_TSC1 || defined(__DOXYGEN__)
TSCDriver TSCD1;
#endif

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level TSC driver initialization.
 *
 * @notapi
 */
void tsc_lld_init(void) {

#if STM32_TSC_USE_TSC1
  /* Driver initialization.*/
  tscObjectInit(&TSCD1);
#endif /* STM32_TSC_USE_TSC1 */
}

/**
 * @brief   Configures and activates the TSC peripheral.
 *
 * @param[in] tscp      pointer to the @p TSCDriver object
 *
 * @notapi
 */
void tsc_lld_start(TSCDriver *tscp) {

  if (tscp->state == TSC_STOP) {
    /* Enables the peripheral.*/
    rccEnableTSC(FALSE);
#if STM32_TSC_USE_TSC1
    if (&TSCD1 == tscp) {

    }
#endif /* STM32_TSC_USE_TSC1 */
  }
  /* Configures the peripheral.*/

}

/**
 * @brief   Deactivates the TSC peripheral.
 *
 * @param[in] tscp      pointer to the @p TSCDriver object
 *
 * @notapi
 */
void tsc_lld_stop(TSCDriver *tscp) {

  if (tscp->state == TSC_READY) {
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if STM32_TSC_USE_TSC1
    if (&TSCD1 == tscp) {

    }
#endif /* STM32_TSC_USE_TSC1 */
  }
  rccDisableTSC(FALSE);
}

#endif /* HAL_USE_TSC */

/** @} */
