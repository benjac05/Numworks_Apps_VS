#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/console.h"
#include <kandinsky/ion_context.h>

namespace Ion {
namespace Console {

char readChar() {
  return 0;
}

void writeChar(char c) {
  KDIonContext::Putchar(c);
}

bool clear() {
  KDIonContext::Clear();
  return true;
}

bool transmissionDone() {
  return true;
}

}
}
