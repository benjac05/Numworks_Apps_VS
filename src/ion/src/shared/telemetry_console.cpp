#include <ion/console.h>
#include <ion/telemetry.h>

void Ion2::Telemetry::reportScreen(const char * screenName) {
  Console::writeLine("TelemetryScreen: ", false);
  Console::writeLine(screenName);
}

void Ion2::Telemetry::reportEvent(const char * category, const char * action, const char * label) {
  Console::writeLine("TelemetryEvent: ", false);
  Console::writeLine(category, false);
  Console::writeLine(", ", false);
  Console::writeLine(action, false);
  Console::writeLine(", ", false);
  Console::writeLine(label);
}
