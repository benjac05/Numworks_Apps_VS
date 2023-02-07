#ifndef ION_SHARED_KEYBOARD_H
#define ION_SHARED_KEYBOARD_H

#include <ion/keyboard.h>

namespace Ion2 {
namespace Keyboard {

void resetMemoizedState();
void pushState(State state);
void keyboardWasScanned(State state);

}
}

#endif
