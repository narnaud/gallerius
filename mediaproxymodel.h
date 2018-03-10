#ifndef MEDIAPROXYMODEL_H
#define MEDIAPROXYMODEL_H

#include <QSortFilterProxyModel>

class MediaProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MediaProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

public slots:
    int mapRowFromSource(int source_row);
};

#endif // MEDIAPROXYMODEL_H
