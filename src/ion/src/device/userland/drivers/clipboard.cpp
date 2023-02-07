#include <ion/clipboard.h>

namespace Ion2 {

/* Dummy implementation
 * On the device, the clipboard is fully handled by Escher::Clipboard. */

void Clipboard::write(const char * text) {}
const char * Clipboard::read() { return nullptr; }

}
