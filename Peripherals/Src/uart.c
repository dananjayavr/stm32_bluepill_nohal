//
// Created by Dananjaya RAMANAYAKE on 27/09/2023.
//

#include "uart.h"
#include "rcc.h"

/*
 * @brief UART1 GPIO Configuration
 */
void uart_UART1_gpio_config(void) {
    // Tx -> PA9, Rx -> PA10
    // Enable Port A clock
    RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN);
    // Mode to AF (UART1)
    GPIOA->CRH &= ~(GPIO_CRH_CNF9);
    GPIOA->CRH |= (GPIO_CRH_CNF9_1); // PA9
    GPIOA->CRH &= ~(GPIO_CRH_CNF10); // PA10
    GPIOA->CRH |= (GPIO_CRH_CNF10_0);

    // Output max 10 MHz
    GPIOA->CRH &= ~(GPIO_CRH_MODE9); // PA9
    GPIOA->CRH |= (GPIO_CRH_MODE9_0);
    GPIOA->CRH &= ~(GPIO_CRH_MODE10); // PA10

    // Map PA9 and PA10 mapped to UART1
    RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN);
    AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP);
}

/*
 * @brief UART1 Peripheral Configuration
 */
void uart_UART1_config(void) {
    // Enable UART1 clock
    RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);
    // Enable Transmit
    USART1->CR1 |= (USART_CR1_TE);
    // Parity to even
    USART1->CR1 &= ~(USART_CR1_PS);
    // Parity control disabled
    USART1->CR1 &= ~(USART_CR1_PCE);
    // Word length 8 bits
    USART1->CR1 &= ~(USART_CR1_M);
    // Stop 1 bit
    USART1->CR2 &= ~(USART_CR2_STOP);
    // Disable hardware flow control
    USART1->CR3 &= ~(USART_CR3_CTSE); // Tx
    USART1->CR3 &= ~(USART_CR3_RTSE); // Rx
    // Set baud rate to 115200
    USART1->BRR = 0;
    USART1->BRR |= (39UL << USART_BRR_DIV_Mantissa_Pos);
    USART1->BRR |= (1UL << USART_BRR_DIV_Fraction_Pos);


    // Disable LIN mode and disable CK pin (used in USART mode)
    USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
    // Disable Smartcard mode, Disable Half-duplex selection and disable IrDA mode
    USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
    // Enable UART1
    USART1->CR1 |= (USART_CR1_UE);
}

/*
 * @brief UART1 Transmit
 */
bool uart_UART1_transmit(uint8_t *data, uint8_t len, uint32_t timeout) {
    // Wait on TXE to start transmit (Transmission data register empty)
    // Write to the DR as TXE flag is HIGH (Transmission data register is empty)
    uint8_t dataIdx = 0;
    uint32_t startTick = rcc_msGetTicks();
    while (dataIdx < len) {
        if(USART1->SR & USART_SR_TXE) { // Tx buffer empty
            USART1->DR = data[dataIdx];
            dataIdx++;
        } else {
            // manage timeout
            if((rcc_msGetTicks() - startTick) >= timeout) return false;
        }
    }
    // Wait for busy flag
    while(USART1->SR & USART_SR_TC) {
        if((rcc_msGetTicks() - startTick) >= timeout) return false;
    }

    return true;
}
