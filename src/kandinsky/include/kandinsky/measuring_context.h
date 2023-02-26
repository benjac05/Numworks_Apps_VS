#ifndef KANDINSKY_MEASURING_CONTEXT_H
#define KANDINSKY_MEASURING_CONTEXT_H

#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/kandinsky/include/kandinsky/context.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/include/ion/display.h"

class KDMeasuringContext : public KDContext {
public:
  KDMeasuringContext(KDContext &other) : KDContext(other), m_rect(KDRectZero) {}
  KDRect writtenRect() { return relativeRect(m_rect); }
  void reset() { m_rect = KDRectZero; }
private:
  void pushRect(KDRect rect, const KDColor * pixels) override { m_rect = m_rect.unionedWith(rect); };
  void pushRectUniform(KDRect rect, KDColor color) override { m_rect = m_rect.unionedWith(rect); };
  void pullRect(KDRect rect, KDColor * pixels) override {};
  KDRect m_rect;
};

#endif
