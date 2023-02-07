#include <drivers/svcall.h>
#include <ion/authentication.h>

namespace Ion2 {
namespace Authentication {

ClearanceLevel SVC_ATTRIBUTES clearanceLevel() {
  SVC_RETURNING_R0(SVC_AUTHENTICATION_CLEARANCE_LEVEL, ClearanceLevel)
}

}
}
