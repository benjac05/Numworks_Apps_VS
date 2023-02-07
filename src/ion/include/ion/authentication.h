#ifndef ION_AUTHENTICATION_H
#define ION_AUTHENTICATION_H

#include <stdint.h>

namespace Ion2 {
namespace Authentication {

enum class ClearanceLevel {
  NumWorks = 0,
  NumWorksAndThirdPartyApps,
  ThirdParty
};

ClearanceLevel clearanceLevel();

}
}

#endif

