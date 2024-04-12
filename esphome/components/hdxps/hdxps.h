
#pragma once

#include <deque>

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"

#include "command.h"

namespace esphome {
namespace hdxps {

enum HDPSStatus {
  HDPS_STATUS_OVP_SHUTDOWN = 1,
  HDPS_STATUS_OLP_SHUTDOWN = 1 << 1,
  HDPS_STATUS_OTP_SHUTDOWN = 1 << 2,
  HDPS_STATUS_FAN_FAILURE = 1 << 3,
  HDPS_STATUS_AUX_SMPS_FAILURE = 1 << 4,
  HDPS_STATUS_HIGH_TEMP_ALARM = 1 << 5,
  HDPS_STATUS_AC_DERATING = 1 << 5,
  HDPS_STATUS_AC_INPUT_FAILURE = 1 << 5,
};

enum HDPSControlMode {
  HDPS_CONTROL_MODE_LOCAL = 0,
  HDPS_CONTROL_MODE_REMOTE = 1,
  HDPS_CONTROL_MODE_QUERY = 2,
};

enum HDPSPowerMode {
  HDPS_POWER_MODE_LOCAL_POWER_OFF = 0,
  HDPS_POWER_MODE_LOCAL_POWER_ON = 1,
  HDPS_POWER_MODE_REMOTE_POWER_OFF = 2,
  HDPS_POWER_MODE_REMOTE_POWER_ON = 3,

};

class HDPSDevice : public uart::UARTDevice, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  void loop() override;

  void set_online_sensor(binary_sensor::BinarySensor *sensor) { online_sensor_ = sensor; }
  void set_ovp_shutdown_sensor(binary_sensor::BinarySensor *sensor) { ovp_shutdown_sensor_ = sensor; }
  void set_otp_shutdown_sensor(binary_sensor::BinarySensor *sensor) { otp_shutdown_sensor_ = sensor; }
  void set_olp_shutdown_sensor(binary_sensor::BinarySensor *sensor) { olp_shutdown_sensor_ = sensor; }
  void set_fan_failure_sensor(binary_sensor::BinarySensor *sensor) { fan_failure_sensor_ = sensor; }
  void set_smps_failure_sensor(binary_sensor::BinarySensor *sensor) { smps_failure_sensor_ = sensor; }
  void set_high_temp_alarm_sensor(binary_sensor::BinarySensor *sensor) { high_temp_alarm_sensor_ = sensor; }
  void set_ac_derating_sensor(binary_sensor::BinarySensor *sensor) { ac_derating_sensor_ = sensor; }
  void set_ac_input_sensor(binary_sensor::BinarySensor *sensor) { ac_input_sensor_ = sensor; }

  void set_identity_sensor(text_sensor::TextSensor *sensor) { identity_sensor_ = sensor; }

  void set_temperature_sensor(sensor::Sensor *sensor) { temperature_sensor_ = sensor; }
  void set_rated_voltage_sensor(sensor::Sensor *sensor) { rated_voltage_sensor_ = sensor; }
  void set_rated_current_sensor(sensor::Sensor *sensor) { rated_current_sensor_ = sensor; }

  void set_output_voltage_sensor(sensor::Sensor *sensor) { output_voltage_sensor_ = sensor; }
  void set_output_current_sensor(sensor::Sensor *sensor) { output_current_sensor_ = sensor; }

  void set_voltage_number(number::Number *number) { voltage_number_ = number; }
  void set_current_number(number::Number *number) { current_number_ = number; }

  void set_output_enabled_switch(switch_::Switch *output_enabled_switch) {
    output_enabled_switch_ = output_enabled_switch;
  }

  void set_control_switch(switch_::Switch *control_switch) { control_switch_ = control_switch; }

  void set_voltage(float voltage);
  void set_current(float current);
  void set_output_enabled(bool enabled);
  void set_control(bool enabled);

 protected:
  binary_sensor::BinarySensor *online_sensor_{nullptr};
  binary_sensor::BinarySensor *ovp_shutdown_sensor_{nullptr};
  binary_sensor::BinarySensor *otp_shutdown_sensor_{nullptr};
  binary_sensor::BinarySensor *olp_shutdown_sensor_{nullptr};
  binary_sensor::BinarySensor *fan_failure_sensor_{nullptr};
  binary_sensor::BinarySensor *smps_failure_sensor_{nullptr};
  binary_sensor::BinarySensor *high_temp_alarm_sensor_{nullptr};
  binary_sensor::BinarySensor *ac_derating_sensor_{nullptr};
  binary_sensor::BinarySensor *ac_input_sensor_{nullptr};

  text_sensor::TextSensor *identity_sensor_{nullptr};

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *rated_voltage_sensor_{nullptr};
  sensor::Sensor *rated_current_sensor_{nullptr};
  sensor::Sensor *output_voltage_sensor_{nullptr};
  sensor::Sensor *output_current_sensor_{nullptr};

  // set points
  number::Number *voltage_number_{nullptr};
  number::Number *current_number_{nullptr};

  switch_::Switch *output_enabled_switch_{nullptr};
  switch_::Switch *control_switch_{nullptr};

  std::deque<HDPSCommand> commands_;
  char data_[256]{};


  void send_command(std::string command, HDPSCommandResultCallback on_result);
};

}  // namespace hdxps
}  // namespace esphome
