#include <ion/battery.h>

bool Ion2::Battery::isCharging() {
  return false;
}

Ion2::Battery::Charge Ion2::Battery::level() {
  return Charge::FULL;
}

float Ion2::Battery::voltage() {
  return 0.0f;
}
