#include <drivers/reset.h>
#include <drivers/svcall.h>

namespace Ion2 {
namespace Reset {

void SVC_ATTRIBUTES core() {
  SVC_RETURNING_VOID(SVC_RESET_CORE)
}

}
}
