#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include <ion.h>

namespace Ion {

const char * SVC_ATTRIBUTES fccId() {
  SVC_RETURNING_R0(SVC_FCC_ID, const char *)
}

}
