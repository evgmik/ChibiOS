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
 * @file    tsc.h
 * @brief   TSC Driver macros and structures.
 *
 * @addtogroup TSC
 * @{
 */

#ifndef _TSC_H_
#define _TSC_H_

#if HAL_USE_TSC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  TSC_UNINIT = 0,                   /**< Not initialized.                   */
  TSC_STOP = 1,                     /**< Stopped.                           */
  TSC_READY = 2,                    /**< Ready.                             */
  TSC_RUNNING = 3,                    /**< Running.                             */
} tscstate_t;

/**
 * @brief   Type of a structure representing a TSC driver.
 */
typedef struct TSCDriver TSCDriver;

#include "tsc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void tscInit(void);
  void tscObjectInit(TSCDriver *tscp);
  void tscStart(TSCDriver *tscp, const TSCConfig *config);
  void tscStop(TSCDriver *tscp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_TSC */

#endif /* _TSC_H_ */

/** @} */
