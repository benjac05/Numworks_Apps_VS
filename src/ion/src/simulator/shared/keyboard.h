#ifndef ION_SIMULATOR_KEYBOARD_H
#define ION_SIMULATOR_KEYBOARD_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/keyboard.h"
#include <SDL.h>

namespace Ion {
namespace Simulator {
namespace Keyboard {

void keyDown(Ion::Keyboard::Key k);
void keyUp(Ion::Keyboard::Key k);
bool scanHandlesSDLKey(SDL_Scancode key);
void didScan();

}
}
}

#endif
