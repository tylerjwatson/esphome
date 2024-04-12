
#include "hdxps_number.h"

namespace esphome {
namespace hdxps {

void HDPSVoltageNumber::control(float voltage) {
  this->parent_->set_voltage(voltage);
}

void HDPSCurrentNumber::control(float current) {
  this->parent_->set_current(current);
}

}  // namespace hdxps
}  // namespace esphome