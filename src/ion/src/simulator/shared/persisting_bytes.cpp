#include <ion/persisting_bytes.h>
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
