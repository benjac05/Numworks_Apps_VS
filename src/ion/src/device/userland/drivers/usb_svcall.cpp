#include <ion/usb.h>
#include <ion/authentication.h>
#include <drivers/svcall.h>
#include <drivers/usb.h>
#include <shared/drivers/board_shared.h>

namespace Ion2 {
namespace USB {

bool SVC_ATTRIBUTES isPlugged() {
  SVC_RETURNING_R0(SVC_USB_IS_PLUGGED, bool)
}

}
}

namespace Ion2 {
namespace Device {
namespace USB {

bool SVC_ATTRIBUTES shouldInterruptDFU() {
  SVC_RETURNING_R0(SVC_USB_SHOULD_INTERRUPT, bool)
}

const char * stringDescriptor() {
  if (AuthenticatIon2::clearanceLevel() == Ion2::AuthenticatIon2::ClearanceLevel::NumWorks || AuthenticatIon2::clearanceLevel() == Ion2::AuthenticatIon2::ClearanceLevel::NumWorksAndThirdPartyApps) {
    return Board::isRunningSlotA() ? Config::InterfaceFlashStringDescriptorAuthenticatedSlotA : Config::InterfaceFlashStringDescriptorAuthenticatedSlotB;
  } else {
    return Board::isRunningSlotA() ? Config::InterfaceFlashStringDescriptorThirdPartySlotA : Config::InterfaceFlashStringDescriptorThirdPartySlotB;
  }
}

}
}
}

