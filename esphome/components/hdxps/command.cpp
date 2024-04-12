#include "command.h"

#include <string.h>
#include <esphome/core/log.h>

namespace esphome {
namespace hdxps {

extern const char *const TAG;

HDPSResult HDPSCommand::append(char *data, size_t len) {
  auto res = HDPS_RESULT_CONTINUE;

  if (!data) {
    return res;
  }

  this->data_.append(data, len);

  auto pos = this->data_.find(">\r\n");
  if (pos != std::string::npos) {
    auto result_chr = this->data_.at(pos - 1 < 0 ? 0 : pos - 1);

    // ESP_LOGD(TAG, "d is %c", result_chr);
    switch (result_chr) {
      case '!':
        res = HDPS_RESULT_ERROR;
        break;
      case '=':
        res = HDPS_RESULT_OK;
        break;
      case '?':
        res = HDPS_RESULT_EXECUTION_ERROR;
        break;
      default:
        res = HDPS_RESULT_CONTINUE;
    }

    if (pos > 3) {
      this->data_.erase(std::max(pos - 3, 0U));
    }
  }

  return res;
}

void HDPSCommand::result(HDPSResult res) { on_result_(res, data_); }

}  // namespace hdxps
}  // namespace esphome