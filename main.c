#include <stdbool.h>
#include <stdio.h>
#include "stm32f1xx.h"
#include "rcc.h"
#include "uart.h"
#include "gpio.h"

int main(void) {
    // Max clock speed 72 MHz
    rcc_HSE_config();
    rcc_SysTick_config(72000); // 72 000 000 Hz = AHB Clock / 72000 to obtain 1 Khz (1000) = 1ms
    uart_UART1_gpio_config();
    uart_UART1_config();
    gpio_LED_config();

    printf("Program is starting...\r\n");
    while(1)
    {
        gpio_LED_toggleGreen();
        gpio_LED_toggleRed();
        printf("Hello, World!\r\n");
        rcc_msDelay(500);
    }
}
