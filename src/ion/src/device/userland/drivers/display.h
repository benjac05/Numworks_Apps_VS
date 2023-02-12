#ifndef ION_DEVICE_USERLAND_DRIVERS_DISPLAY_H
#define ION_DEVICE_USERLAND_DRIVERS_DISPLAY_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/color.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/font.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/point.h"

namespace Ion {
namespace Display {

void drawString(const char * text, KDPoint point, bool largeFont, KDColor textColor, KDColor backgroundColor);


}
}

#endif
