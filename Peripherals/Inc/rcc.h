//
// Created by Dananjaya RAMANAYAKE on 23/09/2023.
//

#ifndef _RCC_H
#define _RCC_H

#include "stm32f1xx.h"



/*
 * @brief HSE Configuration
 */
void rcc_HSE_config(void);

/*
 * @brief SysTick Configuration
 */
void rcc_SysTick_config(uint32_t arr);

/*
 * @brief Increment millisecond ticks
 */
void rcc_msIncTicks(void);

/*
 * @brief Get msTicks
 */
uint32_t rcc_msGetTicks(void);

/*
 * @brief ms Delay
 */
void rcc_msDelay(uint32_t msDelay);

#endif //_RCC_H
