#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/reset.h"
#include <shared-core/drivers/board.h>
#include <shared-core/drivers/reset.h>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/usb/calculator.h"

namespace Ion {
namespace Device {
namespace USB {

/* This implementation is used by the flasher, which does not need to handle
 * privilege escalation */
void Calculator::leave(uint32_t leaveAddress) {
  Board::shutdown();
  Reset::jump(leaveAddress);
}

}
}
}
