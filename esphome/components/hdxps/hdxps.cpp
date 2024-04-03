
#include "hdxps.h"

namespace esphome {
namespace hdxps {

static const char *const TAG = "hdxps";


void HDPSDevice::send_command(const char *command, HDPSCommandResultCallback on_result) {
  HDPSCommand cmd{"*IDN?", on_result};

  this->write_str(command);
  this->write_str("\r\n");
  this->commands_.push_back(std::move(cmd));

  ESP_LOGD(TAG, "HDPSDevice::send_command: %s", command);
}

void HDPSDevice::setup() {
  ESP_LOGD(TAG, "HDPSDevice::setup");

  // This is the setup function that will be called by the framework
  // when the component is being initialized.

  check_uart_settings(4800, 1, uart::UART_CONFIG_PARITY_NONE, 8);

}

void HDPSDevice::update() {
  // This is the update function that will be called by the framework
  // when the component should update.
  ESP_LOGD(TAG, "HDPSDevice::update available=%d", this->available());
}

void HDPSDevice::dump_config() {
  // This will be called by the framework when the user requests the
  // configuration of the component. It will be printed to the logs.

  ESP_LOGCONFIG(TAG, "XP Power HDxPS device");
  ESP_LOGCONFIG(TAG, "Using UART baud rate %d", this->parent_->get_baud_rate());

  ESP_LOGCONFIG(TAG, "Setpoints:");
  ESP_LOGCONFIG(TAG, "  Voltage: %f.2 V", this->voltage_);
  ESP_LOGCONFIG(TAG, "  Current: %f.2 A", this->current_);
}

void HDPSDevice::loop() {
  // This will be called by the framework in the main loop.
  auto av = this->available();
  auto timeout = false;

  if (av) {
    static char data[256];

    if (!this->read_array(reinterpret_cast<uint8_t *>(data), std::min(av, 256))) {
      timeout = true;
    }

    if (this->commands_.empty()) {
      ESP_LOGW(TAG, "HDPSDevice::loop: received data but no command pending");
      return;
    }

    auto& cmd = this->commands_.front();

    if (timeout) {
      ESP_LOGE(TAG, "HDPSDevice::loop: uart read timeout, discarding command.");
      this->commands_.pop_front();
      cmd.error(HDPS_RESULT_TIMEOUT);
      return;
    }

    cmd.append(data);

    if (!this->available()) {
      this->commands_.pop_front();
      cmd.done();
    }

    // append data to running command
    return;
  }


}

int HDPSDevice::on_data(const char *command, char *data) {


  /*
  XP Power Ltd HDS1500PS60 V1
  =>
  */

  ESP_LOGD(TAG, "HDPSDevice::on_data: command=%s, data=%s", command, data);



  if (!strcmp(result, "?>")) {
    ESP_LOGE(TAG, "HDPSDevice::on_data: %s Invalid command.", command, result);
    return -1;
  } else if (!strcmp(result, "!>")) {
    ESP_LOGE(TAG, "HDPSDevice::on_data: %s: execution error, possibly bad parameter or out of range.", command, result);
    return -1;
  } else if (strcmp(result, "=>")) {
    ESP_LOGE(TAG, "HDPSDevice::on_data: %s: unknown command result %s", command, result);
    return -1;
  }

  if (strcmp(command, "*IDN?") == 0) {
    strcpy(this->manufacturer_, query);
    this->device_found_ = true;
    if (this->online_sensor_) {
      this->online_sensor_->publish_state(true);
    }
    ESP_LOGI(TAG, "HDPSDevice::on_data: Found SMPS: %s", this->manufacturer_);

    if (!this->serial_[0]) {
      this->reset_command();
      this->send_command("INFO 5");
      return -2;
    }
  }

  // Serial number
  if (!strcmp(command, "INFO 5")) {
    strcpy(this->serial_, query);
    if (this->serial_sensor_) {
      this->serial_sensor_->publish_state(this->serial_);
    }
    ESP_LOGI(TAG, "HDPSDevice::on_data: Serial number: %s", this->serial_);
  }

  return 0;
}

}  // namespace hdxps
}  // namespace esphome
