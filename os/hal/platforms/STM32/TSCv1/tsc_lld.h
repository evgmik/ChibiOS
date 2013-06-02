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
 * @file    templates/tsc_lld.h
 * @brief   TSC Driver subsystem low level driver header template.
 *
 * @addtogroup TSC
 * @{
 */

#ifndef _TSC_LLD_H_
#define _TSC_LLD_H_

#if HAL_USE_TSC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   TSC driver enable switch.
 * @details If set to @p TRUE the support for TSC1 is included.
 */
#if !defined(STM32_TSC_USE_TSC1) || defined(__DOXYGEN__)
#define STM32_TSC_USE_TSC1             FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an TSC driver.
 */
typedef struct TSCDriver TSCDriver;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {

} TSCConfig;

/**
 * @brief   Structure representing an TSC driver.
 */
struct TSCDriver {
  /**
   * @brief Driver state.
   */
  tscstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const TSCConfig           *config;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_TSC_USE_TSC1 && !defined(__DOXYGEN__)
extern TSCDriver TSCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void tsc_lld_init(void);
  void tsc_lld_start(TSCDriver *tscp);
  void tsc_lld_stop(TSCDriver *tscp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_TSC */

#endif /* _TSC_LLD_H_ */

/** @} */
