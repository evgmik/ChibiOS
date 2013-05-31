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

/**
 * @file    STM32F4xx/dac_lld.c
 * @brief   STM32F4xx DAC subsystem low level driver source.
 *
 * @addtogroup DAC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define DAC_CHN1_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC_CHN1_DMA_STREAM,                            \
                       STM32_DAC_CHN1_DMA_CHN)

#define DAC_CHN2_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_DAC_CHN2_DMA_STREAM,                            \
                       STM32_DAC_CHN2_DMA_CHN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CHN1 driver identifier.*/
#if STM32_DAC_USE_CHN1 || defined(__DOXYGEN__)
DACDriver DACD1;
#endif

/** @brief CHN2 driver identifier.*/
#if STM32_DAC_USE_CHN2 || defined(__DOXYGEN__)
DACDriver DACD2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared end/half-of-tx service routine.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void dac_lld_serve_tx_interrupt(DACDriver *dacp, uint32_t flags) {
#if defined(STM32_DAC_DMA_ERROR_HOOK)
  (void)dacp;
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
		/* DMA errors handling.*/
		_dac_isr_error_code(dacp);
  }
  else {
    if ((flags & STM32_DMA_ISR_HTIF) != 0) {
      /* Half transfer processing.*/
      _dac_isr_half_code(dacp);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0) {
      /* Transfer complete processing.*/
      _dac_isr_full_code(dacp);
    }
  }
#else
  (void)dacp;
  (void)flags;
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level DAC driver initialization.
 *
 * @notapi
 */
void dac_lld_init(void) {

#if STM32_DAC_USE_CHN1
  dacObjectInit(&DACD1);
  DACD1.dac     = DAC;
  DACD1.tim     = STM32_TIM6;
  DACD1.irqprio = STM32_DAC_CHN1_IRQ_PRIORITY;
  DACD1.dma     = STM32_DMA_STREAM(STM32_DAC_CHN1_DMA_STREAM);
  DACD1.dmamode = STM32_DMA_CR_CHSEL(DAC_CHN1_DMA_CHANNEL) |                  \
                  STM32_DMA_CR_PL(STM32_DAC_CHN1_DMA_PRIORITY) |              \
                  STM32_DMA_CR_DIR_M2P |                                      \
                  STM32_DMA_CR_DMEIE |                                        \
                  STM32_DMA_CR_TEIE |                                         \
                  STM32_DMA_CR_MINC | STM32_DMA_CR_TCIE;
#endif

#if STM32_DAC_USE_CHN2
  dacObjectInit(&DACD2);
  DACD2.dac     = DAC;
  DACD2.tim     = STM32_TIM7;
  DACD2.irqprio = STM32_DAC_CHN2_IRQ_PRIORITY;
  DACD2.dma     = STM32_DMA_STREAM(STM32_DAC_CHN2_DMA_STREAM);
  DACD2.dmamode = STM32_DMA_CR_CHSEL(DAC_CHN2_DMA_CHANNEL) |                  \
                  STM32_DMA_CR_PL(STM32_DAC_CHN2_DMA_PRIORITY) |              \
                  STM32_DMA_CR_DIR_M2P |                                      \
                  STM32_DMA_CR_DMEIE |                                        \
                  STM32_DMA_CR_TEIE |                                         \
                  STM32_DMA_CR_MINC | STM32_DMA_CR_TCIE;
#endif
}

