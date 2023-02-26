#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/clipboard.h"

namespace Ion {

/* Dummy implementation
 * On the device, the clipboard is fully handled by Escher::Clipboard. */

void Clipboard::write(const char * text) {}
const char * Clipboard::read() { return nullptr; }

}
