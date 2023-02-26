#include <quiz.h>
#include <string.h>
#include <poincare/code_point_layout.h>
#include <poincare/empty_layout.h>
#include <poincare/expression.h>
#include <poincare/integral_layout.h>
#include <poincare/matrix_layout.h>
#include <poincare/nth_root_layout.h>
#include <poincare/sum_layout.h>
#include <escher/clipboard.h>

using namespace Escher;
using namespace Poincare;

void assert_stored_text_is_parseable(Layout layout) {
  constexpr int bufferSize = 500;
  char buffer[bufferSize];
  layout.serializeForParsing(buffer, bufferSize);
  Clipboard * clipboard = Clipboard::SharedClipboard();
  clipboard->store(buffer);
  Expression e = Expression::Parse(clipboard->storedText(), nullptr, false);
  Layout result = e.createLayout(Preferences::sharedPreferences()->displayMode(), PrintFloat::k_numberOfStoredSignificantDigits, nullptr);
  quiz_assert(layout.isIdenticalTo(result));
}

QUIZ_CASE(escher_clipboard_stored_text_is_parseable) {
  Layout l = IntegralLayout::Builder(EmptyLayout::Builder(), CodePointLayout::Builder('x'), EmptyLayout::Builder(), EmptyLayout::Builder());
  assert_stored_text_is_parseable(l);
  l = NthRootLayout::Builder(EmptyLayout::Builder());
  assert_stored_text_is_parseable(l);
  l = MatrixLayout::Builder(CodePointLayout::Builder('1'), EmptyLayout::Builder(), EmptyLayout::Builder(), CodePointLayout::Builder('2'));
  assert_stored_text_is_parseable(l);
  l = SumLayout::Builder(EmptyLayout::Builder(), CodePointLayout::Builder('n'), EmptyLayout::Builder(), EmptyLayout::Builder());
  assert_stored_text_is_parseable(l);
  l = SumLayout::Builder(EmptyLayout::Builder(), CodePointLayout::Builder('n'), EmptyLayout::Builder(), EmptyLayout::Builder());
  assert_stored_text_is_parseable(l);;
}
