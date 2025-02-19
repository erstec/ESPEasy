#ifndef PLUGINSTRUCTS_P202_DATA_STRUCT_H
#define PLUGINSTRUCTS_P202_DATA_STRUCT_H

#include "../../_Plugin_Helper.h"
#ifdef USES_P202

struct P202_data_struct : public PluginTaskData_base {
public:

  P202_data_struct(uint8_t i2c_addr);

  P202_data_struct() = delete;
  virtual ~P202_data_struct() = default;

  bool     begin();

  float    readTemperature();

private:
  int32_t set_config(uint8_t reg, uint16_t cfg);
  int32_t set_upper_limit(uint8_t reg, uint16_t cfg);
  int32_t set_lower_limit(uint8_t reg, uint16_t cfg);
  int32_t set_critical_limit(uint8_t reg, uint16_t cfg);
  int32_t read_temp_reg(uint8_t reg, uint16_t *temp);
  int32_t set_resolution(uint8_t reg, uint8_t resolution);
  float caculate_temp(uint16_t temp);

  bool initialized = false;

  uint8_t i2caddr;
};

#endif // ifdef USES_P202
#endif // ifndef PLUGINSTRUCTS_P202_DATA_STRUCT_H
