#ifndef ION_SHARED_EVENTS_MODIFIER_H
#define ION_SHARED_EVENTS_MODIFIER_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/events.h"

namespace Ion {
namespace Events {

void removeShift();
void removeAlpha();
bool isShiftActive();
bool isAlphaActive();
bool isLockActive();
bool wasShiftReleased(Keyboard::State state);
bool wasAlphaReleased(Keyboard::State state);
void updateModifiersFromEvent(Event e, Keyboard::State state);
int longPressFactor();
void resetLongPress();
void resetRepetition();
void incrementLongPress();
void incrementRepetition();

}
}

#endif
