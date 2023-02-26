#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/authentication.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/board_shared.h"

namespace Ion {
namespace USB {

bool SVC_ATTRIBUTES isPlugged() {
  SVC_RETURNING_R0(SVC_USB_IS_PLUGGED, bool)
}

}
}

namespace Ion {
namespace Device {
namespace USB {

bool SVC_ATTRIBUTES shouldInterruptDFU() {
  SVC_RETURNING_R0(SVC_USB_SHOULD_INTERRUPT, bool)
}

const char * stringDescriptor() {
  if (AuthenticatIon::clearanceLevel() == Ion::AuthenticatIon::ClearanceLevel::NumWorks || AuthenticatIon::clearanceLevel() == Ion::AuthenticatIon::ClearanceLevel::NumWorksAndThirdPartyApps) {
    return Board::isRunningSlotA() ? Config::InterfaceFlashStringDescriptorAuthenticatedSlotA : Config::InterfaceFlashStringDescriptorAuthenticatedSlotB;
  } else {
    return Board::isRunningSlotA() ? Config::InterfaceFlashStringDescriptorThirdPartySlotA : Config::InterfaceFlashStringDescriptorThirdPartySlotB;
  }
}

}
}
}

