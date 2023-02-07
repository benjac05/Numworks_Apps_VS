#include <ion/battery.h>
#include <ion/display.h>
#include <ion/post_and_hardware_tests.h>
#include <ion/timing.h>
#include <kandinsky/color.h>

namespace Ion2 {
namespace POSTAndHardwareTests {

constexpr static int k_stampSize = 8;

bool BatteryOK() {
  return Ion2::Battery::level() == Ion2::Battery::Charge::FULL;
}

bool VBlankOK() {
  bool result = true;
  for (int i = 0; i < 3; i++) {
    result = result && Ion2::Display::waitForVBlank();
  }
  return result;
}

int LCDDataGlyphFailures() {
  /* Draw a tiled pattern:
   * On first pass, red / blue / red / blue...
   * On second pass, blue / green / blue / green...
   * On third pass, green / red / green / red...
   * And so on. */

  constexpr int stampSize = 10;
  constexpr int numberOfStamps = 3;
  static_assert(Ion2::Display::Width % stampSize == 0, "Stamps must tesselate the display");
  static_assert(Ion2::Display::Height % stampSize == 0, "Stamps must tesselate the display");

  KDColor stamps[numberOfStamps][stampSize*stampSize];
  constexpr KDColor colorForStamp[numberOfStamps] = {KDColorRed, KDColorBlue, KDColorGreen};
  for (int i=0; i<numberOfStamps; i++) {
    KDColor c = colorForStamp[i];
    KDColor * stamp = stamps[i];
    for (int j=0; j<stampSize*stampSize; j++) {
      stamp[j] = c;
    }
  }

  int numberOfHorizontalTiles = Ion2::Display::Width / stampSize;
  int numberOfVerticalTiles = Ion2::Display::Height / stampSize;

  constexpr int numberOfPasses = 150;
  int numberOfInvalidPixels = 0;

  constexpr int stampHeightPull = 1;
  constexpr int stampWidthPull = 1;
  KDColor resultStamp[stampHeightPull*stampHeightPull];

  for (int p=0; p<numberOfPasses; p++) {
    // Push a checker pattern on the screen
    int firstColorIndex = p%numberOfStamps;
    int secondColorIndex = (p+1)%numberOfStamps;
    KDColor * firstStamp = stamps[firstColorIndex];
    KDColor * secondStamp = stamps[secondColorIndex];
    // Draw the pattern
    for (int j=0; j<numberOfVerticalTiles; j++) {
      for (int i=0; i<numberOfHorizontalTiles; i++) {
        KDRect tile(i*stampSize, j*stampSize, stampSize, stampSize);
        Ion2::Display::pushRect(tile, (i+j)%2 == 0 ? firstStamp : secondStamp);
      }
    }
    // Check the pattern
    for (int j=0; j<numberOfVerticalTiles; j++) {
      for (int i=0; i<numberOfHorizontalTiles; i++) {
        KDRect tile((i+1)*stampSize-stampWidthPull, (j+1)*stampSize-stampHeightPull, stampWidthPull, stampHeightPull);
        Ion2::Display::pullRect(tile, resultStamp);
        KDColor c = colorForStamp[(i+j)%2 == 0 ? firstColorIndex : secondColorIndex];
        for (int k = 0; k < stampWidthPull * stampHeightPull; k++) {
          if (resultStamp[k] != c) {
            numberOfInvalidPixels++;
          }
        }
      }
    }
  }
  return numberOfInvalidPixels;
}

int LCDTimingGlyphFailures() {
  int numberOfFailures = 0;
  const int rootNumberTiles = 3; //TODO 1 ?
  for (int i = 0; i < 100; i++) {
    Ion2::Display::POSTPushMulticolor(rootNumberTiles, k_stampSize);
    KDColor stamp[k_stampSize*k_stampSize];
    for (int i = 0; i < rootNumberTiles; i++) {
      for (int j = 0; j < rootNumberTiles; j++) {
        Ion2::Display::pullRect(KDRect(i * k_stampSize, j * k_stampSize, k_stampSize, k_stampSize), stamp);
        int shift = (i+j) % 16;
        uint16_t color = (uint16_t)(1 << shift);
        for (int k = 0; k < k_stampSize*k_stampSize; k++) {
          if (stamp[k] != color) {
            numberOfFailures++;
            break;
          }
          color ^= 0xFFFF;
        }
      }
    }
    Ion2::Timing::msleep(10);
  }
  return numberOfFailures;
}

int ColorsLCDPixelFailures() {
  int numberOfInvalidPixels = 0;
  constexpr KDColor k_colors[] = {KDColorBlack, KDColorRed, KDColorBlue, KDColorGreen, KDColorWhite};
  constexpr int stampHeight = 10;
  constexpr int stampWidth = 10;
  static_assert(Ion2::Display::Width % stampWidth == 0, "Stamps must tesselate the display");
  static_assert(Ion2::Display::Height % stampHeight == 0, "Stamps must tesselate the display");
  constexpr int stampHeightPull = 80;
  constexpr int stampWidthPull = 80;
  for (KDColor c : k_colors) {
    {
      KDColor stamp[stampWidth*stampHeight];
      for (int i=0;i<stampWidth*stampHeight; i++) {
        stamp[i] = c;
      }

      for (int i=0; i<Ion2::Display::Width/stampWidth; i++) {
        for (int j=0; j<Ion2::Display::Height/stampHeight; j++) {
          Ion2::Display::pushRect(KDRect(i*stampWidth, j*stampHeight, stampWidth, stampHeight), stamp);
        }
      }
    }

    int numberOfInvalidPixels = 0;
    KDColor stamp[stampHeightPull*stampHeightPull];

    for (int i=0; i<Ion2::Display::Width/stampWidthPull; i++) {
      for (int j=0; j<Ion2::Display::Height/stampHeightPull; j++) {
        for (int k=0; k<stampWidthPull*stampHeightPull; k++) {
          stamp[k] = KDColorBlack;
        }
        Ion2::Display::pullRect(KDRect(i*stampWidthPull, j*stampHeightPull, stampWidthPull, stampHeightPull), stamp);
        for (int k=0; k<stampWidthPull*stampHeightPull; k++) {
          if (stamp[k] != c) {
            numberOfInvalidPixels++;
            break;
          }
        }
      }
    }
  }
  return numberOfInvalidPixels;
}

constexpr static int k_smallAWidth = 7;
constexpr static int k_smallAHeight = 14;

constexpr static KDColor k_smallABuffer[k_smallAHeight][k_smallAWidth] = {
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(42292), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(42292), KDColor::RGB16(59196), KDColor::RGB16(61342), KDColor::RGB16(61342) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(52889), KDColor::RGB16(42292), KDColor::RGB16(42324), KDColor::RGB16(55035) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) },
  { KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455), KDColor::RGB16(63455) }
};

