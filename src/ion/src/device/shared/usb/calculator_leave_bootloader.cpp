#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/include/n0110/config/board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/board.h"
#include <shared-core/drivers/reset.h>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/usb/calculator.h"

namespace Ion {
namespace Device {
namespace USB {

void Calculator::leave(uint32_t leaveAddress) {
  if (leaveAddress == 0 || leaveAddress == Board::Config::InternalFlashOrigin || leaveAddress == Board::Config::BootloaderOrigin) {
    Reset::core();
  }
  Board::executeIfAuthenticated(leaveAddress);
}

}
}
}
