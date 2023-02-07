#ifndef ION_SIMULATOR_JOURNAL_H
#define ION_SIMULATOR_JOURNAL_H

#include <ion/events.h>

namespace Ion2 {
namespace Simulator {
namespace Journal {

void init();

Ion2::Events::Journal * replayJournal();
Ion2::Events::Journal * logJournal();

}
}
}

#endif
