#ifndef POINCARE_GRID_LAYOUT_NODE_H
#define POINCARE_GRID_LAYOUT_NODE_H

#include <poincare/array.h>
#include <poincare/empty_layout.h>
#include <poincare/layout.h>
#include <poincare/layout_cursor.h>

namespace Poincare {

class GridLayout;
class MatrixLayoutNode;

class GridLayoutNode : public Array, public LayoutNode {
  friend class BinomialCoefficientLayoutNode;
  friend class GridLayout;
public:
  constexpr static KDCoordinate k_gridEntryMargin = 6;

  static bool IsGridLayoutType(Type type) { return type == Type::MatrixLayout || type == Type::PiecewiseOperatorLayout; }

  GridLayoutNode() :
    Array(),
    LayoutNode()
  {}

  // Grid layout
  KDSize gridSize(KDFont::Size font) const { return KDSize(width(font), height(font)); }
  void willAddSiblingToEmptyChildAtIndex(int childIndex);
  virtual void startEditing() = 0;
  virtual void stopEditing() = 0;

  // LayoutNode
  void moveCursorLeft(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool forSelection) override;
  void moveCursorRight(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool forSelection) override;
  void moveCursorUp(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool equivalentPositionVisited = false, bool forSelection = false) override;
  void moveCursorDown(LayoutCursor * cursor, bool * shouldRecomputeLayout, bool equivalentPositionVisited = false, bool forSelection = false) override;
  void moveCursorVertically(VerticalDirection direction, LayoutCursor * cursor, bool * shouldRecomputeLayout, bool equivalentPositionVisited, bool forSelection) override;
  void deleteBeforeCursor(LayoutCursor * cursor) override;

  // SerializableNode
  int serialize(char * buffer, int bufferSize, Preferences::PrintFloatMode floatDisplayMode, int numberOfSignificantDigits) const override {
    assert(false);
    return 0;
  }

  // TreeNode
  size_t size() const override { return sizeof(GridLayoutNode); }
  void didChangeArity(int newNumberOfChildren) override { return Array::didChangeNumberOfChildren(newNumberOfChildren); }
  int numberOfChildren() const override { return m_numberOfRows * m_numberOfColumns; }
  void setNumberOfChildren(int numberOfChildren) override {
    if (numberOfChildren == 0) { // Used to erase children
      m_numberOfRows = 0;
      m_numberOfColumns = 0;
    }
  }
#if POINCARE_TREE_LOG
  void logNodeName(std::ostream & stream) const override {
    stream << "GridLayout";
  }
#endif

protected:
  // Row and columns
  virtual bool numberOfRowsIsFixed() const { return false; }
  virtual bool numberOfColumnsIsFixed() const { return false; }
  virtual bool isEditing() const = 0;
  int indexOfLastNonGrayChildWhenIsEditing() const;
  bool onlyFirstChildIsNonEmpty() const;
  bool isColumnEmpty(int index) const { return isColumnOrRowEmpty(true, index); }
  bool isRowEmpty(int index) const { return isColumnOrRowEmpty(false, index); }
  void addEmptyColumn(EmptyLayoutNode::Color color) {
    assert(!numberOfColumnsIsFixed());
    return addEmptyRowOrColumn(true, color);
  }
  void addEmptyRow(EmptyLayoutNode::Color color) {
    assert(!numberOfRowsIsFixed());
    return addEmptyRowOrColumn(false, color);
  }
  void deleteColumnAtIndex(int index);
  void deleteRowAtIndex(int index);
  bool childIsRightOfGrid(int index) const;
  bool childIsLeftOfGrid(int index) const;
  bool childIsTopOfGrid(int index) const;
  bool childIsBottomOfGrid(int index) const;
  int rowAtChildIndex(int index) const;
  int columnAtChildIndex(int index) const;
  int indexAtRowColumn(int rowIndex, int columnIndex) const;

  // Sizes
  virtual KDCoordinate horizontalGridEntryMargin(KDFont::Size font) const { return k_gridEntryMargin; }
  KDCoordinate verticalGridEntryMargin(KDFont::Size font) const { return k_gridEntryMargin; }
  KDCoordinate rowBaseline(int i, KDFont::Size font);
  KDCoordinate rowHeight(int i, KDFont::Size font) const;
  KDCoordinate height(KDFont::Size font) const;
  KDCoordinate columnWidth(int j, KDFont::Size font) const;
  KDCoordinate width(KDFont::Size font) const;

  // LayoutNode
  KDSize computeSize(KDFont::Size font) override;
  KDCoordinate computeBaseline(KDFont::Size font) override;
  KDPoint positionOfChild(LayoutNode * l, KDFont::Size font) override;

private:
  // GridLayoutNode

  bool isColumnOrRowEmpty(bool column, int index) const;
  void addEmptyRowOrColumn(bool column, EmptyLayoutNode::Color color);
  void colorGrayEmptyLayoutsInYellowInColumnOrRow(bool column, int lineIndex);
  void didReplaceChildAtIndex(int index, LayoutCursor * cursor, bool force) override;
};

class GridLayout : public Layout {
public:
  GridLayout(const GridLayoutNode * n) : Layout(n) {}

  void setDimensions(int rows, int columns);
  using Layout::addChildAtIndex;
  int numberOfRows() const { return node()->numberOfRows(); }
  int numberOfColumns() const { return node()->numberOfColumns(); }
  bool isEditing() const { return node()->isEditing(); }
  void startEditing() { node()->startEditing(); }
  void stopEditing() { node()->stopEditing(); }
private:
  virtual GridLayoutNode * node() const { return static_cast<GridLayoutNode *>(Layout::node()); }
  void setNumberOfRows(int rows) { node()->setNumberOfRows(rows); }
  void setNumberOfColumns(int columns) { node()->setNumberOfColumns(columns); }
};

}

#endif
