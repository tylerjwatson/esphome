
#include "hdxps.h"

namespace esphome {
namespace hdxps {

extern const char *const TAG = "hdxps";

void HDPSDevice::send_command(std::string command, HDPSCommandResultCallback on_result) {
  HDPSCommand cmd(std::move(command), on_result);

  this->commands_.push_back(std::move(cmd));

  ESP_LOGD(TAG, "HDPSDevice::send_command: %s, %d in queue", command.c_str(), this->commands_.size());
}

void HDPSDevice::setup() {
  ESP_LOGD(TAG, "HDPSDevice::setup");

  // This is the setup function that will be called by the framework
  // when the component is being initialized.

  check_uart_settings(4800, 1, uart::UART_CONFIG_PARITY_NONE, 8);

  // dummy write, esp8266 serial seems to not write the first ever
  // byte properly
  // the unit will ignore nonsense commands
  this->write_str("TEST\r\n");
  this->flush();

  auto initial_remote = this->control_switch_->get_initial_state_with_restore_mode().value_or(false);
  if (initial_remote) {
    this->set_control(true);
  }
}

void HDPSDevice::update() {
  // This is the update function that will be called by the framework
  // when the component should update.
  ESP_LOGI(TAG, "HDPSDevice::update: refreshing device");

  auto power_response = [=](auto result, const auto &data) {
    auto int_val = std::strtol(data.c_str(), nullptr, 10);
    auto enabled = int_val == HDPS_POWER_MODE_LOCAL_POWER_ON || int_val == HDPS_POWER_MODE_REMOTE_POWER_ON;
    this->output_enabled_switch_->publish_state(enabled);

    ESP_LOGI(TAG, "HDPSDevice::update: finished.");
  };

  auto rv_response = [=](auto result, const auto &data) {
    auto val = std::strtof(data.c_str(), nullptr);

    if (this->output_voltage_sensor_) {
      this->output_voltage_sensor_->publish_state(val);
    }

    this->send_command("POWER 2", power_response);
  };

  auto ri_response = [=](auto result, const auto &data) {
    auto val = std::strtof(data.c_str(), nullptr);

    if (this->output_current_sensor_) {
      this->output_current_sensor_->publish_state(val);
    }

    this->send_command("RV?", rv_response);
  };

  auto si_response = [=](auto result, const auto &data) {
    auto val = std::strtof(data.c_str(), nullptr);
    this->current_number_->publish_state(val);

    this->send_command("RI?", ri_response);
  };

  auto sv_response = [=](auto result, const auto &data) {
    auto val = std::strtof(data.c_str(), nullptr);
    this->voltage_number_->publish_state(val);

    this->send_command("SI?", si_response);
  };

  auto rems_response = [=](auto result, const auto &data) {
    auto int_val = std::strtol(data.c_str(), nullptr, 10);
    this->control_switch_->publish_state(int_val == HDPS_CONTROL_MODE_REMOTE);

    this->send_command("SV?", sv_response);
  };

  auto rate_response = [=](auto result, const auto &data) {
    char *end{};
    auto voltage = std::strtol(data.c_str(), &end, 10);
    auto current = std::strtol(end, nullptr, 10);

    if (this->rated_voltage_sensor_) {
      this->rated_voltage_sensor_->publish_state(voltage);
    }

    if (this->rated_current_sensor_) {
      this->rated_current_sensor_->publish_state(current);
    }

    this->send_command("REMS 2", rems_response);
  };

  auto rt_response = [=](auto result, const auto &data) {
    char *ptr;
    auto temp = std::strtof(data.c_str(), &ptr);

    if (this->temperature_sensor_) {
      this->temperature_sensor_->publish_state(temp);
    }

    this->send_command("RATE?", rate_response);
  };

  auto stus_response = [=](HDPSResult res, const std::string &data) {
    char *ptr;
    auto status = std::strtoul(data.c_str(), &ptr, 16);

    if (this->ovp_shutdown_sensor_) {
      this->ovp_shutdown_sensor_->publish_state(status & HDPS_STATUS_OVP_SHUTDOWN);
    }

    if (this->olp_shutdown_sensor_) {
      this->olp_shutdown_sensor_->publish_state(status & HDPS_STATUS_OLP_SHUTDOWN);
    }

    if (this->otp_shutdown_sensor_) {
      this->otp_shutdown_sensor_->publish_state(status & HDPS_STATUS_OTP_SHUTDOWN);
    }

    if (this->fan_failure_sensor_) {
      this->fan_failure_sensor_->publish_state(status & HDPS_STATUS_FAN_FAILURE);
    }

    if (this->smps_failure_sensor_) {
      this->smps_failure_sensor_->publish_state(status & HDPS_STATUS_AUX_SMPS_FAILURE);
    }

    if (this->high_temp_alarm_sensor_) {
      this->high_temp_alarm_sensor_->publish_state(status & HDPS_STATUS_HIGH_TEMP_ALARM);
    }

    if (this->ac_derating_sensor_) {
      this->ac_derating_sensor_->publish_state(status & HDPS_STATUS_AC_DERATING);
    }

    if (this->ac_input_sensor_) {
      this->ac_input_sensor_->publish_state(status & HDPS_STATUS_AC_INPUT_FAILURE);
    }

    this->send_command("RT?", rt_response);
  };

  auto idn_response = [=](HDPSResult res, const std::string &data) {
    {
      // ESP_LOGD(TAG, "identity result %s", data.c_str());

      if (this->identity_sensor_) {
        this->identity_sensor_->publish_state(data);
      }

      this->send_command("STUS 0", stus_response);
    }
  };

  this->send_command("*IDN?", idn_response);
}

void HDPSDevice::dump_config() {
  // This will be called by the framework when the user requests the
  // configuration of the component. It will be printed to the logs.

  ESP_LOGCONFIG(TAG, "XP Power HDxPS device");
  ESP_LOGCONFIG(TAG, "Using UART baud rate %d", this->parent_->get_baud_rate());
  if (this->control_switch_->get_initial_state().has_value()) {
    ESP_LOGCONFIG(TAG, "Control switch initial state: '%s'",
                  this->control_switch_->get_initial_state().value() ? "remote" : "local");
  }
}

void HDPSDevice::loop() {
  // This will be called by the framework in the main loop.
  auto timeout = false;
  auto av = this->available();

  if (av) {
    // ESP_LOGD(TAG, "%d available", av);

    if (!this->read_array(reinterpret_cast<uint8_t *>(this->data_), std::min(av, 255))) {
      timeout = true;
    }
  }

  if (!this->commands_.empty()) {
    auto &cmd = this->commands_.front();

    if (timeout || millis() > cmd.deadline_) {
      ESP_LOGW(TAG, "Command %s timeout.", cmd.command_.c_str());
      this->commands_.pop_front();
      return;
    }

    if (!cmd.sent_) {
      this->write_str(cmd.command_.c_str());
      this->write_str("\r\n");
      cmd.sent_ = true;
      return;
    }

    HDPSResult res = HDPS_RESULT_CONTINUE;
    if (av) {
      res = cmd.append(this->data_, std::min(av, 255));
    }

    switch (res) {
      case HDPS_RESULT_OK: {
        auto cmd = std::move(this->commands_.front());
        this->commands_.pop_front();
        cmd.result(HDPS_RESULT_OK);
      }

      break;
      case HDPS_RESULT_ERROR:
      case HDPS_RESULT_EXECUTION_ERROR:
        ESP_LOGE(TAG, "cmd %s execution error.");
        this->commands_.pop_front();
        break;
      case HDPS_RESULT_CONTINUE:
      default:
        break;
    }
  }
}

void HDPSDevice::set_voltage(float voltage) {
  char buf[32]{};
  ESP_LOGD(TAG, "set_voltage %f", voltage);
  snprintf(buf, 32, "SV %.1f", voltage);
  auto b = std::string(buf);

  this->send_command(std::move(b), [=](auto result, const auto &data) {
    if (this->voltage_number_) {
      this->voltage_number_->publish_state(voltage);
    }
  });
}

void HDPSDevice::set_current(float current) {
  char buf[32]{};
  ESP_LOGD(TAG, "set_current %f", current);
  snprintf(buf, 32, "SI %.1f", current);
  auto b = std::string(buf);

  this->send_command(std::move(b), [=](auto result, const auto &data) {
    if (this->current_number_) {
      this->current_number_->publish_state(current);
    }
  });
}

void HDPSDevice::set_output_enabled(bool enabled) {
  ESP_LOGD(TAG, "set_output_enabled %d", enabled);

  this->send_command(enabled ? "POWER 1" : "POWER 0", [this](auto result, const auto &data) {
    if (this->output_enabled_switch_) {
      this->output_enabled_switch_->publish_state(result == HDPS_RESULT_OK);
    }
  });
}

void HDPSDevice::set_control(bool enabled) {
  ESP_LOGD(TAG, "set_control %d", enabled);

  this->send_command(enabled ? "REMS 1" : "REMS 0", [this](auto result, const auto &data) {
    if (this->control_switch_) {
      this->control_switch_->publish_state(result == HDPS_RESULT_OK);
    }
  });
}

}  // namespace hdxps
}  // namespace esphome
