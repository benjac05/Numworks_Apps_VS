#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/unicode/code_point.h"
#include "/Users/Ben/Documents/Numworks_Apps/Numworks_Apps_VS/src/ion/unicode/utf8_decoder.h"
#include <assert.h>

char CodePoint::getChar() const {
  assert(UTF8Decoder::CharSizeOfCodePoint(*this) == 1);
  return static_cast<char>(m_code);
}

bool CodePoint::isEquationOperator() const {
  return m_code == '<'
      || m_code == '='
      || m_code == '>'
      || m_code == UCodePointInferiorEqual
      || m_code == UCodePointSuperiorEqual;
}
