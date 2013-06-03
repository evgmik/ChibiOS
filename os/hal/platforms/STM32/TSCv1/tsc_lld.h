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

#define STM32_TSC_IO1 (uint32_t)0x01
#define STM32_TSC_IO2 (uint32_t)0x02
#define STM32_TSC_IO3 (uint32_t)0x04
#define STM32_TSC_IO4 (uint32_t)0x08

#define STM32_TSC_GROUP1 (uint32_t)0x00
#define STM32_TSC_GROUP2 (uint32_t)0x04
#define STM32_TSC_GROUP3 (uint32_t)0x08
#define STM32_TSC_GROUP4 (uint32_t)0x0C
#define STM32_TSC_GROUP5 (uint32_t)0x10
#define STM32_TSC_GROUP6 (uint32_t)0x14
#define STM32_TSC_GROUP7 (uint32_t)0x18
#define STM32_TSC_GROUP8 (uint32_t)0x1C

#define STM32_TSC_CR_EN (uint32_t)0x01
#define STM32_TSC_CR_START (uint32_t)0x02
#define STM32_TSC_CR_AM (uint32_t)0x04
#define STM32_TSC_CR_SYNCPOL (uint32_t)0x08
#define STM32_TSC_CR_IODEF (uint32_t)0x10

#define STM32_TSC_CR_MCV_MASK (uint32_t)0x2C
#define STM32_TSC_CR_MCV_0 (uint32_t)0x20
#define STM32_TSC_CR_MCV_1 (uint32_t)0x40
#define STM32_TSC_CR_MCV_2 (uint32_t)0x80

#define STM32_TSC_CR_PGPSC_MASK (uint32_t)0x7000
#define STM32_TSC_CR_PGPSC_0 (uint32_t)0x1000
#define STM32_TSC_CR_PGPSC_1 (uint32_t)0x2000
#define STM32_TSC_CR_PGPSC_2 (uint32_t)0x4000

#define STM32_TSC_CR_SSPSC (uint32_t)0x8000
#define STM32_TSC_CR_SSE (uint32_t)0x10000

#define STM32_TSC_CR_SSD_MASK (uint32_t)0xFE0000
#define STM32_TSC_CR_SSD_0 (uint32_t)0x20000
#define STM32_TSC_CR_SSD_1 (uint32_t)0x40000
#define STM32_TSC_CR_SSD_2 (uint32_t)0x80000
#define STM32_TSC_CR_SSD_3 (uint32_t)0x100000
#define STM32_TSC_CR_SSD_4 (uint32_t)0x200000
#define STM32_TSC_CR_SSD_5 (uint32_t)0x400000
#define STM32_TSC_CR_SSD_6 (uint32_t)0x800000

#define STM32_TSC_CR_CTPL_MASK (uint32_t)0xF000000
#define STM32_TSC_CR_CTPL_0 (uint32_t)0x1000000
#define STM32_TSC_CR_CTPL_1 (uint32_t)0x2000000
#define STM32_TSC_CR_CTPL_2 (uint32_t)0x4000000
#define STM32_TSC_CR_CTPL_3 (uint32_t)0x8000000

#define STM32_TSC_CR_CTPH_MASK (uint32_t)0xF0000000
#define STM32_TSC_CR_CTPH_0 (uint32_t)0x10000000
#define STM32_TSC_CR_CTPH_1 (uint32_t)0x20000000
#define STM32_TSC_CR_CTPH_2 (uint32_t)0x40000000
#define STM32_TSC_CR_CTPH_3 (uint32_t)0x80000000

#define STM32_TSC_IER_EOAIE (uint32_t)0x01
#define STM32_TSC_IER_MCEIE (uint32_t)0x02

#define STM32_TSC_ICR_EOAIC (uint32_t)0x01
#define STM32_TSC_ICR_MCEIC (uint32_t)0x02

#define STM32_TSC_ISR_EOAF (uint32_t)0x01
#define STM32_TSC_ISR_MCEF (uint32_t)0x02

#define STM32_TSC_IOGCSR_G1E (uint32_t)0x01
#define STM32_TSC_IOGCSR_G2E (uint32_t)0x02
#define STM32_TSC_IOGCSR_G3E (uint32_t)0x04
#define STM32_TSC_IOGCSR_G4E (uint32_t)0x08
#define STM32_TSC_IOGCSR_G5E (uint32_t)0x10
#define STM32_TSC_IOGCSR_G6E (uint32_t)0x20

#define STM32_TSC_IOGCSR_G1S (uint32_t)0x10000
#define STM32_TSC_IOGCSR_G2S (uint32_t)0x20000
#define STM32_TSC_IOGCSR_G3S (uint32_t)0x40000
#define STM32_TSC_IOGCSR_G4S (uint32_t)0x80000
#define STM32_TSC_IOGCSR_G5S (uint32_t)0x100000
#define STM32_TSC_IOGCSR_G6S (uint32_t)0x200000

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
#define STM32_TSC_USE_TSC1             TRUE
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
  /* Callback function pointer */
  const void* callback;

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
  TSC_TypeDef               *tsc;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define TSC_PADS(group, pads) (uint32_t)(pads << group)

#define tscReadPad(group, pad) ((TSCDriver)->tsc->IOGXCR[group+1] & (pad))

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
