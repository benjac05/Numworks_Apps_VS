#include <poincare/layout.h>
#include <poincare/autocompleted_bracket_pair_layout.h>
#include <poincare/bracket_pair_layout.h>
#include <poincare/code_point_layout.h>
#include <poincare/combined_code_points_layout.h>
#include <poincare/empty_layout.h>
#include <poincare/expression.h>
#include <poincare/horizontal_layout.h>
#include <poincare/layout_cursor.h>
#include <poincare/symbol_abstract.h>

namespace Poincare {

Layout Layout::clone() const {
  if (isUninitialized()) {
    return Layout();
  }
  TreeHandle c = TreeHandle::clone();
  Layout cast = Layout(static_cast<LayoutNode *>(c.node()));
  cast.invalidAllSizesPositionsAndBaselines();
  return cast;
}

Layout Layout::LayoutFromAddress(const void * address, size_t size) {
  if (address == nullptr || size == 0) {
    return Layout();
  }
  return Layout(static_cast<LayoutNode *>(TreePool::sharedPool()->copyTreeFromAddress(address, size)));
}

int Layout::serializeParsedExpression(char * buffer, int bufferSize, Context * context) const {
  /* This method fixes the following problem:
   * Some layouts have a special serialization so they can be parsed afterwards,
   * such has logBase3(2) that serializes as log_{3}(2). When handling the
   * layout text, we want to use log(2,3) because we might paste the text in a
   * LinearEdition textfield, so what we really want is the parsed expression's
   * serialization. */
  if (bufferSize <= 0) {
    return 0;
  }
  serializeForParsing(buffer, bufferSize);
  Poincare::Expression e = Poincare::Expression::Parse(buffer, context);
  if (e.isUninitialized()) {
    buffer[0] = 0;
    return 0;
  }
  return e.serialize(buffer, bufferSize, Poincare::Preferences::sharedPreferences()->displayMode());
}

Layout Layout::recursivelyMatches(LayoutTest test) const {
  TrinaryBoolean testResult = test(*this);
  if (testResult == TrinaryBoolean::True) {
    return *this;
  }
  if (testResult == TrinaryBoolean::False) {
    return Layout();
  }
  int childrenNumber = numberOfChildren();
  for (int i = 0; i < childrenNumber; i++) {
    Layout childResult = childAtIndex(i).recursivelyMatches(test);
    if (!childResult.isUninitialized()) {
      return childResult;
    }
  }
  return Layout();
}

Layout Layout::XNTLayout() const {
  Layout xntLayout = const_cast<Layout *>(this)->node()->XNTLayout();
  assert(xntLayout.isUninitialized() || xntLayout.numberOfDescendants(true) >= 0);
  if (!xntLayout.isUninitialized() && static_cast<size_t>(xntLayout.numberOfDescendants(true)) <= SymbolAbstract::k_maxNameSize && xntLayout.recursivelyMatches(
      [](const Layout l) {
        if (l.type() != LayoutNode::Type::CodePointLayout) {
          return l.type() == LayoutNode::Type::HorizontalLayout ? TrinaryBoolean::Unknown :  TrinaryBoolean::True;
        }
        CodePoint c = static_cast<const CodePointLayout&>(l).codePoint();
        return (c.isDecimalDigit() || c.isLatinLetter() || c == '_') ? TrinaryBoolean::Unknown : TrinaryBoolean::True;
      } ).isUninitialized()) {
    /* Return xnt if :
     * - it is initialized and only contains horizontal layouts and code points
     * - all code points are letters, numbers or _
     * - There are less than k_maxNameSize descendants */
    return xntLayout;
  }
  return Layout();
}

bool Layout::displayEmptyLayouts() const {
  return AutocompletedBracketPairLayoutNode::IsAutoCompletedBracketPairType(type());
}

// Cursor
LayoutCursor Layout::cursor() const {
  assert(!isUninitialized());
  return LayoutCursor(const_cast<Layout *>(this)->node());
}

LayoutCursor Layout::equivalentCursor(LayoutCursor * cursor) {
  assert(!isUninitialized());
  return node()->equivalentCursor(cursor);
}

Layout Layout::childAtIndex(int i) const {
  assert(i >= 0 && i < numberOfChildren());
  TreeHandle c = TreeHandle::childAtIndex(i);
  return static_cast<Layout &>(c);
}

// Tree modification

void Layout::replaceChild(Layout oldChild, Layout newChild, LayoutCursor * cursor, bool force) {
  int childIndex = indexOfChild(oldChild);
  assert(childIndex >= 0);
  if (!node()->willReplaceChild(oldChild.node(), newChild.node(), cursor, force)) {
    return;
  }
  replaceChildInPlace(oldChild, newChild);
  if (cursor != nullptr && cursor->layout() != newChild) {
    // Do not alter cursor if it was already sticked to newChild
    cursor->setLayout(newChild);
    cursor->setPosition(LayoutCursor::Position::Right);
  }
  node()->didReplaceChildAtIndex(childIndex, cursor, force);
}

void Layout::replaceChildWithEmpty(Layout oldChild, LayoutCursor * cursor) {
  EmptyLayoutNode::Visibility visibility = displayEmptyLayouts() ? EmptyLayoutNode::Visibility::Never : EmptyLayoutNode::Visibility::On;
  replaceChild(oldChild, EmptyLayout::Builder(EmptyLayoutNode::Color::Yellow, visibility), cursor);
}

void Layout::replaceWith(Layout newChild, LayoutCursor * cursor) {
  Layout p = parent();
  assert(!p.isUninitialized());
  p.replaceChild(*this, newChild, cursor);
}

void Layout::replaceWithJuxtapositionOf(Layout leftChild, Layout rightChild, LayoutCursor * cursor, bool putCursorInTheMiddle) {
  Layout p = parent();
  assert(!p.isUninitialized());
  if (p.type() != LayoutNode::Type::HorizontalLayout) {
    /* One of the children to juxtapose might be "this", so we cannot just call
     * replaceWith. */
    HorizontalLayout horizontalLayoutR = HorizontalLayout::Builder();
    p.replaceChild(*this, horizontalLayoutR, cursor);
    horizontalLayoutR.addOrMergeChildAtIndex(leftChild, 0, false);
    if (putCursorInTheMiddle) {
      if (!horizontalLayoutR.isEmpty()) {
        cursor->setLayout(horizontalLayoutR.childAtIndex(horizontalLayoutR.numberOfChildren()-1));
        cursor->setPosition(LayoutCursor::Position::Right);
      } else {
        cursor->setLayout(horizontalLayoutR);
        cursor->setPosition(LayoutCursor::Position::Left);
      }
    }
    horizontalLayoutR.addOrMergeChildAtIndex(rightChild, horizontalLayoutR.numberOfChildren(), true);
    return;
  }
  /* The parent is an Horizontal layout, so directly add the two juxtaposition
   * children to the parent. */
  int idxInParent = p.indexOfChild(*this);
  HorizontalLayout castedParent = HorizontalLayout(static_cast<HorizontalLayoutNode *>(p.node()));
  if (putCursorInTheMiddle) {
    if (idxInParent > 0) {
      cursor->setLayout(castedParent.childAtIndex(idxInParent-1));
      cursor->setPosition(LayoutCursor::Position::Right);
    } else {
      cursor->setLayout(castedParent);
      cursor->setPosition(LayoutCursor::Position::Left);
    }
  }
  idxInParent -= p.removeChild(*this, cursor->layout() == *this ? cursor : nullptr, true);
  castedParent.addOrMergeChildAtIndex(rightChild, idxInParent, true);
  castedParent.addOrMergeChildAtIndex(leftChild, idxInParent, true, putCursorInTheMiddle ? cursor : nullptr);
}

void Layout::addChildAtIndex(Layout l, int index, int currentNumberOfChildren, LayoutCursor * cursor) {
  int newIndex = index;
  int newCurrentNumberOfChildren = currentNumberOfChildren;
  if (!node()->willAddChildAtIndex(l.node(), &newIndex, &newCurrentNumberOfChildren, cursor)) {
    return;
  }
  Layout nextPointedLayout;
  LayoutCursor::Position nextPosition = LayoutCursor::Position::Left;
  if (cursor != nullptr) {
    if (newIndex < this->numberOfChildren()) {
      nextPointedLayout = childAtIndex(newIndex);
      nextPosition = LayoutCursor::Position::Left;
    } else {
      nextPointedLayout = *this;
      nextPosition = LayoutCursor::Position::Right;
    }
  }

  addChildAtIndexInPlace(l, newIndex, newCurrentNumberOfChildren);

  if (cursor != nullptr) {
    cursor->setLayout(nextPointedLayout);
    cursor->setPosition(nextPosition);
  }
}

void Layout::addSibling(LayoutCursor * cursor, Layout * sibling, bool moveCursor) {
  if (!node()->willAddSibling(cursor, sibling, moveCursor)) {
    return;
  }
  /* The layout must have a parent, because HorizontalLayout's
   * preprocessAddSibling returns false only an HorizontalLayout can be the
   * root layout. */
  Layout rootLayout = root();
  Layout p = parent();
  assert(!p.isUninitialized());
  if (p.type() == LayoutNode::Type::HorizontalLayout) {
    int indexInParent = p.indexOfChild(*this);
    int indexOfCursor = p.indexOfChild(cursor->layout());
    /* indexOfCursor == -1 if cursor->layout() is not a child of p. This should
     * never happen, as addSibling is only called from inside
     * LayoutField::handleEventWithText, and LayoutField is supposed to keep
     * its cursor up to date.*/
    assert(indexOfCursor >= 0);
    int siblingIndex = cursor->position() == LayoutCursor::Position::Left ? indexOfCursor : indexOfCursor + 1;

    /* Special case: If the neighbour sibling is a VerticalOffsetLayout, let it
     * handle the insertion of the new sibling. Do not enter the special case if
     * "this" is a VerticalOffsetLayout, to avoid an infinite loop. */
    if (type() != LayoutNode::Type::VerticalOffsetLayout) {
      Layout neighbour;
      if (cursor->position() == LayoutCursor::Position::Left && indexInParent > 0) {
        neighbour = p.childAtIndex(indexInParent - 1);
      } else if (cursor->position() == LayoutCursor::Position::Right && indexInParent < p.numberOfChildren() - 1) {
        neighbour = p.childAtIndex(indexInParent + 1);
      }
      if (!neighbour.isUninitialized() && neighbour.type() == LayoutNode::Type::VerticalOffsetLayout) {
        if (moveCursor) {
          cursor->setLayout(neighbour);
          cursor->setPosition(cursor->position() == LayoutCursor::Position::Left ? LayoutCursor::Position::Right : LayoutCursor::Position::Left);
        }
        neighbour.addSibling(cursor, sibling, moveCursor);
        return;
      }
    }

    // Else, let the parent add the sibling.
    HorizontalLayout(static_cast<HorizontalLayoutNode *>(p.node())).addOrMergeChildAtIndex(*sibling, siblingIndex, true, moveCursor ? cursor : nullptr);
    return;
  }
  if (cursor->position() == LayoutCursor::Position::Left) {
    replaceWithJuxtapositionOf(*sibling, *this, cursor, moveCursor);
  } else {
    assert(cursor->position() == LayoutCursor::Position::Right);
    replaceWithJuxtapositionOf(*this, *sibling, cursor);
  }
}

int Layout::removeChild(Layout l, LayoutCursor * cursor, bool force) {
  int childrenRemovedLeftOfIndex = node()->willRemoveChild(l.node(), cursor, force);
  if (childrenRemovedLeftOfIndex != -1) {
    return childrenRemovedLeftOfIndex;
  }
  assert(hasChild(l));
  int index = indexOfChild(l);
  removeChildInPlace(l, l.numberOfChildren());
  if (cursor) {
    if (index < numberOfChildren()) {
      Layout newCursorRef = childAtIndex(index);
      cursor->setLayout(newCursorRef);
      cursor->setPosition(LayoutCursor::Position::Left);
    } else {
      int newPointedLayoutIndex = index - 1;
      if (newPointedLayoutIndex >= 0 && newPointedLayoutIndex < numberOfChildren()) {
        cursor->setLayout(childAtIndex(newPointedLayoutIndex));
        cursor->setPosition(LayoutCursor::Position::Right);
      } else {
        cursor->setLayout(*this);
        cursor->setPosition(LayoutCursor::Position::Right);
      }
    }
  }
  return node()->didRemoveChildAtIndex(index, cursor, force);
}

int Layout::removeChildAtIndex(int index, LayoutCursor * cursor, bool force) {
  return removeChild(childAtIndex(index), cursor, force);
}

bool Layout::collapseOnDirection(HorizontalDirection direction, int absorbingChildIndex, LayoutCursor * cursor) {
  Layout p = parent();
  if (p.isUninitialized() || p.type() != LayoutNode::Type::HorizontalLayout) {
    return false;
  }
  int idxInParent = p.indexOfChild(*this);
  int numberOfSiblings = p.numberOfChildren();
  int numberOfOpenParenthesis = 0;
  bool canCollapse = true;
  Layout absorbingChild = childAtIndex(absorbingChildIndex);
  if (absorbingChild.isUninitialized() || !absorbingChild.isEmpty()) {
    return false;
  }
  if (absorbingChild.type() != LayoutNode::Type::HorizontalLayout) {
    Layout horizontal = HorizontalLayout::Builder();
    replaceChild(absorbingChild, horizontal, cursor, true);
    horizontal.addChildAtIndexInPlace(absorbingChild, 0, 0);
    absorbingChild = horizontal;
  }
  HorizontalLayout horizontalAbsorbingChild = HorizontalLayout(static_cast<HorizontalLayoutNode *>(absorbingChild.node()));
  if (direction == HorizontalDirection::Right && idxInParent < numberOfSiblings - 1) {
    canCollapse = !(p.childAtIndex(idxInParent+1).mustHaveLeftSibling());
  } else if (direction == HorizontalDirection::Left && idxInParent > 0) {
    canCollapse = !(p.childAtIndex(idxInParent - 1).mustHaveRightSibling());
  }
  Layout sibling;
  bool forceCollapse = false;
  while (canCollapse) {
    if (direction == HorizontalDirection::Right && idxInParent == numberOfSiblings - 1) {
      break;
    }
    if (direction == HorizontalDirection::Left && idxInParent == 0) {
      break;
    }
    int siblingIndex = direction == HorizontalDirection::Right ? idxInParent+1 : idxInParent-1;
    sibling = p.childAtIndex(siblingIndex);
    /* Even if forceCollapse is true, isCollapsable should be called to update
     * the number of open parentheses. */
    bool shouldCollapse = sibling.isCollapsable(&numberOfOpenParenthesis, direction == HorizontalDirection::Left);
    if (shouldCollapse || forceCollapse) {
      /* If the collapse direction is Left and the next sibling to be collapsed
       * must have a left sibling, force the collapsing of this needed left
       * sibling. */
      forceCollapse = (direction == HorizontalDirection::Left && sibling.mustHaveLeftSibling()) ||(direction == HorizontalDirection::Right && sibling.mustHaveRightSibling());
      siblingIndex -= p.removeChild(sibling, nullptr);
      // siblingIndex is decremented in case it is used later.
      (void)siblingIndex;
      int newIndex = direction == HorizontalDirection::Right ? absorbingChild.numberOfChildren() : 0;
      horizontalAbsorbingChild.addOrMergeChildAtIndex(sibling, newIndex, true);
      // removeChildAtIndex may have removed more than one child.
      numberOfSiblings = p.numberOfChildren();
      idxInParent = p.indexOfChild(*this);
    } else {
      break;
    }
  }
  return true;
}

void Layout::collapseSiblings(LayoutCursor * cursor) {
  bool collapsed = false;
  if (node()->shouldCollapseSiblingsOnRight()) {
    collapsed |= collapseOnDirection(HorizontalDirection::Right, rightCollapsingAbsorbingChildIndex(), cursor);
  }
  if (node()->shouldCollapseSiblingsOnLeft()) {
    collapsed |= collapseOnDirection(HorizontalDirection::Left, leftCollapsingAbsorbingChildIndex(), cursor);
  }
  if (collapsed) {
    node()->didCollapseSiblings(cursor);
  }
}

bool Layout::privateHasTopLevelComparisonSymbol(bool includingNotEqualSymbol) const {
  if (type() != Poincare::LayoutNode::Type::HorizontalLayout) {
    return false;
  }
  const int childrenCount = numberOfChildren();
  for (int i = 0; i < childrenCount; i++) {
    Poincare::Layout child = childAtIndex(i);
    if ((child.type() == Poincare::LayoutNode::Type::CodePointLayout && static_cast<Poincare::CodePointLayout &>(child).codePoint().isEquationOperator())
     || (includingNotEqualSymbol && child.type() == Poincare::LayoutNode::Type::CombinedCodePointsLayout && static_cast<Poincare::CombinedCodePointsLayout &>(child).node()->isNotEqualOperator())) {
      return true;
    }
  }
  return false;
}

}