/**
 * @brief   Configures and activates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start(DACDriver *dacp) {
  uint32_t arr, regshift, trgo, dataoffset;
	bool_t b;
  /* If in stopped state then enables the DAC and DMA clocks.*/
  if (dacp->state == DAC_STOP) {
    rccEnableDAC(FALSE);
#if STM32_DAC_USE_CHN1
    if (&DACD1 == dacp) {
      /* DAC1 CR data is at bits 0:15 */
      regshift = 0;
      dataoffset = 0;
      /* Timer setup */
      rccEnableTIM6(FALSE);
      rccResetTIM6();
      trgo = STM32_DAC_CR_TSEL_TIM6;
    }
#endif
#if STM32_DAC_USE_CHN2
    if (&DACD2 == dacp) {
      /* DAC2 CR data is at bits 16:31 */
      regshift = 16;
      dataoffset = &dacp->dac->DHR12R2 - &dacp->dac->DHR12R1;
      /* Timer setup */
      rccEnableTIM7(FALSE);
      rccResetTIM7();
      trgo = STM32_DAC_CR_TSEL_TIM7;
    }
#endif
#if STM32_DAC_USE_CHN1 || STM32_DAC_USE_CHN2
    dacp->clock = STM32_TIMCLK1;
    arr = (dacp->clock / dacp->config->frequency);
    chDbgAssert((arr <= 0xFFFF) &&
            (arr * dacp->config->frequency) == dacp->clock,
            "dac_lld_start(), #1", "invalid frequency");

    /* Timer configuration.*/
    dacp->tim->CR1  = 0;                        /* Initially stopped.   */
    dacp->tim->PSC  = 0;                        /* Prescaler value.     */
    dacp->tim->DIER = 0;
    dacp->tim->ARR  = arr;
    dacp->tim->EGR  = TIM_EGR_UG;               /* Update event.        */
    dacp->tim->CR2  &= (uint16_t)~TIM_CR2_MMS;
    dacp->tim->CR2  |= (uint16_t)TIM_CR2_MMS_1; /* Enable TRGO updates. */
    dacp->tim->CNT  = 0;                        /* Reset counter.       */
    dacp->tim->SR   = 0;                        /* Clear pending IRQs.  */
    /* Update Event IRQ enabled. */
    /* Timer start.*/
    dacp->tim->CR1  = TIM_CR1_CEN;

    /* DAC configuration */
    dacp->dac->CR |= ( (dacp->dac->CR & ~STM32_DAC_CR_MASK) | \
        (STM32_DAC_CR_EN | STM32_DAC_CR_DMAEN | dacp->config->cr_flags) )\
            << regshift;

    /* DMA setup. */
    b = dmaStreamAllocate(dacp->dma,
                dacp->irqprio,
                (stm32_dmaisr_t)dac_lld_serve_tx_interrupt,
                (void *)dacp);
    chDbgAssert(!b, "dac_lld_start(), #2", "stream already allocated");
    switch (dacp->config->dhrm) {
      /* Sets the DAC data register */
      case DAC_DHRM_12BIT_RIGHT:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12R1 + dataoffset);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                    STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_12BIT_LEFT:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12L1 + dataoffset);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                    STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_8BIT_RIGHT:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR8R1 + dataoffset);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                    STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
        break;
      case DAC_DHRM_12BIT_RIGHT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12RD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                    STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_12BIT_LEFT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR12LD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                    STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
        break;
      case DAC_DHRM_8BIT_RIGHT_DUAL:
        dmaStreamSetPeripheral(dacp->dma, &dacp->dac->DHR8RD);
        dacp->dmamode = (dacp->dmamode & ~STM32_DMA_CR_SIZE_MASK) |
                 STM32_DMA_CR_PSIZE_BYTE | STM32_DMA_CR_MSIZE_BYTE;
        break;
	}
	
	dacp->dac->CR |= trgo << regshift; /* Enable trigger */
#endif
  }
}

/**
 * @brief   Deactivates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_stop(DACDriver *dacp) {

  /* If in ready state then disables the DAC clock.*/
  if (dacp->state == DAC_READY) {

    /* DMA disable.*/
    dmaStreamRelease(dacp->dma);

#if STM32_DAC_USE_CHN1
    if (&DACD1 == dacp) {
      dacp->dac->CR &= ~STM32_DAC_CR_EN; /* DAC disable.*/
    }
#endif
#if STM32_DAC_USE_CHN2
    if (&DACD2 == dacp) {
      dacp->dac->CR &= ~STM32_DAC_CR_EN << 16); /* DAC disable.*/
    }
#endif
    dacp->tim->CR1 &= ~TIM_CR1_CEN; /* Disable associated timer */
    dacp->state = DAC_STOP;

    if (!(DAC->CR & (STM32_DAC_CR_EN | STM32_DAC_CR_EN << 16))) {
      /* DAC Clock disable only if all channels are off.*/
      rccDisableDAC(FALSE);
    }
  }
}

/**
 * @brief   Sends data over the DAC bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void dac_lld_send(DACDriver *dacp, size_t n, const void *txbuf) {
  dmaStreamSetMemory0(dacp->dma, txbuf);
  dmaStreamSetTransactionSize(dacp->dma, n);
  dmaStreamSetMode(dacp->dma, dacp->dmamode | STM32_DMA_CR_EN);
}

void dac_lld_send_circular(DACDriver *dacp, size_t n, const void *txbuf){
  dmaStreamSetMemory0(dacp->dma, txbuf);
  dmaStreamSetTransactionSize(dacp->dma, n);
  dmaStreamSetMode(dacp->dma, dacp->dmamode | STM32_DMA_CR_EN |               \
                   STM32_DMA_CR_CIRC);
}

void dac_lld_send_doublebuffer(DACDriver *dacp, size_t n, const void *txbuf0, \
                               const void *txbuf1) {
  dmaStreamSetMemory0(dacp->dma, txbuf0);
  dmaStreamSetMemory1(dacp->dma, txbuf1);
  dmaStreamSetTransactionSize(dacp->dma, n);
  dmaStreamSetMode(dacp->dma, dacp->dmamode | STM32_DMA_CR_EN |               \
                   STM32_DMA_CR_DBM);
}

#endif /* HAL_USE_DAC */

/** @} */
