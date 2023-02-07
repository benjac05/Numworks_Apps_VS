#ifndef ION_SIMULATOR_WINDOW_H
#define ION_SIMULATOR_WINDOW_H

#include <SDL.h>

namespace Ion2 {
namespace Simulator {
namespace Window {

void init();
void shutdown();

bool isHeadless();

void setNeedsRefresh();
void refresh();
void relayout();

// Callbacks
void didRefresh();

void didInit(SDL_Window * window);
void willShutdown(SDL_Window * window);

}
}
}

#endif
