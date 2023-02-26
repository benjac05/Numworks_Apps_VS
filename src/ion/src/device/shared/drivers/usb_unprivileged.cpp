#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/regs/otg.h"

using namespace Ion::Device::Regs;

namespace Ion {
namespace USB {

void clearEnumerationInterrupt() {
  OTG.GINTSTS()->setENUMDNE(true);
}

void enable() {
  // Get out of soft-disconnected state
  OTG.DCTL()->setSDIS(false);
}

void disable() {
  OTG.DCTL()->setSDIS(true);
}

}
}
