#include "_Plugin_Helper.h"
#ifdef USES_P202

// #######################################################################################################
// ############################# Plugin 202 MCP9808 I2C Temperature Sensor  ##############################
// #######################################################################################################


# include "src/PluginStructs/P202_data_struct.h"

# define PLUGIN_202
# define PLUGIN_ID_202        202
# define PLUGIN_NAME_202       "Environment - MCP9808"
# define PLUGIN_VALUENAME1_202 "Temperature"


boolean Plugin_202(uint8_t function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {
    case PLUGIN_DEVICE_ADD:
    {
      Device[++deviceCount].Number           = PLUGIN_ID_202;
      Device[deviceCount].Type               = DEVICE_TYPE_I2C;
      Device[deviceCount].VType              = Sensor_VType::SENSOR_TYPE_SINGLE;
      Device[deviceCount].Ports              = 0;
      Device[deviceCount].PullUpOption       = false;
      Device[deviceCount].InverseLogicOption = false;
      Device[deviceCount].FormulaOption      = true;
      Device[deviceCount].ValueCount         = 1;
      Device[deviceCount].SendDataOption     = true;
      Device[deviceCount].TimerOption        = true;
      Device[deviceCount].GlobalSyncOption   = true;
      Device[deviceCount].PluginStats        = true;
      break;
    }

    case PLUGIN_GET_DEVICENAME:
    {
      string = F(PLUGIN_NAME_202);
      break;
    }

    case PLUGIN_GET_DEVICEVALUENAMES:
    {
      strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_202));
      break;
    }

    case PLUGIN_I2C_HAS_ADDRESS:
    case PLUGIN_WEBFORM_SHOW_I2C_PARAMS:
    {
      const uint8_t i2cAddressValues[] = { 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F };

      if (function == PLUGIN_WEBFORM_SHOW_I2C_PARAMS) {
        addFormSelectorI2C(F("i2c_addr"), 8, i2cAddressValues, PCONFIG(0));
        // addFormNote(F("ADDR Low=0x18, High=0x1f"));
      } else {
        success = intArrayContains(8, i2cAddressValues, event->Par1);
      }
      break;
    }

    # if FEATURE_I2C_GET_ADDRESS
    case PLUGIN_I2C_GET_ADDRESS:
    {
      event->Par1 = PCONFIG(0);
      success     = true;
      break;
    }
    # endif // if FEATURE_I2C_GET_ADDRESS

    case PLUGIN_WEBFORM_LOAD:
    {
      // addFormNumericBox(F("Altitude [m]"), F("elev"), PCONFIG(1));
      success = true;
      break;
    }

    case PLUGIN_WEBFORM_SAVE:
    {
      // PCONFIG(1) = getFormItemInt(F("elev"));
      PCONFIG(0) = getFormItemInt(F("i2c_addr"));
      success    = true;
      break;
    }

    case PLUGIN_INIT:
    {
      initPluginTaskData(event->TaskIndex, new (std::nothrow) P202_data_struct());
      P202_data_struct *P202_data =
        static_cast<P202_data_struct *>(getPluginTaskData(event->TaskIndex));

      success = (nullptr != P202_data);
      break;
    }

    case PLUGIN_READ:
    {
      P202_data_struct *P202_data =
        static_cast<P202_data_struct *>(getPluginTaskData(event->TaskIndex));

      if (nullptr != P202_data) {
        if (P202_data->begin())
        {
          UserVar[event->BaseVarIndex] = P202_data->readTemperature();

          if (loglevelActiveFor(LOG_LEVEL_INFO)) {
            String log = F("MCP  : Temperature: ");
            log += formatUserVarNoCheck(event->TaskIndex, 0);
            addLogMove(LOG_LEVEL_INFO, log);
          }
          success = true;
        }
      }
      break;
    }
  }
  return success;
}

#endif // USES_P202
