#ifndef COMPARE_SCREENSHOT_H
#define COMPARE_SCREENSHOT_H

#include <ion/events.h>

namespace Ion2 {
namespace Simulator {

class Screenshot {
public:
  Screenshot(const char * path = nullptr);
  void initEachStep(const char * path) { init(path, true); }
  void init(const char * path, bool eachStep = false);
  void captureStep(Events::Event nextEvent = Events::None);
  void capture(Events::Event nextEvent = Events::None);
  static Screenshot * commandlineScreenshot();

private:
  const char * m_path;
  int m_stepNumber;
  bool m_eachStep;
};

}  // namespace Simulator
}  // namespace Ion2

#endif /* COMPARE_SCREENSHOT_H */
