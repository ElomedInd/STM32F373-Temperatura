/**
 * @file mlx90632_depends.h
 * @brief MLX90632 driver dependencies
 * @internal
 *
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @endinternal
 *
 * @addtogroup mlx90632_API_depends MLX90632 Driver Library Externally defined
 * @brief Functions to be defined externally as they are uC or OS dependent
 *
 * @details
 * Some functions need to be declared externally as they are different for each
 * uC or OS. Depends on platform implementation SMBus functions read 8 bytes,
 * but MLX90632 has 16bit address registers (some are also 32bit). So reading
 * at least 16bits at time is recommended.
 *
 * @{
 */

#define MLX90632_ADDRESS_EAR 0x3A
#define MLX90632_ADDRESS_ARM 0x3B

#ifndef _MLX90632_DEPENDS_LIB_
#define _MLX90632_DEPENDS_LIB_
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "wwdg.h"
#include "gpio.h"
#include "mlx90632.h"

typedef struct
{
    int32_t P_R;
    int32_t P_G;
    int32_t P_T;
    int32_t P_O;
    int32_t Ea;
    int32_t Eb;
    int32_t Fa;
    int32_t Fb;
    int32_t Ga;
    int16_t Gb;
    int16_t Ka;
    int16_t Ha;
    int16_t Hb;
}  mlx90632_calibration_parameters;

/** Read the register_address value from the mlx90632
 *
 * i2c read is processor specific and this function expects to have address of mlx90632 known, as it operates purely on
 * register addresses.
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[out] *value pointer to where read data can be written

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_read(int16_t register_address, uint16_t *value);

/** Write value to register_address of the mlx90632
 *
 * i2c write is processor specific and this function expects to have address of mlx90632 known, as it operates purely
 * on register address and value to be written to.
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[in] value value to be written to register address of mlx90632

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_write(int16_t register_address, uint16_t value);

/** Blocking function for sleeping in microseconds
 *
 * Range of microseconds which are allowed for the thread to sleep. This is to avoid constant pinging of sensor if the
 * data is ready.
 *
 * @note Needs to be implemented externally
 * @param[in] min_range Minimum amount of microseconds to sleep
 * @param[in] max_range Maximum amount of microseconds to sleep
 */
void usleep(int min_range, int max_range);

/** Blocking function for sleeping in milliseconds
 *
 * milliseconds which are allowed for the thread to sleep. This is to avoid constant pinging of sensor
 * while the measurement is ongoing in sleeping step mode.
 *
 * @note Needs to be implemented externally
 * @param[in] msecs Amount of milliseconds to sleep
 */
void msleep(int msecs);

/** Reads an int32_t value from the mlx90632
 *
 * Reads 4 bytes and stores in an int32_t
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[out] *value pointer to where read data can be written

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_read_int32(uint16_t register_address, int32_t *value);

/** Reads an int16_t value from the mlx90632
 *
 * Reads 2 bytes and stores in an int16_t
 *
 * @note Needs to be implemented externally
 * @param[in] register_address Address of the register to be read from
 * @param[out] *value pointer to where read data can be written

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_i2c_read_int16(uint16_t register_address, int16_t *value);

/** Reads the calibration parameters of mlx90632
 *
 * reads the calibration parameters from EEPROM that are needed for calculating temperatures
 *
 * @param[out] *parameters Calibration parameters

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_get_calibration_parameters(mlx90632_calibration_parameters *parameters);

/** Reads the object temperature from mlx90632
 *
 * @param[in] device_address
 * @param[out] *temperature

 * @retval 0 for success
 * @retval <0 for failure
 */
int32_t mlx90632_read_temperature(uint8_t device_address,double *temperature);


/** Used on main to read temperatures and show on OLED and Serial
 *
 */
void mlx90632_ambiente_loop();

///@}
#endif
