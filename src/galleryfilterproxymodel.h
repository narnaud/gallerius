#ifndef GALLERYFILTERPROXYMODEL_H
#define GALLERYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class GalleryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum Filter {
        All,
        OnlyVisible,
        OnlyHidden,
    };

    GalleryFilterProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

public slots:
    void setFilter(Filter f);

private:
    Filter m_filter = All;
};

#endif // GALLERYFILTERPROXYMODEL_H
