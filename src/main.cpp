#include "eadkpp.h"
#include "palette.h"
#include "kandinsky/include/kandinsky/rect.h"
#include "ion/include/ion.h"


extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "More";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;


int main(int argc, char * argv[]) {
  Ion2::Display::pushRect(KDRectScreen, &KDColorBlue);
  while (true);
}
