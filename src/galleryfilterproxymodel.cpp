#include "galleryfilterproxymodel.h"

#include "data.h"
#include "gallerymodel.h"

GalleryFilterProxyModel::GalleryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool GalleryFilterProxyModel::filterAcceptsRow(int source_row,
                                               const QModelIndex &source_parent) const
{
    if (!m_filter)
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    return !index.data(GalleryModel::FilterRole).toBool();
}

bool GalleryFilterProxyModel::filter() const
{
    return m_filter;
}

bool GalleryFilterProxyModel::isDirectory(int proxy_row)
{
    QModelIndex sourceIndex = mapToSource(index(proxy_row, 0));
    return sourceIndex.data(GalleryModel::TypeRole).value<Media::Type>() == Media::Dir;
}

void GalleryFilterProxyModel::setFilter(bool showAll)
{
    if (m_filter == showAll)
        return;

    m_filter = showAll;
    emit filterChanged(m_filter);
    invalidateFilter();
}
