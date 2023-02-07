#include <iostream>
#include <ion/events.h>

// TODO : Update this script to handle state file headers

int main(int argc, char * argv[]) {
  unsigned char c = 0;
  while (std::cin >> c) {
    Ion2::Events::Event e(c);
    if (Ion2::Events::isDefined(c) && e != Ion2::Events::Termination) {
      std::cout << c;
    }
  }
}
