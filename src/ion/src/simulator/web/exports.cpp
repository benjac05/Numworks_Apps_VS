#include "exports.h"
#include "../shared/display.h"
#include "../shared/journal.h"
#include "../shared/keyboard.h"
#include "../shared/window.h"
#include <ion.h>

const char * IonSoftwareVersion() {
  return Ion2::epsilonVersion();
}

const char * IonPatchLevel() {
  return Ion2::patchLevel();
}

void IonSimulatorKeyboardKeyDown(int keyNumber) {
  Ion2::Keyboard::Key key = static_cast<Ion2::Keyboard::Key>(keyNumber);
  Ion2::Simulator::Keyboard::keyDown(key);
}

void IonSimulatorKeyboardKeyUp(int keyNumber) {
  Ion2::Keyboard::Key key = static_cast<Ion2::Keyboard::Key>(keyNumber);
  Ion2::Simulator::Keyboard::keyUp(key);
}

void IonSimulatorEventsPushEvent(int eventNumber) {
  Ion2::Events::Journal * j = Ion2::Simulator::Journal::replayJournal();
  if (j != nullptr) {
    Ion2::Events::Event event(eventNumber);
    j->pushEvent(event);
    Ion2::Events::replayFrom(j);
  }
}
