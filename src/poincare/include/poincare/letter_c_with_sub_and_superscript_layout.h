#ifndef LETTER_C_WITH_SUB_AND_SUPERSCRIPT_LAYOUT_NODE_H
#define LETTER_C_WITH_SUB_AND_SUPERSCRIPT_LAYOUT_NODE_H

#include <poincare/letter_with_sub_and_superscript_layout.h>
#include <poincare/layout_helper.h>

namespace Poincare {

class LetterCWithSubAndSuperscriptLayoutNode final : public LetterWithSubAndSuperscriptLayoutNode {
public:
  // Layout
  Type type() const override { return Type::LetterCWithSubAndSuperscriptLayout; }

  // SerializableNode
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;

#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "LetterCWithSubAndSuperscriptLayout";
  }
#endif

private:
  void renderLetter(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) override;
};

class LetterCWithSubAndSuperscriptLayout final : public LayoutTwoChildren<LetterCWithSubAndSuperscriptLayout, LetterCWithSubAndSuperscriptLayoutNode> {
public:
  LetterCWithSubAndSuperscriptLayout() = delete;
};

}

#endif
