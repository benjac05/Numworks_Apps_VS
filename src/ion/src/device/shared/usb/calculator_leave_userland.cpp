#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/include/n0110/config/board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/reset.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/userland_header.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/usb/calculator.h"

extern "C" {
extern char _external_apps_flash_start;
extern char _external_apps_flash_end;
}

namespace Ion {
namespace Device {
namespace USB {

void Calculator::leave(uint32_t leaveAddress) {
  if (leaveAddress >= reinterpret_cast<uint32_t>(&_external_apps_flash_start) && leaveAddress < reinterpret_cast<uint32_t>(&_external_apps_flash_end)) {
    return;
  }
  Board::switchExecutableSlot(leaveAddress);
  USB::didExecuteDFU();
  /* The jump can't be done from switchExecutableSlot since we need to
   * terminate the interruption procedure before jumping. */
  Reset::jump(leaveAddress + sizeof(UserlandHeader));
}

}
}
}
