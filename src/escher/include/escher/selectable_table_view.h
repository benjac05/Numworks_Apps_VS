#ifndef ESCHER_SELECTABLE_TABLE_VIEW_H
#define ESCHER_SELECTABLE_TABLE_VIEW_H

#include <escher/table_view.h>
#include <escher/selectable_table_view_data_source.h>
#include <escher/selectable_table_view_delegate.h>
#include <escher/table_view_data_source.h>
#include <escher/palette.h>
#include <escher/responder.h>

namespace Escher {

/* SelectableTableView is a Table View that handles selection. To implement it,
 * it needs a class which should be both data source and view controller. This
 * takes the selectable table view as instance variable and makes it first
 * responder. The selectable table view bubbles up events when they do not
 * concern selection. */
class SelectableTableView : public TableView, public Responder {
public:
  SelectableTableView(Responder * parentResponder, TableViewDataSource * dataSource,
      SelectableTableViewDataSource * selectionDataSource = nullptr, SelectableTableViewDelegate * delegate = nullptr);
  template <typename T> SelectableTableView(T * p) : SelectableTableView(p, p, p) {};

  void setDelegate(SelectableTableViewDelegate * delegate) { m_delegate = delegate; }
  int selectedRow() { return m_selectionDataSource->selectedRow(); }
  int selectedColumn() { return m_selectionDataSource->selectedColumn(); }
  void selectRow(int j) { m_selectionDataSource->selectRow(j); }
  void selectColumn(int i) { m_selectionDataSource->selectColumn(i); }

  HighlightCell * selectedCell();
  bool selectCellAtLocation(int col, int row, bool setFirstResponder = true, bool withinTemporarySelection = false);
  bool selectCellAtClippedLocation(int col, int row, bool setFirstResponder = true, bool withinTemporarySelection = false);

  bool handleEvent(Ion::Events::Event event) override;
  void unhighlightSelectedCell();
  void deselectTable(bool withinTemporarySelection = false);
  void reloadData(bool setFirstResponder = true, bool setSelection = true);

  void didBecomeFirstResponder() override;
  void didEnterResponderChain(Responder * previousFirstResponder) override;
  void willExitResponderChain(Responder * nextFirstResponder) override;

protected:
  SelectableTableViewDataSource * m_selectionDataSource;
  SelectableTableViewDelegate * m_delegate;

private:
  bool cellAtLocationIsSelectable(int col, int row) { return dataSource()->cellAtLocationIsSelectable(cellAtLocation(col, row), col, row); }
  /* This function searches (for delta = n) :
   * - The n-th next selectable row in the currentCol starting from currentRow
   *   if searchForRow = true
   * - The n-th next selectable col in the currentRow starting from currentCol
   *   if searchForRow = false
   * If delta < 0, it searches backwards.
   * If the found index goes out of bounds, return the last valid index.
   *
   * Examples:
   * delta = 2, searchForRow = false (only currentRow is displayed)
   * o = selectable, x = non selectable
   *
   * Col index: | 0 | 1 | 2 | 3 | 4 |
   * Example 1: | o | o | x | o | o | currentCol = 0 -> resultCol = 3
   * Example 2: | o | x | x | o | o | currentCol = 0 -> resultCol = 4
   * Example 3: | o | x | x | o | x | currentCol = 2 -> resultCol = 3
   * Example 4: | o | x | x | o | x | currentCol = 4 -> resultCol = 3
   * */
  int indexOfNextSelectableColumnOrRow(int delta, int currentCol, int currentRow, bool searchForRow);
  int indexOfNextSelectableRow(int delta, int currentColumn, int currentRow) { return indexOfNextSelectableColumnOrRow(delta, currentColumn, currentRow, true); }
  int indexOfNextSelectableColumn(int delta, int currentColumn, int currentRow) { return indexOfNextSelectableColumnOrRow(delta, currentColumn, currentRow, false); }
  int firstSelectableColumnOrRow(bool searchForRoww);
  int firstOrLastSelectableColumnOrRow(bool first, bool searchForRow);
};

}
#endif
