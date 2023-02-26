#include "base64.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/include/n0110/config/serial_number.h"
#include "serial_number.h"
#include <ion.h>

namespace Ion {
namespace Device {
namespace SerialNumber {

void copy(char * buffer) {
  const unsigned char * rawUniqueID = reinterpret_cast<const unsigned char *>(Config::UniqueDeviceIDAddress);
  Base64::encode(rawUniqueID, 12, buffer);
  buffer[k_serialNumberLength] = 0;
}

}
}
}
