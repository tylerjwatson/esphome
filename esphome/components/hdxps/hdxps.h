
#pragma once

#include <deque>

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/sensor/sensor.h"

#include "command.h"

namespace esphome {
namespace hdxps {




class HDPSDevice : public uart::UARTDevice, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  void loop() override;

  void set_online_sensor(binary_sensor::BinarySensor *online_sensor) { online_sensor_ = online_sensor; }
  void set_serial_sensor(text_sensor::TextSensor *serial_sensor) { serial_sensor_ = serial_sensor; }

  void set_voltage(float voltage) { voltage_ = voltage; }
  void set_current(float current) { current_ = current; }
  void set_output_enabled(bool output_enabled) { output_enabled_ = output_enabled; }

 protected:
  binary_sensor::BinarySensor *online_sensor_{nullptr};
  text_sensor::TextSensor *serial_sensor_{nullptr};

  std::deque<HDPSCommand> commands_;

  void send_command(const char *command, HDPSCommandResultCallback on_result);

  float voltage_{0.0};
  float current_{0.0};
  bool output_enabled_{false};
  bool online_{false};

  // internal device info
  bool device_found_{false};
  char manufacturer_[32]{};
  char model_name_[32]{};
  char output_voltage_[32]{};
  char mfg_date_[32]{};
  char serial_[32]{};
  char mfg_location_[32]{};
};

}  // namespace hdxps
}  // namespace esphome
