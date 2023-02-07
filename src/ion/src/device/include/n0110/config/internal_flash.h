#ifndef ION_DEVICE_N0110_CONFIG_INTERNAL_FLASH_H
#define ION_DEVICE_N0110_CONFIG_INTERNAL_FLASH_H

#include <stdint.h>

namespace Ion2 {
namespace Device {
namespace InternalFlash {
namespace Config {

constexpr int NumberOfSectors = 4;
constexpr uint32_t SectorAddresses[NumberOfSectors+1] = {
  0x08000000, 0x08004000, 0x08008000, 0x0800C000,
  0x08010000
};

}
}
}
}

#endif

