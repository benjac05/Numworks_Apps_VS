#include "display.h"
#include "eadkpp.h"

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Voord";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  EADK::Display::pushRectUniform(EADK::Screen::Rect, LightBlue);
  EADK::Display::drawString("Hello, world");

  

  }
