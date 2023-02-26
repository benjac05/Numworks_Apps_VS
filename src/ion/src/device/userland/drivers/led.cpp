#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/led.h"

namespace Ion {
namespace LED {

KDColor SVC_ATTRIBUTES getColor() {
  SVC_RETURNING_R0(SVC_LED_GET_COLOR, KDColor)
}

void SVC_ATTRIBUTES setColor(KDColor c) {
  SVC_RETURNING_VOID(SVC_LED_SET_COLOR)
}

void SVC_ATTRIBUTES setBlinking(uint16_t periodInMilliseconds, float dutyCycle) {
  SVC_RETURNING_VOID(SVC_LED_SET_BLINKING)
}

KDColor SVC_ATTRIBUTES updateColorWithPlugAndCharge() {
  SVC_RETURNING_R0(SVC_LED_UPDATE_COLOR_WITH_PLUG_AND_CHARGE, KDColor)
}

}
}
