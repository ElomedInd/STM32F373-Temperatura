#ifndef TEMPERATURA_HPP
#define TEMPERATURA_HPP

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void acelerometro_ambiente_setup();
    void acelerometro_ambiente_loop();
    void mlx90632_ambiente_loop();
    extern float ler_pressao_atmosferica();
    extern float ler_temperatura_ambiente();
    extern float ler_umidade();

#ifdef __cplusplus
}
#endif

#endif //#ifndef TEMPERATURA_HPP