#include "eadkpp.h"
#include "kandinsky/include/kandinsky/rect.h"
#include "ion/include/ion.h"


extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Test App";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  Ion::Display::pushRect(KDRectScreen, &KDColorBlack);
  
  while (true);
}
