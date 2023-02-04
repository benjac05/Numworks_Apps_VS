#include "display.h"
#include "eadkpp.h"
#include "palette.h"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Messages";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  EADK::Display::pushRectUniform(EADK::Screen::Rect, LightBlue);
  EADK::Display::drawString("Hello, world!", EADK::Point((EADK::Screen::Width - 9 * 10) / 2, (EADK::Screen::Height - 18) / 2), true, Black, LightBlue);
  while (true) {
    EADK::Keyboard::State keyboardState = EADK::Keyboard::scan();
  }
}
