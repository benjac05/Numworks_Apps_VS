#ifndef ION_LED_H
#define ION_LED_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/color.h"

namespace Ion2 {
namespace LED {

KDColor getColor();
void setColor(KDColor c);
void setBlinking(uint16_t periodInMilliseconds, float dutyCycle);

KDColor updateColorWithPlugAndCharge();

}
}

#endif
