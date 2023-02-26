#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/reset.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"

namespace Ion {
namespace Reset {

void SVC_ATTRIBUTES core() {
  SVC_RETURNING_VOID(SVC_RESET_CORE)
}

}
}
