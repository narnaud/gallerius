#include "galleryfilterproxymodel.h"

#include "gallerymodel.h"

GalleryFilterProxyModel::GalleryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool GalleryFilterProxyModel::filterAcceptsRow(int source_row,
                                               const QModelIndex &source_parent) const
{
    if (m_filter == All)
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    const bool filtered = index.data(GalleryModel::FilterRole).toBool();
    return (!filtered && (m_filter == OnlyVisible)) || (filtered && (m_filter == OnlyHidden));
}

void GalleryFilterProxyModel::setFilter(GalleryFilterProxyModel::Filter f)
{
    if (m_filter == f)
        return;

    m_filter = f;
    invalidateFilter();
}
