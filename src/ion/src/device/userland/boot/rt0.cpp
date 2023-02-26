#include "isr.h"
#include <ion.h>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/include/n0110/config/board.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/boot/rt0.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/usb.h"

extern "C" {
  void abort();
}

void abort() {
#if DEBUG
  while (1) {
  }
#else
  Ion::Reset::core();
#endif
}

void __attribute__((noinline)) start() {
  Ion::Device::Init::configureRAM();
  // Initialize slotInfo to be accessible to Kernel
  Ion::Device::USB::slotInfo();
  Ion::ExternalApps::deleteApps();
  ion_main(0, nullptr);
  abort();
}
