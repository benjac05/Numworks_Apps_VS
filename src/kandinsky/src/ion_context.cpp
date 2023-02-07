#include <kandinsky/ion_context.h>
#include <ion/display.h>

KDIonContext * KDIonContext::SharedContext() {
  static KDIonContext context;
  return &context;
}

KDIonContext::KDIonContext() :
KDContext(KDPointZero, KDRectScreen)
{
}

void KDIonContext::pushRect(KDRect rect, const KDColor * pixels) {
  Ion2::Display::pushRect(rect, pixels);
}

void KDIonContext::pushRectUniform(KDRect rect, KDColor color) {
  Ion2::Display::pushRectUniform(rect, color);
}

void KDIonContext::pullRect(KDRect rect, KDColor * pixels) {
  Ion2::Display::pullRect(rect, pixels);
}

static KDPoint s_cursor = KDPointZero;

void KDIonContext::Putchar(char c) {
  char text[2] = {c, 0};
  if (s_cursor.x() > Ion2::Display::Width - KDFont::GlyphWidth(KDFont::Size::Large)) {
    s_cursor = SharedContext()->drawString("\n", s_cursor);
  }
  if (s_cursor.y() > Ion2::Display::Height - KDFont::GlyphHeight(KDFont::Size::Large)) {
    Clear(KDPoint(s_cursor.x(), 0));
  }
  s_cursor = SharedContext()->drawString(text, s_cursor);
}

void KDIonContext::Clear(KDPoint newCursorPosition) {
  KDRect screen(0, 0, Ion2::Display::Width, Ion2::Display::Height);
  KDIonContext * ctx = SharedContext();
  ctx->setOrigin(KDPointZero);
  ctx->setClippingRect(screen);
  ctx->pushRectUniform(screen, KDColorWhite);
  s_cursor = newCursorPosition;
}
