#ifndef ION_SIMULATOR_JOURNAL_QUEUE_JOURNAL_H
#define ION_SIMULATOR_JOURNAL_QUEUE_JOURNAL_H

#include <ion/events.h>
#include <queue>

namespace Ion2 {
namespace Simulator {
namespace Journal {

class QueueJournal : public Ion2::Events::Journal {
public:
  void pushEvent(Ion2::Events::Event e) override {
    if (e != Ion2::Events::None) {
      m_eventStorage.push(e);
    }
  }
  Ion2::Events::Event popEvent() override {
    if (isEmpty()) {
      return Ion2::Events::None;
    }
    Ion2::Events::Event e = m_eventStorage.front();
    m_eventStorage.pop();
    return e;
  }
  bool isEmpty() override {
    return m_eventStorage.empty();
  }
private:
  std::queue<Ion2::Events::Event> m_eventStorage;
};

}
}
}

#endif
