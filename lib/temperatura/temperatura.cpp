#include "temperatura.hpp"
#include "Adafruit_BMP280.h"
#include "Adafruit_BME280.h"
#include <Adafruit_Sensor.h>
#include "interface_comunicacao.hpp"
#include "OLED_Hal_SPI.h"

Adafruit_BME280 bme; // use I2C interface
bool mpu_initialzed = false;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void acelerometro_ambiente_setup()
{

    oled_print(0, 2, "Elomed Temperatura");

    HAL_Delay(10);
    HAL_WWDG_Refresh(&hwwdg); //as vezes a HAL_Delay pode disparar o watchdog.

    if (!bme.begin(0x76))
        oled_print(1, 2, "BME n Inicializado");
    else
        oled_print(1, 3, "BME Inicializado");

    sensors_event_t a, g, temp;
    uart_print("Temperature: %f\n", temp.temperature);
}

float ler_pressao_atmosferica()
{
    sensors_event_t pressure_event;
    bme_pressure->getEvent(&pressure_event);
    return pressure_event.pressure;
}

float ler_temperatura_ambiente()
{
    sensors_event_t temp_event;
    bme_temp->getEvent(&temp_event);
    return temp_event.temperature;
}

float ler_umidade()
{
    sensors_event_t humidity_event;
    bme_humidity->getEvent(&humidity_event);
    return humidity_event.relative_humidity;
}

void acelerometro_ambiente_loop()
{
    oled_print(4, 2, "umidade %.0f %%    ", ler_umidade());
    oled_print(5, 2, "temp.   %.1f       ", ler_temperatura_ambiente());
    oled_print(6, 2, "pressao %.1f       ", ler_pressao_atmosferica());

    uart_print("umidade %.0f %%    \r\n", ler_umidade());
    uart_print("temp.   %.1f       \r\n", ler_temperatura_ambiente());
    uart_print("pressao %.1f       \r\n", ler_pressao_atmosferica());
}