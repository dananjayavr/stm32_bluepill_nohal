//
// Created by Dananjaya RAMANAYAKE on 23/09/2023.
//

#include "rcc.h"

// Milliseconds tick
static __IO uint32_t msTicks = 0;

// Driving an HSE (external oscillator) through PLL to obtain the desired frequency.

/*
 * @brief HSE Configuration
 */
void rcc_HSE_config(void) {
    /*
     * Configuration parameters: STM32F103 Clock Tree
     * HSE = 8 MHz
     * PLL_M = x9 (obtained from CubeMX)
     * USB divider = 1.5 (OTGFS divider in recent reference manuals, divided by 3 if 72 MHz PLL)
     * AHB Prescaler = 1
     * ARM Cortex Prescaler = 1 (this goes to SysTick)
     *
     * Obtain: 72 MHz clock
     *
     * APB1 Prescaler = 2 (32 MHz, 72 MHz)
     * APB2 Prescaler = 1 (72 MHz, 72 MHz)
     * ADC Prescaler = 6 (12 MHz)
     */

    // PLL Configuration (PLL_M = 9)
    //RCC->CFGR &= ~(0xFUL << 18); // Clear bit fields [21:18]
    RCC->CFGR &= ~(RCC_CFGR_PLLMULL_Msk); // Clear bit fields [21:18] (same as above)
    RCC->CFGR |= (7UL << RCC_CFGR_PLLMULL_Pos); // Writing 0111 (7) to set the PLL_M to 9. Page 135 in the reference manual.

    //OTGFS Divider
    RCC->CFGR &= ~(RCC_CFGR_USBPRE);

    // Enable HSE oscillator and wait for it to stabilize (waiting until it becomes 8 MHz)
    RCC->CR |= RCC_CR_HSEON;
    // Wait
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    // Select HSE as the PLL source
    RCC->CFGR |= RCC_CFGR_PLLSRC;

    // Enable PLL and wait for it to be ready
    RCC->CR |= RCC_CR_PLLON;
    // Wait
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    // Set Flash prefetch and Flash wait state to 2WS = 72 MHz
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    FLASH->ACR |= FLASH_ACR_LATENCY_1;
    // Enable prefetch buffer
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    // Select PLL as the main system clock
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_1;
    // Wait for PLL to be the active clock source
    while ((RCC->CFGR & RCC_CFGR_SWS_1) == 0);

    // Peripheral clock setup (AHB, APB1, and APB2)
    // AHB prescaler
    RCC->CFGR &= ~(RCC_CFGR_HPRE); // clear all bit-fields to make it divided by 1
    // APB1 prescaler
    RCC->CFGR &= ~(RCC_CFGR_PPRE1);
    RCC->CFGR |= RCC_CFGR_PPRE1_2;
    // APB2 prescaler
    RCC->CFGR &= ~(RCC_CFGR_PPRE2);

    //ADC prescaler
    RCC->CFGR &= ~(RCC_CFGR_ADCPRE);
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
}

/*
 * @brief SysTick Configuration
 */
void rcc_SysTick_config(uint32_t arr) {
    // Reset control register
    SysTick->CTRL = 0;
    // Set the reload value
    SysTick->LOAD = arr - 1;
    // Set SysTick interrupt priority (NVIC)
    NVIC_SetPriority(SysTick_IRQn, 0);
    // Reset SysTick value to 0
    SysTick->VAL = 0;
    // Enable SysTick from CTRL register
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //0x1UL << 0;
}

/*
 * @brief Increment millisecond ticks
 */
void rcc_msIncTicks(void) {
    msTicks++;
}

/*
 * @brief Get msTicks
 */
uint32_t rcc_msGetTicks(void) {
    return msTicks;
}

/*
 * @brief ms Delay
 */
void rcc_msDelay(uint32_t msDelay) {
    uint32_t startTicks = rcc_msGetTicks();
    while(rcc_msGetTicks() - startTicks < msDelay);
}


/*
 * @brief SysTick IT callback
 */
void SysTick_Handler(void) {
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    rcc_msIncTicks();
}
