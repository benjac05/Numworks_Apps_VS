#ifndef ION_SHARED_KEYBOARD_H
#define ION_SHARED_KEYBOARD_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/keyboard.h"

namespace Ion {
namespace Keyboard {

void resetMemoizedState();
void pushState(State state);
void keyboardWasScanned(State state);

}
}

#endif
