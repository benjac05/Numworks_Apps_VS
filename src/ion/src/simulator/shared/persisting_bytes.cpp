#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/persisting_bytes.h"
#include <assert.h>

namespace Ion {
namespace PersistingBytes {

PersistingBytesInt s_persistedBytes = 0;

void write(PersistingBytesInt value) {
  s_persistedBytes = value;
}

PersistingBytesInt read() {
  return s_persistedBytes;
}

}
}
