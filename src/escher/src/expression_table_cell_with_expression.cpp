#include <escher/expression_table_cell_with_expression.h>
#include <escher/container.h>
#include <escher/palette.h>
#include <assert.h>

namespace Escher {

ExpressionTableCellWithExpression::ExpressionTableCellWithExpression(Responder * parentResponder, KDFont::Size font) :
    ExpressionTableCell(parentResponder, font),
    m_subLabelExpressionView(this, 0, 0, KDContext::k_alignRight, KDContext::k_alignCenter, Palette::GrayDark, KDColorWhite, font)
  {}

void ExpressionTableCellWithExpression::setHighlighted(bool highlight) {
  ExpressionTableCell::setHighlighted(highlight);
  KDColor backgroundColor = highlight? Palette::Select : KDColorWhite;
  m_subLabelExpressionView.setBackgroundColor(backgroundColor);
}

void ExpressionTableCellWithExpression::setSubLabelLayout(Poincare::Layout l) {
  m_subLabelExpressionView.setLayout(l);
  layoutSubviews();
}

void ExpressionTableCellWithExpression::didBecomeFirstResponder() {
  Container::activeApp()->setFirstResponder(&m_subLabelExpressionView);
}

}
