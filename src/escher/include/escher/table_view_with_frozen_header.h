#ifndef ESCHER_TABLE_VIEW_WITH_FROZEN_HEADER_H
#define ESCHER_TABLE_VIEW_WITH_FROZEN_HEADER_H

#include <escher/view.h>
#include <escher/selectable_table_view.h>

namespace Escher {

class TableViewWithFrozenHeader: public Escher::View {
public:
  TableViewWithFrozenHeader(SelectableTableView * selectableTableView, I18n::Message header);
  void drawRect(KDContext * ctx, KDRect rect) const override;
  void layoutSubviews(bool force = false) override;
  void resetSize() { m_selectableTableView->setSize(KDSizeZero); }
private:
  constexpr static KDCoordinate k_textMargin = Metric::CommonSmallMargin;
  // Removing a pixel to skew title's baseline downward.
  constexpr static KDCoordinate k_titleMargin = Metric::CommonTopMargin - 1;
  int numberOfSubviews() const override { return 2; };
  View * subviewAtIndex(int index) override;
  MessageTextView m_headerView;
  SelectableTableView * m_selectableTableView;
};

}

#endif /* ESCHER_TABLE_VIEW_WITH_FROZEN_HEADER_H */
