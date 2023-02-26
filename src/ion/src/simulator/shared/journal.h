#ifndef ION_SIMULATOR_JOURNAL_H
#define ION_SIMULATOR_JOURNAL_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/events.h"

namespace Ion {
namespace Simulator {
namespace Journal {

void init();

Ion::Events::Journal * replayJournal();
Ion::Events::Journal * logJournal();

}
}
}

#endif
