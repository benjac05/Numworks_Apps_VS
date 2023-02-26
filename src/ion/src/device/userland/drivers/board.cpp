#include "board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"

namespace Ion {
namespace Device {
namespace Board {

void SVC_ATTRIBUTES switchExecutableSlot(uint32_t address) {
  SVC_RETURNING_VOID(SVC_BOARD_SWITCH_EXECUTABLE_SLOT);
}

void SVC_ATTRIBUTES enableExternalApps() {
  SVC_RETURNING_VOID(SVC_BOARD_ENABLE_EXTERNAL_APPS);
}

}
}
}
