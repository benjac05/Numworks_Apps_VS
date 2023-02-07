#include "platform_device_capability_descriptor.h"

namespace Ion2 {
namespace Device {
namespace USB {

void PlatformDeviceCapabilityDescriptor::push(Channel * c) const {
  DeviceCapabilityDescriptor::push(c);
  c->push(m_bReserved);
  for (int i = 0; i < k_platformCapabilityUUIDSize; i++) {
    c->push(m_platformCapabilityUUID[i]);
  }
}

}
}
}
