#ifndef POINCARE_VERTICAL_OFFSET_LAYOUT_NODE_H
#define POINCARE_VERTICAL_OFFSET_LAYOUT_NODE_H

#include <poincare/layout_cursor.h>
#include <poincare/layout.h>

namespace Poincare {

class VerticalOffsetLayoutNode final : public LayoutNode {
  friend class HigherOrderDerivativeLayoutNode;
public:
  enum class VerticalPosition : uint8_t {
    Subscript = 0,
    Superscript
  };
  enum class HorizontalPosition : uint8_t {
    Prefix = 0,
    Suffix
  };

  VerticalOffsetLayoutNode(VerticalPosition verticalPosition = VerticalPosition::Superscript, HorizontalPosition horizontalPosition = HorizontalPosition::Suffix) :
    LayoutNode(),
    m_verticalPosition(verticalPosition),
    m_horizontalPosition(horizontalPosition)
  {}

  // Layout
  Type type() const override { return Type::VerticalOffsetLayout; }

  // VerticalOffsetLayoutNode
  VerticalPosition verticalPosition() const { return m_verticalPosition; }
  HorizontalPosition horizontalPosition() const { return m_horizontalPosition; }

  // LayoutNode
  void moveCursorLeft(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool forSelection) override;
  void moveCursorRight(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool forSelection) override;
  void moveCursorUp(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool equivalentPositionVisited = false, bool forSelection = false) override;
  void moveCursorDown(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool equivalentPositionVisited = false, bool forSelection = false) override;
  void deleteBeforeCursor(LayoutCursor * cursor) override;
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override;
  bool canBeOmittedMultiplicationRightFactor() const override { return false; }

  // TreeNode
  size_t size() const override { return sizeof(VerticalOffsetLayoutNode); }
  int numberOfChildren() const override { return 1; }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << (verticalPosition() == VerticalPosition::Subscript ? "Subscript" : "Superscript");
  }
  void logAttributes(std::ostream & stream) const override {
    if (m_horizontalPosition == HorizontalPosition::Prefix) {
      stream << " prefix=\"1\"";
    }
  }
#endif

private:
  // LayoutNode
  constexpr static KDCoordinate k_indiceHeight = 10;
  constexpr static KDCoordinate k_separationMargin = 5;

  KDSize computeSize(KDFont::Size font) override;
  KDCoordinate computeBaseline(KDFont::Size font) override;
  KDPoint positionOfChild(LayoutNode * child, KDFont::Size font) override;
  bool willAddSibling(LayoutCursor * cursor, Layout * sibling, bool moveCursor) override;
  void render(KDContext * ctx, KDPoint p, KDFont::Size font, KDColor expressionColor, KDColor backgroundColor, Layout * selectionStart = nullptr, Layout * selectionEnd = nullptr, KDColor selectionColor = KDColorRed) override {}
  bool protectedIsIdenticalTo(Layout l) override;

  LayoutNode * indiceLayout() { return childAtIndex(0); }
  int baseOffsetInParent() const { return m_horizontalPosition == HorizontalPosition::Prefix ? +1 : -1; }
  LayoutNode * baseLayout();

  /* Since both enums can be represented on one bit each, use the bitfield
   * syntax to let the compiler cram them into one byte instead of two. */
  VerticalPosition m_verticalPosition : 1;
  HorizontalPosition m_horizontalPosition : 1;
};

class VerticalOffsetLayout final : public Layout {
public:
  static VerticalOffsetLayout Builder(Layout l, VerticalOffsetLayoutNode::VerticalPosition verticalPosition, VerticalOffsetLayoutNode::HorizontalPosition horizontalPosition = VerticalOffsetLayoutNode::HorizontalPosition::Suffix);
  VerticalOffsetLayout() = delete;
  VerticalOffsetLayoutNode::VerticalPosition verticalPosition() const { return static_cast<VerticalOffsetLayoutNode *>(node())->verticalPosition(); }
};

}

#endif
