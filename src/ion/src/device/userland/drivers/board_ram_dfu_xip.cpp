#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/include/n0110/config/board.h"

namespace Ion {
namespace Device {
namespace Board {

/* N0110/N0120 userland DFU cannot write the SRAM allocated for the kernel or
 * its own process stack. */

uint32_t writableSRAMStartAddress() {
  return Config::UserlandSRAMOrigin;
}

uint32_t writableSRAMEndAddress() {
  return Config::UserlandSRAMOrigin + Config::UserlandSRAMLength - Config::UserlandStackLength;
}

}
}
}

