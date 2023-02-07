#ifndef ION_SIMULATOR_FRAMEBUFFER_H
#define ION_SIMULATOR_FRAMEBUFFER_H

#include <kandinsky/color.h>

namespace Ion2 {
namespace Simulator {
namespace Framebuffer {

const KDColor * address();
void setActive(bool enabled);

}
}
}

#endif
