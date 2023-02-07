#ifndef ION_DEVICE_USERLAND_DRIVERS_RESET_H
#define ION_DEVICE_USERLAND_DRIVERS_RESET_H

#include <stdint.h>

namespace Ion2 {
namespace Device {
namespace Reset {

void jump(uint32_t jumpIsrVectorAddress);

}
}
}

#endif
