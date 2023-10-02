//
// Created by Dananjaya RAMANAYAKE on 02/10/2023.
//

#include "gpio.h"

/*
 * @brief LEDs GPIO configuration (PB13=Green, PB=14=Red)
 */
void gpio_LED_config(void) {
    // Enable Port B clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // Set the mode to output
    GPIOB->CRH &= ~(GPIO_CRH_MODE13); //CLEAR_BIT(GPIOB->CRH, GPIO_CRH_MODE13);
    GPIOB->CRH &= ~(GPIO_CRH_MODE14);
    GPIOB->CRH |= (GPIO_CRH_MODE13_1);
    GPIOB->CRH |= (GPIO_CRH_MODE14_1);
    // Set to output push-pull
    GPIOB->CRH &= ~(GPIO_CRH_CNF13);
    GPIOB->CRH &= ~(GPIO_CRH_CNF14);
}

/*
 * @brief Write/Toggle functions
 */
void gpio_LED_writeGreen(bool state) {
    if(state) {
        GPIOB->ODR |= GPIO_ODR_ODR13;
    } else {
        GPIOB->ODR &= ~(GPIO_ODR_ODR13);
    }
}

void gpio_LED_writeRed(bool state) {
    if(state) {
        GPIOB->ODR |= GPIO_ODR_ODR14;
    } else {
        GPIOB->ODR &= ~(GPIO_ODR_ODR14);
    }
}

void gpio_LED_toggleGreen(void) {
    GPIOB->ODR ^= GPIO_ODR_ODR13;
}

void gpio_LED_toggleRed(void) {
    GPIOB->ODR ^= GPIO_ODR_ODR14;
}
