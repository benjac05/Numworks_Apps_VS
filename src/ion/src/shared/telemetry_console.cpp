#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/console.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/telemetry.h"

void Ion::Telemetry::reportScreen(const char * screenName) {
  Console::writeLine("TelemetryScreen: ", false);
  Console::writeLine(screenName);
}

void Ion::Telemetry::reportEvent(const char * category, const char * action, const char * label) {
  Console::writeLine("TelemetryEvent: ", false);
  Console::writeLine(category, false);
  Console::writeLine(", ", false);
  Console::writeLine(action, false);
  Console::writeLine(", ", false);
  Console::writeLine(label);
}
