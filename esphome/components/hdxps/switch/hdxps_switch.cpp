
#include "hdxps_switch.h"

namespace esphome {
namespace hdxps {

void HDPSOutputEnabledSwitch::write_state(bool state) { this->parent_->set_output_enabled(state); }

void HDPSControlSwitch::write_state(bool state) { this->parent_->set_control(state); }

}  // namespace hdxps
}  // namespace esphome