#pragma once

#include <functional>
#include <string>

enum HDPSResult {
  HDPS_RESULT_OK = 0,
  HDPS_RESULT_ERROR = 1,
  HDPS_RESULT_EXECUTION_ERROR = 2,
  HDPS_RESULT_TIMEOUT = 3,
};

class HDPSCommand;

using HDPSCommandResultCallback = std::function<void(const HDPSCommand& command)>;

class HDPSCommand {
public:
  HDPSCommand(const char *command, HDPSCommandResultCallback on_result)
    : command_(command), on_result_(on_result) {}


  void append(char *data);


 protected:
  HDPSCommandResultCallback on_result_;
  const char *command_;
  std::string data_;
  HDPSResult status_;
};
