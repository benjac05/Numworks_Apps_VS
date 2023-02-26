#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/battery.h"

namespace Ion {
namespace Battery {

bool SVC_ATTRIBUTES isCharging() {
  SVC_RETURNING_R0(SVC_BATTERY_IS_CHARGING, bool)
}

Charge SVC_ATTRIBUTES level() {
  SVC_RETURNING_R0(SVC_BATTERY_LEVEL, Charge)
}

float SVC_ATTRIBUTES voltage() {
  SVC_RETURNING_S0(SVC_BATTERY_VOLTAGE, float)
}

}
}
