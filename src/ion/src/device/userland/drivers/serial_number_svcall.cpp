#include <ion.h>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"

namespace Ion {
namespace Device {
namespace SerialNumber {

void SVC_ATTRIBUTES copy(char * buffer) {
  SVC_RETURNING_VOID(SVC_SERIAL_NUMBER_COPY)
}

}
}
}
