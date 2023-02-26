#include "kandinsky/include/kandinsky/rect.h"
#include "kandinsky/include/kandinsky/color.h"
#include "ion/include/ion.h"
#include "eadkpp.h"


extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Test App";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  Ion::Display::pushRectUniform(KDRectScreen, KDColorWhite);
  for (int i = 0; i < 100; i++) {
    Ion::Display::drawString("Hello!", KDPoint(i*5,i*20), false, KDColorBlack, KDColor::RGB24(0x69fc4c)); //lime green background
  }

  while (true) {
    Ion::Keyboard::State myState = Ion::Keyboard::scan();
    if (myState.keyDown(Ion::Keyboard::Key::Left)) {
      break;
    }
  }
}
