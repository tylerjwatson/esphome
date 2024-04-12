#pragma once

#include "esphome/components/switch/switch.h"
#include "../hdxps.h"

namespace esphome {
namespace hdxps {

class HDPSDevice;

class HDPSOutputEnabledSwitch : public switch_::Switch, public Parented<HDPSDevice> {
 public:
  HDPSOutputEnabledSwitch() = default;

  protected:
  void write_state(bool state) override;
};

class HDPSControlSwitch : public switch_::Switch, public Parented<HDPSDevice> {
 public:
  HDPSControlSwitch() = default;

  protected:
  void write_state(bool state) override;
};

}  // namespace hdxps
}  // namespace esphome