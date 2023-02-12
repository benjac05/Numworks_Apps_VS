#include <shared/drivers/board_shared.h>
#include <config/board.h>

extern "C" {
extern char _external_apps_flash_start;
}

namespace Ion {
namespace Device {
namespace Board {

bool isRunningSlotA() {
  return reinterpret_cast<uint32_t>(&_external_apps_flash_start) < Config::ExternalFlashOrigin + Config::SlotBOffset;
}

}
}
}
