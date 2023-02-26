#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/external_apps.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/board_shared.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/usb.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"

namespace Ion {
namespace Device {
namespace USB {

void SVC_ATTRIBUTES willExecuteDFU() {
  SVC_RETURNING_VOID(SVC_USB_WILL_EXECUTE_DFU)

  // Keep usefull information about the currently running slot
  slotInfo()->updateUserlandHeader();
}

void SVC_ATTRIBUTES didExecuteDFU() {
  if (Ion::ExternalApps::numberOfApps() > 0) {
    Board::enableExternalApps(); // Display pop-up
  }
  SVC_RETURNING_VOID(SVC_USB_DID_EXECUTE_DFU)
}

SlotInfo * slotInfo() {
  static SlotInfo __attribute__((used)) __attribute__((section(".slot_info"))) slotInformation;
  return &slotInformation;
}

void SlotInfo::updateUserlandHeader() {
  m_userlandHeaderAddress = Board::userlandHeader();
}

}
}
}
