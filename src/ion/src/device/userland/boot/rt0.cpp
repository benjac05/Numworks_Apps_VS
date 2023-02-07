#include "isr.h"
#include <ion.h>
#include <config/board.h>
#include <shared/boot/rt0.h>
#include <shared/drivers/usb.h>

extern "C" {
  void abort();
}

void abort() {
#if DEBUG
  while (1) {
  }
#else
  Ion2::Reset::core();
#endif
}

void __attribute__((noinline)) start() {
  Ion2::Device::Init::configureRAM();
  // Initialize slotInfo to be accessible to Kernel
  Ion2::Device::USB::slotInfo();
  Ion2::ExternalApps::deleteApps();
  ion_main(0, nullptr);
  abort();
}
