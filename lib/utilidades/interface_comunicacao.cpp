#if defined(STM32)
#include "wwdg.h"
#include "usart.h"
#include "interface_comunicacao.hpp"
#include <stdarg.h>
#include <stdio.h>

extern "C" void oled_print(int linha, int coluna, const char *txt, ...)
{
    char buffer[80];
    va_list args;
    va_start(args, txt);
    vsprintf(buffer, txt, args);
    Print_String(linha, coluna, buffer);
    Oled_Refresh();
    va_end(args);
}

extern "C" void uart_print(const char *txt, ...)
{
    HAL_WWDG_Refresh(&hwwdg);
    char buffer[256];
    va_list args;
    va_start(args, txt);
    int len = vsprintf(buffer, txt, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t *)buffer, len, 1000);
}

#endif