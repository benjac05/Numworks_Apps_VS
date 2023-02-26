#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/authentication.h"

namespace Ion {
namespace Authentication {

ClearanceLevel SVC_ATTRIBUTES clearanceLevel() {
  SVC_RETURNING_R0(SVC_AUTHENTICATION_CLEARANCE_LEVEL, ClearanceLevel)
}

}
}
