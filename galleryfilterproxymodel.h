#ifndef GALLERYFILTERPROXYMODEL_H
#define GALLERYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class GalleryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    GalleryFilterProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool filter() const;

public slots:
    void setFilter(bool setFilter);

signals:
    void filterChanged(bool setFilter);

private:
    bool m_filter = false;
};

#endif // GALLERYFILTERPROXYMODEL_H
