#ifndef ION_SIMULATOR_EVENTS_H
#define ION_SIMULATOR_EVENTS_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/events.h"
#include <SDL.h>

namespace Ion {
namespace Events {

constexpr static size_t sharedExternalTextBufferSize = sizeof(SDL_TextInputEvent::text);
char * sharedExternalTextBuffer();

}
}

#endif
