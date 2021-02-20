#include "mlx90632_depends.h"
static uint8_t mlx90632_address = 0x3A << 1; //Default address

int32_t mlx90632_i2c_read(int16_t register_address, uint16_t *value)
{
    uint8_t d[2];
    int ret = HAL_I2C_Mem_Read(&hi2c1, mlx90632_address, register_address,
                               I2C_MEMADD_SIZE_16BIT, d, 2, 50);
    if (ret != HAL_OK)
        return -ret;
    *value = d[0];
    *value = *value << 8;
    *value |= d[1];
    HAL_Delay(1);
    return 0;
}

int32_t mlx90632_i2c_write(int16_t register_address, uint16_t value)
{
    uint8_t d[2];
    d[0] = value >> 8;
    d[1] = value;
    if (HAL_I2C_Mem_Write(&hi2c1, mlx90632_address, register_address,
                          I2C_MEMADD_SIZE_16BIT, d, 2, 50) != HAL_OK)
        return -1;
    return 0;
}

void usleep(int min_range, int max_range)
{
    if (min_range < 1000)
    {
        HAL_Delay(1);
    }
    else
    {
        uint32_t delay = (uint32_t)min_range / 1000;
        HAL_Delay(delay);
    }
}

void msleep(int msecs)
{
    HAL_Delay(msecs);
}

int32_t mlx90632_i2c_read_int32(uint16_t register_address, int32_t *value)
{
    uint8_t d[2];
    if (HAL_I2C_Mem_Read(&hi2c1, mlx90632_address, register_address + 1,
                         I2C_MEMADD_SIZE_16BIT, d, 2, 50) != HAL_OK)
        return -1;
    *value = d[0];
    *value = *value << 8;
    *value |= d[1];
    *value = *value << 8;
    //HAL_UART_Transmit(&huart1, (uint8_t*) d, 2, 10);
    if (HAL_I2C_Mem_Read(&hi2c1, mlx90632_address, register_address,
                         I2C_MEMADD_SIZE_16BIT, d, 2, 50) != HAL_OK)
        return -1;
    *value |= d[0];
    *value = *value << 8;
    *value |= d[1];
    //HAL_UART_Transmit(&huart1, (uint8_t*) d, 2, 10);
    return 0;
}

int32_t mlx90632_i2c_read_int16(uint16_t register_address, int16_t *value)
{
    uint8_t d[2];
    if (HAL_I2C_Mem_Read(&hi2c1, mlx90632_address, register_address,
                         I2C_MEMADD_SIZE_16BIT, d, 2, 5000) != HAL_OK)
        return -1;
    *value = d[0];
    *value = *value << 8;
    *value |= d[1];
    //HAL_UART_Transmit(&huart1, (uint8_t*) d, 2, 10);
    return 0;
}
