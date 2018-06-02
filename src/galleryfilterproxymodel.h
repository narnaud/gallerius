#ifndef GALLERYFILTERPROXYMODEL_H
#define GALLERYFILTERPROXYMODEL_H

#include "data.h"

#include <QSortFilterProxyModel>

class GalleryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    GalleryFilterProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool filter() const;

    Q_INVOKABLE bool isDirectory(int proxy_row);

public slots:
    void setFilter(bool setFilter);

signals:
    void filterChanged(bool setFilter);

private:
    bool m_filter = false;
};

#endif // GALLERYFILTERPROXYMODEL_H
