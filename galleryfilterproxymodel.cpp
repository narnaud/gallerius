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

    if (m_showAll)
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    return !index.data(GalleryModel::ExcludedRole).toBool();
}

bool GalleryFilterProxyModel::isAllVisible() const
{
    return m_showAll;
}

void GalleryFilterProxyModel::showAll(bool showAll)
{
    if (m_showAll == showAll)
        return;

    m_showAll = showAll;
    emit showAllChanged(m_showAll);
    invalidateFilter();
}
