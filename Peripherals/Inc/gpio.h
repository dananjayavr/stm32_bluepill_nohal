//
// Created by Dananjaya RAMANAYAKE on 02/10/2023.
//

#ifndef _GPIO_H
#define _GPIO_H

#include "stm32f1xx.h"
#include <stdbool.h>

/*
 * @brief LEDs GPIO configuration (PB13=Green, PB=14=Red)
 */
void gpio_LED_config(void);

/*
 * @brief Write/Toggle functions
 */
void gpio_LED_writeGreen(bool state);
void gpio_LED_writeRed(bool state);

void gpio_LED_toggleGreen(void);
void gpio_LED_toggleRed(void);



#endif //_GPIO_H
