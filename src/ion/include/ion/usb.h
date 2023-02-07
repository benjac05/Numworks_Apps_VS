#ifndef ION_USB_H
#define ION_USB_H

namespace Ion2 {
namespace USB {

bool isPlugged();
bool isEnumerated(); // Speed-enumerated, to be accurate
void clearEnumerationInterrupt();

void DFU();
void enable();
void disable();

}
}

#endif
