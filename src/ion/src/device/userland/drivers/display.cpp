#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/svcall.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/ion_context.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/src/device/userland/drivers/display.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/eadkpp.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/eadk_palette.h"

namespace Ion {
namespace Display {

void SVC_ATTRIBUTES pushRect(KDRect r, const KDColor * pixels) {
  SVC_RETURNING_VOID(SVC_DISPLAY_PUSH_RECT)
}

void SVC_ATTRIBUTES pushRectUniform(KDRect r, KDColor c) {
  //SVC_RETURNING_VOID(SVC_DISPLAY_PUSH_RECT_UNIFORM)
  EADK::Display::pushRectUniform(EADK::Rect(r.left(), r.top(), r.width(), r.height()), c.toRGB24());
}

void SVC_ATTRIBUTES pullRect(KDRect r, KDColor * pixels) {
  SVC_RETURNING_VOID(SVC_DISPLAY_PULL_RECT)
}

bool SVC_ATTRIBUTES waitForVBlank() {
  SVC_RETURNING_R0(SVC_DISPLAY_WAIT_FOR_V_BLANK, bool)
}

void SVC_ATTRIBUTES POSTPushMulticolor(int rootNumberTiles, int tileSize) {
  SVC_RETURNING_VOID(SVC_DISPLAY_POST_PUSH_MULTICOLOR)
}

void drawString(const char * text, KDPoint point, bool largeFont, KDColor textColor, KDColor backgroundColor) {
  EADK::Display::drawString(text, EADK::Point(point.x(), point.y()), largeFont, EADK::Color(textColor.toRGB24()), EADK::Color(backgroundColor.toRGB24()));
}

void setScreenshotCallback(void(*)(void)) {

}

}
}
