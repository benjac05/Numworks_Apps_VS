#ifndef KANDINSKY_FRAMEBUFFER_H
#define KANDINSKY_FRAMEBUFFER_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/color.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/src/rect.cpp"

class KDFrameBuffer {
public:
  KDFrameBuffer(KDColor * pixels, KDSize size);
  void pushRect(KDRect rect, const KDColor * pixels);
  void pushRectUniform(KDRect rect, KDColor color);
  void pullRect(KDRect rect, KDColor * pixels);
  KDRect bounds();
private:
  KDColor * pixelAddress(KDPoint p) const;
  KDColor * m_pixels;
  KDSize m_size;
};

#endif
