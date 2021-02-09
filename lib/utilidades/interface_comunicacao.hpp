#ifndef INTERFACE_COMUNICACAO_HPP
#define INTERFACE_COMUNICACAO_HPP

#if defined(STM32)

#ifdef __cplusplus
extern "C"
{
#endif

#include "wwdg.h"
#include "usart.h"
#include "OLED_Hal_SPI.h"

#include <stdarg.h>
#include <stdio.h>

    void oled_print(int linha, int coluna, const char *txt, ...);
    void uart_print(const char *txt, ...);

#ifdef __cplusplus
}
#endif
#endif //defined stm32
#endif 