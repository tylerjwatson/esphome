#pragma once

#include <functional>
#include <string>
#include <Arduino.h>

namespace esphome {
namespace hdxps {

enum HDPSResult {
  HDPS_RESULT_OK = 0,
  HDPS_RESULT_ERROR = 1,
  HDPS_RESULT_EXECUTION_ERROR = 2,
  HDPS_RESULT_TIMEOUT = 3,
  HDPS_RESULT_CONTINUE,
};

class HDPSCommand;

using HDPSCommandResultCallback = std::function<void(const HDPSResult result, const std::string &data)>;

struct HDPSCommand {
  bool sent_{false};
  int deadline_{0};
  std::string command_;
  std::string data_;

  HDPSCommand(const std::string&& command, HDPSCommandResultCallback on_result)
      : deadline_(millis() + 450), command_(command), on_result_(on_result) {}

  HDPSResult append(char *data, size_t len);
  void result(HDPSResult r);

 protected:
  HDPSCommandResultCallback on_result_;

};

}  // namespace hdxps
}  // namespace esphome