#include "galleryfilterproxymodel.h"

#include "gallerymodel.h"

GalleryFilterProxyModel::GalleryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool GalleryFilterProxyModel::filterAcceptsRow(int source_row,
                                               const QModelIndex &source_parent) const
{
    Q_UNUSED(source_row);

    if (!m_filter)
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    return !index.data(GalleryModel::FilterRole).toBool();
}

bool GalleryFilterProxyModel::filter() const
{
    return m_filter;
}

void GalleryFilterProxyModel::setFilter(bool showAll)
{
    if (m_filter == showAll)
        return;

    m_filter = showAll;
    emit filterChanged(m_filter);
    invalidateFilter();
}
