#pragma once

#include "esphome/components/number/number.h"
#include "../hdxps.h"

namespace esphome {
namespace hdxps {

class HDPSDevice;

class HDPSVoltageNumber : public number::Number, public Parented<HDPSDevice> {
 public:
  HDPSVoltageNumber() = default;

 protected:
  void control(float voltage) override;
};

class HDPSCurrentNumber : public number::Number, public Parented<HDPSDevice> {
 protected:
  void control(float current) override;
};

}  // namespace hdxps
}  // namespace esphome