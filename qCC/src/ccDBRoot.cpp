#include "ccDBRoot.h"

ccCustomQTreeView::ccCustomQTreeView(QWidget* parent) : QTreeView(parent) {}

QItemSelectionModel::SelectionFlags ccCustomQTreeView::selectionCommand(const QModelIndex& index,
                                                                        const QEvent* event) const {
  return QTreeView::selectionCommand(index, event);
}