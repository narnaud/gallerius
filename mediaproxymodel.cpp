#include "mediaproxymodel.h"

#include "data.h"
#include "gallerymodel.h"

MediaProxyModel::MediaProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MediaProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);
    return index.data(GalleryModel::TypeRole).value<Media::Type>() != Media::Dir;
}

int MediaProxyModel::mapRowFromSource(int source_row)
{
    return mapFromSource(sourceModel()->index(source_row, 0)).row();
}
