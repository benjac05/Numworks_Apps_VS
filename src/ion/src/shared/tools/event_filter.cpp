#include <iostream>
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/events.h"

// TODO : Update this script to handle state file headers

int main(int argc, char * argv[]) {
  unsigned char c = 0;
  while (std::cin >> c) {
    Ion::Events::Event e(c);
    if (Ion::Events::isDefined(c) && e != Ion::Events::Termination) {
      std::cout << c;
    }
  }
}
