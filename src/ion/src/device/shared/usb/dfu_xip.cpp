#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/board_shared.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/serial_number.h"
#include <ion.h>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/usb/calculator.h"

namespace Ion {
namespace USB {

void DFU() {
  // Configure the kernel to avoid interrupting DFU protocole except on Back key
  Device::USB::willExecuteDFU();
  Device::USB::Calculator::PollAndReset();
  Device::USB::didExecuteDFU();
}

}
}
