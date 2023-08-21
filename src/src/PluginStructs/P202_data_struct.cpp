#include "../PluginStructs/P202_data_struct.h"

#ifdef USES_P202

#define SET_CONFIG_ADDR                     (0x01)
#define SET_UPPER_LIMIT_ADDR                (0x02)
#define SET_LOWER_LIMIT_ADDR                (0x03)
#define SET_CRITICAL_LIMIT_ADDR             (0x04)

#define AMBIENT_TEMPERATURE_ADDR            (0x05)
#define SET_RESOLUTION_ADDR                 (0x08)

#define RESOLUTION_0_5_DEGREE               (0x00)
#define RESOLUTION_0_25_DEGREE              (0x01)
#define RESOLUTION_0_125_DEGREE             (0x02)
#define RESOLUTION_0_0625_DEGREE            (0x03)
#define SIGN_BIT                            (0x10)

P202_data_struct::P202_data_struct(uint8_t i2c_addr) : i2caddr(i2c_addr) {}

bool P202_data_struct::begin()
{
  if (!initialized) {
    if (set_resolution(AMBIENT_TEMPERATURE_ADDR, RESOLUTION_0_25_DEGREE)) {
      initialized = true;
      return true;
    } else {
      return false;
    }
  }

  return true;
}

float P202_data_struct::readTemperature()
{
  uint16_t temp_value = 0;
  read_temp_reg(AMBIENT_TEMPERATURE_ADDR, &temp_value);
  float temp = caculate_temp(temp_value);
  return temp;
}

// -----------------------------------------------------------------------------------------------

/**@brief Set configuration of sensor.
 * @param Register addr of sensor configuration.
 * @param The value to set.
 * @return 0 if success;
 * 
 * */
int32_t P202_data_struct::set_config(uint8_t reg, uint16_t cfg)
{
    return I2C_write16_reg(i2caddr, reg, cfg);
}

/**@brief Set upper limit of sensor.
 * @param Register addr of upper limit.
 * @param The value to set.
 * @return 0 if success;
 * */
int32_t P202_data_struct::set_upper_limit(uint8_t reg, uint16_t cfg)
{
    return I2C_write16_reg(i2caddr, reg, cfg);
}

/**@brief Set lower limit of sensor.
 * @param Register addr of lower limit.
 * @param The value to set.
 * @return 0 if success;
 * */
int32_t P202_data_struct::set_lower_limit(uint8_t reg, uint16_t cfg)
{
    return I2C_write16_reg(i2caddr, reg, cfg);
}

/**@brief Set critical limit of sensor.
 * @param Register addr of critical limit.
 * @param The value to set.
 * @return 0 if success;
 * */
int32_t P202_data_struct::set_critical_limit(uint8_t reg, uint16_t cfg)
{
    return I2C_write16_reg(i2caddr, reg, cfg);
}

/**@brief Set resolution of sensor.range:0,0.25,0.125,0.0625
 * @param Register addr of resolution.
 * @param The value to set.
 * @return 0 if success;
 * */
int32_t P202_data_struct::set_resolution(uint8_t reg, uint8_t resolution)
{
    return I2C_write8_reg(i2caddr, reg, resolution);
}


/**@brief caculate u16 data to a float temp num;
 * @param register addr of ambient temp.
 * @param dst temp data.
 * */
int32_t P202_data_struct::read_temp_reg(uint8_t reg, uint16_t *temp)
{
    *temp = I2C_read16_reg(i2caddr, reg);
    return 0;
}

/**@brief caculate u16 data to a float temp num;
 * @param u16-form data.
 * @return float-form temp data.
 * */
float P202_data_struct::caculate_temp(uint16_t temp_value)
{
    float temp = 0;
    uint8_t temp_upper = 0, temp_lower = 0;
    temp_upper = (uint8_t)(temp_value >> 8);
    temp_lower = (uint8_t)temp_value;

    temp_upper &= 0x1f;           // Clear flag bits
    if (temp_upper & SIGN_BIT) {  // Check SIGN
        temp_upper &= 0x0f;       // Clear SIGN
        temp = 256 - (temp_upper * 16 + temp_lower * 0.0625);
        temp *= -1;
    } else {
      temp = temp_upper * 16 + temp_lower * 0.0625;
    }

    return temp;
}

#endif // ifdef USES_P202