bool pushOrPullAs(bool push) {
  KDCoordinate currentWidth = 0;
  KDCoordinate currentHeight = 0;
  KDSize size(k_smallAWidth, k_smallAHeight);
  while (currentHeight + k_smallAHeight < Ion2::Display::Height ) {
    while (currentWidth + k_smallAWidth < Ion2::Display::Width ) {
      KDPoint position(currentWidth, currentHeight);
      if (push) {
        // Push the character on the screen
        Ion2::Display::pushRect(KDRect(position, size), reinterpret_cast<const KDColor *>(k_smallABuffer));
      } else {
        // Pull and compare the character from the screen
        KDColor workingBuffer[k_smallAHeight][k_smallAWidth];
        for (int i = 0; i < k_smallAHeight; i++) {
          for (int j = 0; j < k_smallAWidth; j++) {
            workingBuffer[i][j] = KDColorRed;
          }
        }
        /* Caution: Unlike fillRectWithPixels, pullRect accesses outside (0, 0,
         * Ion2::Display::Width, Ion2::Display::Height) might give weird data. */
        Ion2::Display::pullRect(KDRect(position, size), reinterpret_cast<KDColor *>(workingBuffer));
        for (int i = 0; i < k_smallAHeight; i++) {
          for (int j = 0; j < k_smallAWidth; j++) {
            if (k_smallABuffer[i][j] != workingBuffer[i][j]) {
              return false;
            }
          }
        }
      }
      currentWidth += k_smallAWidth;
    }
    currentHeight += k_smallAHeight;
  }
  return true;
}

bool TextLCDGlyphFailures() {
  // Fill the screen with 'a'
  pushOrPullAs(true);
 // Check the drawing
  return pushOrPullAs(false);
}

}
}
