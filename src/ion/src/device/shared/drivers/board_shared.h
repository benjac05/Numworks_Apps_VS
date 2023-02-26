#ifndef ION_DEVICE_SHARED_DRIVERS_BOARD_SHARED_H
#define ION_DEVICE_SHARED_DRIVERS_BOARD_SHARED_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/kernel_header.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/shared/drivers/userland_header.h"
#include <stdint.h>

namespace Ion {
namespace Device {
namespace Board {

KernelHeader * kernelHeader();
UserlandHeader * userlandHeader();
uint32_t userlandStart();
uint32_t userlandEnd();
uint32_t writableSRAMEndAddress();
uint32_t writableSRAMStartAddress();
bool isRunningSlotA();
uint32_t securityLevel();

}
}
}

#endif
