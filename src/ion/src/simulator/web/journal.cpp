#include "../shared/journal.h"
#include "../shared/journal/queue_journal.h"
#include <emscripten.h>

namespace Ion2 {
namespace Simulator {
namespace Journal {

using Ion2::Events::Event;
using Ion2::Events::None;

class LogJournal : public Ion2::Events::Journal {
public:
  void pushEvent(Event e) override {
    static bool lastEventWasNone = false;
    if (e != None) {
      EM_ASM({
        if (typeof Module.onIonEvent === "function") {
          Module.onIonEvent($0);
        }
      }, static_cast<uint8_t>(e));
      lastEventWasNone = false;
    } else {
      if (!lastEventWasNone) {
        EM_ASM({
          if (typeof Module.onEpsilonIdle === "function") {
            Module.onEpsilonIdle();
          }
        });
        lastEventWasNone = true;
      }
    }
  }
  Event popEvent() override {
    return None;
  }
  bool isEmpty() override {
    return true;
  }
};

void init() {
  Events::logTo(logJournal());
}

Events::Journal * replayJournal() {
  static QueueJournal journal;
  return &journal;
}

Ion2::Events::Journal * logJournal() {
  static LogJournal journal;
  return &journal;
}

}
}
}
