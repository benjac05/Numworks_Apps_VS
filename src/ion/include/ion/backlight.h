#ifndef ION_BACKLIGHT_H
#define ION_BACKLIGHT_H

#include <stdint.h>

namespace Ion2 {
namespace Backlight {

constexpr uint8_t MaxBrightness = 240;
void setBrightness(uint8_t b);
uint8_t brightness();

}
}

#endif
