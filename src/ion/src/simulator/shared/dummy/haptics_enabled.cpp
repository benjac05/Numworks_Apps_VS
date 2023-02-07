#include "../haptics.h"

namespace Ion2 {
namespace Simulator {
namespace Haptics {

bool isEnabled() {
  /* Dummy default to false as failsafe.
   * Avoid to get haptics feedback that are not desactivable. */
  return false;
}

}
}
}
