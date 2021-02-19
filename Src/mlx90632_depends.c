#include "mlx90632_depends.h"

static uint8_t mlx90632_address = MLX90632_ADDRESS_EAR << 1;

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

int32_t mlx90632_get_calibration_parameters(mlx90632_calibration_parameters *parameters)
{
    int32_t ret, tmp32;
    int16_t tmp16;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_P_R, &tmp32);
    if (ret < 0)
        return ret;
    parameters->P_R = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_P_G, &tmp32);
    if (ret < 0)
        return ret;
    parameters->P_G = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_P_T, &tmp32);
    if (ret < 0)
        return ret;
    parameters->P_T = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_P_O, &tmp32);
    if (ret < 0)
        return ret;
    parameters->P_O = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_Ea, &tmp32);
    if (ret < 0)
        return ret;
    parameters->Ea = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_Eb, &tmp32);
    if (ret < 0)
        return ret;
    parameters->Eb = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_Fa, &tmp32);
    if (ret < 0)
        return ret;
    parameters->Fa = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_Fb, &tmp32);
    if (ret < 0)
        return ret;
    parameters->Fb = tmp32;
    ret = mlx90632_i2c_read_int32(MLX90632_EE_Ga, &tmp32);
    if (ret < 0)
        return ret;
    parameters->Ga = tmp32;
    ret = mlx90632_i2c_read_int16(MLX90632_EE_Gb, &tmp16);
    if (ret < 0)
        return ret;
    parameters->Gb = tmp16;
    ret = mlx90632_i2c_read_int16(MLX90632_EE_Ka, &tmp16);
    if (ret < 0)
        return ret;
    parameters->Ka = tmp16;
    ret = mlx90632_i2c_read_int16(MLX90632_EE_Ha, &tmp16);
    if (ret < 0)
        return ret;
    parameters->Ha = tmp16;
    ret = mlx90632_i2c_read_int16(MLX90632_EE_Hb, &tmp16);
    if (ret < 0)
        return ret;
    parameters->Hb = tmp16;
    return 0;
}

int32_t mlx90632_get_raw_ambient_temperature(int16_t *ram6, int16_t *ram9)
{
    int16_t tmp;
    int32_t ret;

    ret = mlx90632_i2c_read((uint16_t)0x4005, &tmp);
    *ram6 = tmp;
    if (ret < 0)
        return ret;
    msleep(1);
    //print2bytes(ram6);

    ret = mlx90632_i2c_read((uint16_t)0x4008, &tmp);
    *ram9 = tmp;
    if (ret < 0)
        return ret;
    msleep(1);
    //print2bytes(ram9);
    return 0;
}

double mlx90632_pre_calculate_ambient_temperature(int16_t ram6, int16_t ram9, int16_t Gb)
{
    double VR_Ta, kGb;

    kGb = ((double)Gb) / 1024.0;

    VR_Ta = ram9 + kGb * (ram6 / (12));
    return ((ram6 / (12)) / VR_Ta) * 524288.0;
}

double mlx90632_calculate_ambient_temperature(int16_t ram6, int16_t ram9, int32_t P_T, int32_t P_R, int32_t P_G, int32_t P_O, int16_t Gb)
{
    double Asub, Bsub, Ablock, Bblock, Cblock, AMB;

    AMB = mlx90632_pre_calculate_ambient_temperature(ram6, ram9, Gb);

    Asub = ((double)P_T) / (double)17592186044416.0;
    Bsub = (double)AMB - ((double)P_R / (double)256.0);
    Ablock = Asub * (Bsub * Bsub);
    Bblock = (Bsub / (double)P_G) * (double)1048576.0;
    Cblock = (double)P_O / (double)256.0;

    return Bblock + Ablock + Cblock;
}

int32_t mlx90632_read_temperature(uint8_t device_address, double *temperature)
{
    mlx90632_calibration_parameters parameters;
    double ambient; /**< Ambient temperature in degrees Celsius */
    double object;  /**< Object temperature in degrees Celsius */
    int ret, tries = MLX90632_MAX_NUMBER_MESUREMENT_READ_TRIES;
    uint16_t reg_status;
    int16_t ram6, ram9, amb;
    uint16_t channel;

    ret = mlx90632_get_calibration_parameters(&parameters);
    if (ret < 0)
        return ret;

    ret = mlx90632_i2c_read(MLX90632_REG_STATUS, &reg_status);
    if (ret < 0)
        return ret;

    ret = mlx90632_i2c_write(MLX90632_REG_STATUS,
                             reg_status & (~MLX90632_STAT_DATA_RDY));
    if (ret < 0)
        return ret;

    while (tries-- > 0)
    {
        ret = mlx90632_i2c_read(MLX90632_REG_STATUS, &reg_status);
        if (ret < 0)
            return ret;
        if ((reg_status & MLX90632_STAT_DATA_RDY))
        {
            HAL_WWDG_Refresh(&hwwdg);
            msleep(1);
            break;
        }
        HAL_WWDG_Refresh(&hwwdg);
        msleep(10);
    }

    if (tries < 0)
    {
        // data not ready
        return -ETIMEDOUT;
    }
    channel = (reg_status & 0x007F) >> 2;

    ret = mlx90632_get_raw_ambient_temperature(&ram6, &ram9);

    ambient = mlx90632_calculate_ambient_temperature(ram6, ram9, parameters.P_T, parameters.P_R, parameters.P_G, parameters.P_O, parameters.Gb);
    
    uart_print("TA: %.2lf\r\n", ambient);

    return 0;
}

void mlx90632_ambiente_loop()
{
    double temperatura;
    mlx90632_read_temperature(0x3A << 1, &temperatura);
}