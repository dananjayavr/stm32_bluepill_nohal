//
// Created by Dananjaya RAMANAYAKE on 27/09/2023.
//

#ifndef _UART_H
#define _UART_H

#include <stdbool.h>
#include "stm32f1xx.h"

/*
 * @brief UART1 GPIO Configuration
 */
void uart_UART1_gpio_config(void);

/*
 * @brief UART1 Peripheral Configuration
 */
void uart_UART1_config(void);

/*
 * @brief UART1 Transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout);

#endif //_UART_H
