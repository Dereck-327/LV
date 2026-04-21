#pragma once

#include <QTreeView>

class ccCustomQTreeView : public QTreeView {
  Q_OBJECT

 public:
  explicit ccCustomQTreeView(QWidget* parent = nullptr);

 protected:
  QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex& index,
                                                       const QEvent* event = nullptr) const override;
};