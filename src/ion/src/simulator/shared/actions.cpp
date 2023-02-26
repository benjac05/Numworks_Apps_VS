#include "actions.h"

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/display.h"

#include "display.h"
#include "framebuffer.h"
#include "platform.h"
#include "state_file.h"
#include "screenshot.h"
#include "window.h"

#include <signal.h>

namespace Ion {
namespace Simulator {
namespace Actions {

constexpr const char * kStateFileExtension = "nws";

void saveState() {
  const char * path = Platform::filePathForWriting(kStateFileExtension);
  if (path != nullptr) {
    StateFile::save(path);
  }
}

#if 0
void loadState() {
// We would need to be able to perform a reset for this to be callable anytime
  const char * path = Platform::filePathForReading(kStateFileExtension);
  if (path != nullptr) {
    StateFile::load(path);
    // State file's language is ignored
  }
}
#endif

void copyScreenshot() {
  Display::prepareForScreenshot();
  Screenshot s;
  s.capture();
}

void saveScreenshot() {
  Display::prepareForScreenshot();
  Screenshot s(Platform::filePathForWriting("png"));
  s.capture();
}

void saveStateForReload() {
  const char * state_filename = Platform::filePathInTempDir("numworks.reload.nws");
  StateFile::save(state_filename);
}

void handleUSR1Sig(int s) {
#if !defined(_WIN32)
  if (s == SIGUSR1) {
    saveStateForReload();
    Window::shutdown();
    SDL_Quit();
    exit(0);
  }
#endif
}

}
}
}
