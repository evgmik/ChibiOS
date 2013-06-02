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
 * @file    tsc.c
 * @brief   TSC Driver code.
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

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   TSC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void tscInit(void) {

  tsc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p TSCDriver structure.
 *
 * @param[out] tscp     pointer to the @p TSCDriver object
 *
 * @init
 */
void tscObjectInit(TSCDriver *tscp) {

  tscp->state  = TSC_STOP;
  tscp->config = NULL;
}

/**
 * @brief   Configures and activates the TSC peripheral.
 *
 * @param[in] tscp      pointer to the @p TSCDriver object
 * @param[in] config    pointer to the @p TSCConfig object
 *
 * @api
 */
void tscStart(TSCDriver *tscp, const TSCConfig *config) {

  chDbgCheck((tscp != NULL) && (config != NULL), "tscStart");

  chSysLock();
  chDbgAssert((tscp->state == TSC_STOP) || (tscp->state == TSC_READY),
              "tscStart(), #1", "invalid state");
  tscp->config = config;
  tsc_lld_start(tscp);
  tscp->state = TSC_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the TSC peripheral.
 *
 * @param[in] tscp      pointer to the @p TSCDriver object
 *
 * @api
 */
void tscStop(TSCDriver *tscp) {

  chDbgCheck(tscp != NULL, "tscStop");

  chSysLock();
  chDbgAssert((tscp->state == TSC_STOP) || (tscp->state == TSC_READY),
              "tscStop(), #1", "invalid state");
  tsc_lld_stop(tscp);
  tscp->state = TSC_STOP;
  chSysUnlock();
}

#endif /* HAL_USE_TSC */

/** @} */
