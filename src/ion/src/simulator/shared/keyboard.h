#ifndef ION_SIMULATOR_KEYBOARD_H
#define ION_SIMULATOR_KEYBOARD_H

#include <ion/keyboard.h>
#include <SDL.h>

namespace Ion2 {
namespace Simulator {
namespace Keyboard {

void keyDown(Ion2::Keyboard::Key k);
void keyUp(Ion2::Keyboard::Key k);
bool scanHandlesSDLKey(SDL_Scancode key);
void didScan();

}
}
}

#endif
