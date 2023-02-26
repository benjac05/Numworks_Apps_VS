#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion.h"

namespace Ion {

uint32_t SVC_ATTRIBUTES random() {
  SVC_RETURNING_R0(SVC_RANDOM, uint32_t)
}

}
